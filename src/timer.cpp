#include "timer.h"

#include <iostream>

// ─────────────────────────────────────────────
// Timer
// ─────────────────────────────────────────────

Timer::Timer(int id, TimerServer& server)
    : _id(id),
      _server(server)
{
    _server.registerTimer(*this);
}

Timer::~Timer()
{
    _server.unregisterTimer(_id);

    // Kernel-Timer aufräumen ohne Fehlerausgabe – der Destruktor ist kein
    // unerwarteter Zustand, egal in welchem Zustand der Timer ist.
    {
        std::lock_guard<std::mutex> lock(_mtx);

        if (_state == TimerState::RUNNING || _state == TimerState::PAUSED)
        {
            cleanup();
            std::cout << "[Timer " << _id << "] gestoppt (Destruktor)\n";
        }
    }
}

void Timer::startOnce(uint64_t durationMs)
{
    {
        std::lock_guard<std::mutex> lock(_mtx);

        // Erlaubt in IDLE und EXPIRED; in allen anderen Zuständen Fehler.
        if (_state == TimerState::RUNNING || _state == TimerState::PAUSED)
        {
            std::cerr << "[Timer " << _id << "] Fehler: Timer ist bereits aktiv (startOnce)\n";
            return;
        }

        _type        = TimerType::OneShot;
        _remainingNs = durationMs * 1000000ULL;
        _intervalNs  = 0;

        createTimer(PULSE_TIMER_EXPIRED);
        arm(_remainingNs, 0);

        _state = TimerState::RUNNING;
    }

    std::cout << "[Timer " << _id << "] gestartet (OneShot, " << durationMs << " ms)\n";
}

void Timer::startPeriodic(uint64_t intervalMs)
{
    {
        std::lock_guard<std::mutex> lock(_mtx);

        if (_state == TimerState::RUNNING || _state == TimerState::PAUSED)
        {
            std::cerr << "[Timer " << _id << "] Fehler: Timer ist bereits aktiv (startPeriodic)\n";
            return;
        }

        _type        = TimerType::Periodic;
        _remainingNs = intervalMs * 1000000ULL;
        _intervalNs  = intervalMs * 1000000ULL;

        createTimer(PULSE_TIMER_TICK);
        arm(_remainingNs, _intervalNs);

        _state = TimerState::RUNNING;
    }

    std::cout << "[Timer " << _id << "] gestartet (Periodic, " << intervalMs << " ms)\n";
}

void Timer::pause()
{
    {
        std::lock_guard<std::mutex> lock(_mtx);

        if (_state != TimerState::RUNNING)
        {
            std::cerr << "[Timer " << _id << "] Fehler: Timer läuft nicht (pause)\n";
            return;
        }

        // Verbleibende Zeit aus dem Kernel lesen bevor wir deaktivieren.
        itimerspec ts {};
        timer_gettime(_timerid, &ts);

        _remainingNs =
            static_cast<uint64_t>(ts.it_value.tv_sec)  * 1000000000ULL +
            static_cast<uint64_t>(ts.it_value.tv_nsec);

        disarm();

        _state = TimerState::PAUSED;
    }

    std::cout << "[Timer " << _id << "] pausiert\n";
}

void Timer::resume()
{
    {
        std::lock_guard<std::mutex> lock(_mtx);

        if (_state != TimerState::PAUSED)
        {
            std::cerr << "[Timer " << _id << "] Fehler: Timer ist nicht pausiert (resume)\n";
            return;
        }

        if (_type == TimerType::OneShot)
            arm(_remainingNs, 0);
        else
            arm(_remainingNs, _intervalNs);

        _state = TimerState::RUNNING;
    }

    std::cout << "[Timer " << _id << "] fortgesetzt\n";
}

void Timer::stop()
{
    {
        std::lock_guard<std::mutex> lock(_mtx);

        if (_state != TimerState::RUNNING && _state != TimerState::PAUSED)
        {
            std::cerr << "[Timer " << _id << "] Fehler: Timer ist nicht aktiv (stop)\n";
            return;
        }

        cleanup();
    }

    std::cout << "[Timer " << _id << "] gestoppt\n";
}

void Timer::onExpired()
{
    std::lock_guard<std::mutex> lock(_mtx);

    if (_state != TimerState::RUNNING)
        return;

    // Kernel-Timer freigeben ohne Log – die EXPIRED-Ausgabe macht der Server.
    disarm();
    timer_delete(_timerid);

    _state = TimerState::EXPIRED;
}

