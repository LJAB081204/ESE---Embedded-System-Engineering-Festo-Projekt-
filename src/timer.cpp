#include "timer.h"
#include <sys/neutrino.h>
#include <cstdio>
#include <cstdlib>
#include "PulseMessages.h"

// Pulse-Code für interne Tick-Pulse des Systemtimers
static const int TIMER_TICK = _PULSE_CODE_MINAVAIL;


// ===========================================================================
// SimpleTimer
// ===========================================================================

SimpleTimer::SimpleTimer(int id, int dispatcherCoid)
    : id_(id), coid_(dispatcherCoid)
{
    // sigevent konfigurieren: Bei Timer-Ablauf wird ein Pulse mit Code
    // PULSE_TIMER_EXPIRED und Value id_ an den Dispatcher-Channel gesendet.
    struct sigevent event;
    SIGEV_PULSE_INIT(&event, coid_, SIGEV_PULSE_PRIO_INHERIT, PULSE_TIMER_EXPIRED, id_);

    // Monotonischen POSIX-Timer anlegen (nicht durch Systemzeit-Änderungen beeinflussbar)
    timer_create(CLOCK_MONOTONIC, &event, &timerId_);
}

SimpleTimer::~SimpleTimer()
{
    timer_delete(timerId_);   // Systemtimer-Ressource freigeben
}

void SimpleTimer::start(uint32_t millisec)
{
    struct itimerspec its;
    // Ablaufzeit einstellen: Millisekunden in Sekunden + Nanosekunden aufteilen
    its.it_value.tv_sec     = millisec / 1000;
    its.it_value.tv_nsec    = (millisec % 1000) * 1000000L;
    // Kein periodisches Wiederholen – Einmaliger Ablauf
    its.it_interval.tv_sec  = 0;
    its.it_interval.tv_nsec = 0;
    timer_settime(timerId_, 0, &its, nullptr);
}

void SimpleTimer::stop()
{
    // Alle Felder auf 0 setzen deaktiviert den Timer sofort
    struct itimerspec its;
    memset(&its, 0, sizeof(its));
    timer_settime(timerId_, 0, &its, nullptr);
}


// ===========================================================================
// DynamicTimer
// ===========================================================================

DynamicTimer::DynamicTimer(int id, TimerHandler& handler)
    : id_(id), targetTime_(0), virtualTime_(0), running_(false), handler_(handler)
{
    // Timer beim Handler registrieren, damit er bei jedem Tick berücksichtigt wird
    handler_.registerTimer(this);
}

DynamicTimer::~DynamicTimer()
{
    // Aus der Handler-Liste entfernen, bevor das Objekt zerstört wird,
    // um Zugriffe auf ungültige Speicherbereiche im Receiver-Thread zu verhindern
    handler_.unregisterTimer(this);
}

void DynamicTimer::start(uint32_t millisec)
{
    pthread_mutex_lock(&handler_.mutex_);
    targetTime_  = millisec; // Zielzeit setzen
    virtualTime_ = 0;        // Zähler zurücksetzen
    running_     = true;     // Timer aktivieren
    pthread_mutex_unlock(&handler_.mutex_);
}

void DynamicTimer::stop()
{
    pthread_mutex_lock(&handler_.mutex_);
    running_     = false; // Timer deaktivieren – kein Pulse wird gesendet
    virtualTime_ = 0;     // Zähler zurücksetzen
    targetTime_  = 0;
    pthread_mutex_unlock(&handler_.mutex_);
}

uint32_t DynamicTimer::getTime() const
{
    // Mutex schützt den Lesezugriff gegenüber dem Receiver-Thread
    pthread_mutex_lock(&handler_.mutex_);
    uint32_t t = virtualTime_;
    pthread_mutex_unlock(&handler_.mutex_);
    return t;
}


// ===========================================================================
// TimerHandler
// ===========================================================================

TimerHandler::TimerHandler(int dispatcherCoid)
    : chidInternal_(-1), coidInternal_(-1), coidDispatcher_(dispatcherCoid),
      slowFactor_(1.0), paused_(false)
{
    pthread_mutex_init(&mutex_, nullptr);

    // Eigenen internen Channel anlegen, der ausschließlich Tick-Pulses empfängt.
    // Trennung vom Dispatcher-Channel verhindert, dass Tick-Pulses im
    // Anwendungscode auftauchen.
    chidInternal_ = ChannelCreate(0);
    if (chidInternal_ == -1) { perror("ChannelCreate"); exit(EXIT_FAILURE); }

    // Connection auf den internen Channel – wird vom Systemtimer als Ziel genutzt
    coidInternal_ = ConnectAttach(0, 0, chidInternal_, _NTO_SIDE_CHANNEL, 0);
    if (coidInternal_ == -1) { perror("ConnectAttach internal"); exit(EXIT_FAILURE); }

    // Receiver-Thread starten, der Tick-Pulses auf dem internen Channel empfängt
    pthread_create(&thread_, nullptr, receiverThread, this);

    // Systemtimer konfigurieren: Jeder Ablauf sendet einen TIMER_TICK-Pulse
    // an den internen Channel
    struct sigevent event;
    SIGEV_PULSE_INIT(&event, coidInternal_, SIGEV_PULSE_PRIO_INHERIT, TIMER_TICK, 0);
    timer_create(CLOCK_MONOTONIC, &event, &sysTick_);

    // Periodischen Takt einrichten: Erstablauf und Wiederholintervall = TICK_MS
    struct itimerspec its;
    its.it_value.tv_sec     = 0;
    its.it_value.tv_nsec    = TICK_MS * 1000000L;
    its.it_interval.tv_sec  = 0;
    its.it_interval.tv_nsec = TICK_MS * 1000000L;
    timer_settime(sysTick_, 0, &its, nullptr);
}

