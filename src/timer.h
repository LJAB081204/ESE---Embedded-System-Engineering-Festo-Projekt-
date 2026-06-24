#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <time.h>
#include <pthread.h>
#include <vector>

class TimerHandler;

// ===========================================================================
// SimpleTimer
// ===========================================================================
/**
 * Einmaliger QNX-Systemtimer, der bei Ablauf einen Pulse an den
 * Dispatcher-Channel sendet.

 * @note Der SimpleTimer nutzt den POSIX-Systemtimer (timer_create) direkt und
 *       läuft unabhängig vom TimerHandler. Er hat keine Slow-/Pause-Funktion.
 *       Der im Konstruktor übergebene coid wird im Destruktor per
 *       ConnectDetach freigegeben.
 */
class SimpleTimer {
public:
    SimpleTimer(int dispatcherCoid, int pulseMessage);
    ~SimpleTimer();
    void     start(uint32_t millisec);
    uint32_t stop();   // gibt abgelaufene Zeit in ms zurück

private:
    int        coid_;
    timer_t    timerId_;
    uint32_t   startMs_ = 0;   // gesetzte Laufzeit beim letzten start()
};


// ===========================================================================
// DynamicTimer
// ===========================================================================
/**
 * Virtueller Software-Timer, der vom TimerHandler getaktet wird.
 *
 * Im Gegensatz zum SimpleTimer besitzt der DynamicTimer keinen eigenen
 * Systemtimer. Er wird durch den TICK_MS-Takt des TimerHandlers vorwärts
 * gezählt und unterstützt damit Pause- und Slow-Motion-Funktionen.
 *
 * @note DynamicTimer registriert sich im Konstruktor selbst beim übergebenen
 *       TimerHandler und meldet sich im Destruktor wieder ab. Die Lebensdauer
 *       des TimerHandlers muss die des DynamicTimers überschreiten.
 */
class DynamicTimer
{
public:

	//bool expired_ = false;
    /**
     * Erstellt den virtuellen Timer und registriert ihn im Handler.
     * @param id      Frei wählbare Timer-ID; wird als Pulse-Value übertragen.
     * @param handler Referenz auf den zugehörigen TimerHandler.
     */
    DynamicTimer(int id, TimerHandler& handler);

    /**
     * Stoppt den Timer und meldet ihn beim TimerHandler ab.
     */
    ~DynamicTimer();

    /**
     * Startet den virtuellen Countdown.
     * @param millisec Zielzeit in (virtuellen) Millisekunden. Der interne
     *                 Zähler beginnt bei 0 und wird je TICK_MS-Takt um
     *                 (TICK_MS * slowFactor) erhöht.
     */
    void start(uint32_t millisec, uint32_t runnedTime);

    /**
     * Bricht den Countdown ab und setzt den internen Zähler zurück.
     *        Kein Pulse wird gesendet.
     */
    void stop();

    /**
     * Liefert den aktuellen virtuellen Zählerstand in Millisekunden.
     * @return Bisher abgelaufene virtuelle Zeit seit dem letzten start()-Aufruf.
     */
    uint32_t getTime() const;

private:
    int           id_;           ///< Timer-ID für den Expired-Pulse
    uint32_t      targetTime_;   ///< Zielzeit in ms (gesetzt durch start())
    uint32_t      virtualTime_;  ///< Bisher gezählte virtuelle Zeit in ms
    bool          running_;      ///< true, solange der Timer läuft
    TimerHandler& handler_;      ///< Zugehöriger TimerHandler

    friend class TimerHandler;   ///< Handler darf private Felder direkt lesen/schreiben
};