// ─────────────────────────────────────────────
// Private Hilfsmethoden
// Alle folgenden Methoden setzen voraus, dass _mtx vom Aufrufer gehalten wird.
// ─────────────────────────────────────────────

void Timer::createTimer(int pulseCode)
{
    sigevent sev {};

    // Bei Timer-Ablauf wird ein Pulse an den Server-Channel gesendet.
    // Der Pulse-Value enthält _id zur Identifikation im Server.
    SIGEV_PULSE_INIT(
        &sev,
        _server.getConid(),
        SIGEV_PULSE_PRIO_INHERIT,
        pulseCode,
        _id
    );

    if (timer_create(CLOCK_MONOTONIC, &sev, &_timerid) != 0)
    {
        perror("timer_create");
        return;
    }
}

void Timer::arm(uint64_t initialNs, uint64_t intervalNs)
{
    itimerspec ts {};

    ts.it_value.tv_sec  = static_cast<time_t>(initialNs  / 1000000000ULL);
    ts.it_value.tv_nsec = static_cast<long>  (initialNs  % 1000000000ULL);

    ts.it_interval.tv_sec  = static_cast<time_t>(intervalNs / 1000000000ULL);
    ts.it_interval.tv_nsec = static_cast<long>  (intervalNs % 1000000000ULL);

    if (timer_settime(_timerid, 0, &ts, nullptr) != 0)
        perror("timer_settime");
}

void Timer::disarm()
{
    // Nullwerte deaktivieren den Timer ohne ihn zu löschen.
    itimerspec ts {};
    timer_settime(_timerid, 0, &ts, nullptr);
}

void Timer::cleanup()
{
    // Nur aufrufen wenn _state == RUNNING oder PAUSED.
    if (_state == TimerState::RUNNING)
        disarm();

    timer_delete(_timerid);
    _state = TimerState::IDLE;
}

// ─────────────────────────────────────────────
// TimerServer
// ─────────────────────────────────────────────

TimerServer::TimerServer()
{
    _chid = ChannelCreate(0);

    if (_chid == -1)
    {
        perror("ChannelCreate");
        return;
    }

    // _NTO_SIDE_CHANNEL: MsgSendPulse blockiert nicht wenn der Empfänger
    // gerade nicht wartet.
    _conid = ConnectAttach(0, 0, _chid, _NTO_SIDE_CHANNEL, 0);

    if (_conid == -1)
    {
        perror("ConnectAttach");
        return;
    }

    _thread = std::thread(&TimerServer::run, this);
}

TimerServer::~TimerServer()
{
    _running = false;

    // Sentinel-Pulse weckt den blockierten MsgReceive-Aufruf.
    MsgSendPulse(_conid, SIGEV_PULSE_PRIO_INHERIT, PULSE_SERVER_STOP, 0);

    if (_thread.joinable())
        _thread.join();

    ConnectDetach(_conid);
    ChannelDestroy(_chid);
}

void TimerServer::registerTimer(Timer& timer)
{
    std::lock_guard<std::mutex> lock(_timersMtx);
    _timers[timer.getId()] = &timer;
}

void TimerServer::unregisterTimer(int id)
{
    std::lock_guard<std::mutex> lock(_timersMtx);
    _timers.erase(id);
}

void TimerServer::run()
{
    while (_running)
    {
        struct _pulse pulse {};

        int rcvid = MsgReceive(_chid, &pulse, sizeof(pulse), nullptr);

        if (rcvid == -1)
        {
            perror("MsgReceive");
            continue;
        }

        // rcvid > 0 wäre eine normale Message – hier nicht erwartet, ignorieren.
        if (rcvid != 0)
            continue;

        int timerId = pulse.value.sival_int;

        switch (pulse.code)
        {
            case PULSE_TIMER_TICK:
                // TODO: Message an Dispatcher senden
                std::cout << "[TimerServer] Timer " << timerId << " TICK\n";
                break;

            case PULSE_TIMER_EXPIRED:
            {
                // Zustand auf EXPIRED setzen bevor wir den Log ausgeben.
                {
                    std::lock_guard<std::mutex> lock(_timersMtx);
                    auto it = _timers.find(timerId);
                    if (it != _timers.end())
                        it->second->onExpired();
                }
                // TODO: Message an Dispatcher senden
                std::cout << "[TimerServer] Timer " << timerId << " EXPIRED\n";
                break;
            }

            case PULSE_SERVER_STOP:
                return;
        }
    }
}