TimerHandler::~TimerHandler()
{
    timer_delete(sysTick_);           // Systemtimer anhalten und freigeben
    pthread_cancel(thread_);          // Receiver-Thread zum Abbruch auffordern
    pthread_join(thread_, nullptr);   // Auf sauberes Beenden warten
    ConnectDetach(coidInternal_);     // Interne Connection trennen
    ChannelDestroy(chidInternal_);    // Internen Channel zerstören
    pthread_mutex_destroy(&mutex_);
}

void TimerHandler::pauseAll()
{
    pthread_mutex_lock(&mutex_);
    paused_ = true;  // processTick() wird bei gesetztem Flag übersprungen
    pthread_mutex_unlock(&mutex_);
}

void TimerHandler::resumeAll()
{
    pthread_mutex_lock(&mutex_);
    paused_ = false; // processTick() läuft wieder normal durch
    pthread_mutex_unlock(&mutex_);
}

void TimerHandler::setSlow(double factor)
{
    pthread_mutex_lock(&mutex_);
    // Faktor > 1.0: pro Tick wird weniger virtuelle Zeit addiert → Timer läuft langsamer
    slowFactor_ = factor;
    pthread_mutex_unlock(&mutex_);
}

void TimerHandler::setNormal()
{
    pthread_mutex_lock(&mutex_);
    slowFactor_ = 1.0; // Echtzeit: increment = TICK_MS
    pthread_mutex_unlock(&mutex_);
}

void TimerHandler::registerTimer(DynamicTimer* t)
{
    pthread_mutex_lock(&mutex_);
    timers_.push_back(t);
    pthread_mutex_unlock(&mutex_);
}

void TimerHandler::unregisterTimer(DynamicTimer* t)
{
    pthread_mutex_lock(&mutex_);
    // Lineares Durchsuchen der Liste und Entfernen des ersten Treffers
    for (auto it = timers_.begin(); it != timers_.end(); ++it) {
        if (*it == t) { timers_.erase(it); break; }
    }
    pthread_mutex_unlock(&mutex_);
}

// Wird unter gehaltener mutex_ aufgerufen (kein erneutes Locking nötig)
void TimerHandler::processTick()
{
    if (paused_) return; // Alle Timer eingefroren → nichts tun

    // Virtuellen Zeitinkrement berechnen:
    // Bei slowFactor_ = 1.0 entspricht increment = TICK_MS (Echtzeit).
    // Bei slowFactor_ = 2.0 ist increment = 5 ms → Timer braucht doppelt so lange.
    uint32_t increment = (uint32_t)(TICK_MS * slowFactor_);
    //if (increment == 0) increment = 1; // Sicherheit bei sehr großem slowFactor_ (deaktiviert)

    for (DynamicTimer* t : timers_) {
        if (!t->running_) continue; // Nicht laufende Timer überspringen

        t->virtualTime_ += increment;

        // Prüfen, ob Zielzeit erreicht oder überschritten wurde
        if (t->virtualTime_ >= t->targetTime_) {
            t->running_     = false;           // Timer deaktivieren
            t->virtualTime_ = t->targetTime_;  // Zähler auf exakte Zielzeit kappen

            // Expired-Pulse mit der Timer-ID als Value an den Dispatcher senden
            MsgSendPulse(coidDispatcher_, SIGEV_PULSE_PRIO_INHERIT, PULSE_TIMER_EXPIRED, t->id_);
        }
    }
}

void* TimerHandler::receiverThread(void* arg)
{
    TimerHandler* self = (TimerHandler*)arg;
    struct _pulse pulse;

    while(true) {
        // Blockiert bis zum nächsten Tick-Pulse auf dem internen Channel
        MsgReceivePulse(self->chidInternal_, &pulse, sizeof(pulse), nullptr);

        // Mutex holen und alle registrierten Timer um einen Takt vorwärts zählen
        pthread_mutex_lock(&self->mutex_);
        self->processTick();
        pthread_mutex_unlock(&self->mutex_);
    }
    return nullptr;
}