// ===========================================================================
// TimerHandler
// ===========================================================================
/**
 * Verwaltet alle DynamicTimer und stellt Pause- sowie
 *        Zeitlupenfunktionalität bereit.
 *
 * Der TimerHandler erzeugt intern einen periodischen Systemtimer
 * (Auflösung: TICK_MS = 10 ms) und einen eigenen Receiver-Thread.
 * Bei jedem Tick wird virtualTime_ aller registrierten DynamicTimer
 * um (TICK_MS * slowFactor_) erhöht. Läuft ein Timer ab, sendet der
 * Handler einen PULSE_TIMER_EXPIRED-Pulse an den Dispatcher-Channel.
 *
 * @note Der TimerHandler ist Thread-sicher. Alle öffentlichen Methoden
 *       und processTick() schützen den gemeinsamen Zustand mit mutex_.
 *       mutex_ ist public, damit DynamicTimer::getTime() darauf zugreifen kann.
 */
class TimerHandler
{
public:

    /**
     * Initialisiert den internen Channel, den Receiver-Thread und
     *        den periodischen Systemtimer.
     * @param dispatcherCoid Connection-ID des Dispatcher-Channels, an den
     *                       Expired-Pulses der DynamicTimer weitergeleitet werden.
     */
    explicit TimerHandler(int dispatcherCoid);

    /**
     * Löscht den Systemtimer, beendet den Receiver-Thread und gibt
     *        alle Ressourcen (Channel, Connection, Mutex) frei.
     */
    ~TimerHandler();

    /**
     * Hält alle registrierten DynamicTimer an. virtualTime_ wird
     *        eingefroren; kein Pulse wird ausgelöst.
     */
    void pauseAll();

    /**
     * Setzt alle pausierten DynamicTimer fort.
     */
    void resumeAll();

    /**
     * Verlangsamt alle DynamicTimer.
     * @param factor Verlangsamungsfaktor (> 1.0 = langsamer, 1.0 = normal).
     *               Jeder Tick erhöht virtualTime_ nur um TICK_MS / factor.
     *               Beispiel: factor = 2.0 → Timer läuft halb so schnell.
     */
    void setSlow(double factor);

    /**
     * Setzt den Verlangsamungsfaktor auf 1.0 (Echtzeit) zurück.
     */
    void setNormal();

    /**
     * Registriert einen DynamicTimer (wird vom DynamicTimer-Konstruktor
     *        automatisch aufgerufen).
     * @param t Zeiger auf den zu registrierenden Timer.
     */
    void registerTimer(DynamicTimer* t);

    /**
     * Entfernt einen DynamicTimer aus der internen Liste (wird vom
     *        DynamicTimer-Destruktor automatisch aufgerufen).
     * @param t Zeiger auf den abzumeldenden Timer.
     */
    void unregisterTimer(DynamicTimer* t);
    void setCoid(int dispCoid);

    mutable pthread_mutex_t mutex_; ///< Schützt timers_, paused_, slowFactor_
                                    ///< sowie die Felder aller DynamicTimer.
                                    ///< public, da DynamicTimer::getTime() ihn benötigt.

private:
    static const uint32_t TICK_MS = 10; ///< Systemtimer-Auflösung in Millisekunden

    int       chidInternal_;    ///< Interner Channel für Tick-Pulses des Systemtimers
    int       coidInternal_;    ///< Connection auf den internen Channel
    int       coidDispatcher_;  ///< Connection auf den Dispatcher-Channel
    timer_t   sysTick_;         ///< Handle des periodischen Systemtimers
    pthread_t thread_;          ///< Receiver-Thread für Tick-Pulses

    double slowFactor_;                    ///< Aktueller Verlangsamungsfaktor (Standard: 1.0)
    bool   paused_;                        ///< true, wenn alle Timer pausiert sind
    std::vector<DynamicTimer*> timers_;    ///< Liste aller registrierten DynamicTimer

    /**
     * Wird bei jedem Tick unter gehaltener mutex_ aufgerufen.
     *        Erhöht virtualTime_ aller laufenden Timer und löst Expired-Pulses aus.
     */
    void processTick();

    /**
     * Einstiegspunkt des Receiver-Threads.
     *        Empfängt Tick-Pulses und delegiert an processTick().
     */
    static void* receiverThread(void* arg);
};

#endif
