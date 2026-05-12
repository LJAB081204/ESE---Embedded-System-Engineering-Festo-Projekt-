#include "timer.h"

#include <iostream>

// ─────────────────────────────────────────────
// Timer
// ─────────────────────────────────────────────

Timer::Timer(int id, int dispatcherConid)
    : _id(id),
      _dispatcherConid(dispatcherConid)
{}

Timer::~Timer()
{
    std::lock_guard<std::mutex> lock(_mtx);

    if (_state == TimerState::RUNNING || _state == TimerState::PAUSED)
    {
        cleanup();
        std::cout << "[Timer " << _id << "] gestoppt (Destruktor)\n";
    }
}

void Timer::startOnce(uint64_t durationMs)
{
    {
        std::lock_guard<std::mutex> lock(_mtx);

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

// ─────────────────────────────────────────────
// Private Hilfsmethoden
// Alle folgenden Methoden setzen voraus, dass _mtx vom Aufrufer gehalten wird.
// ─────────────────────────────────────────────

void Timer::createTimer(int pulseCode)
{
    sigevent sev {};

    // Der Kernel sendet den Pulse direkt an den Dispatcher-Channel.
    // Der Pulse-Value enthält _id zur Identifikation im Dispatcher.
    SIGEV_PULSE_INIT(
        &sev,
        _dispatcherConid,
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
    itimerspec ts {};
    timer_settime(_timerid, 0, &ts, nullptr);
}

void Timer::cleanup()
{
    if (_state == TimerState::RUNNING)
        disarm();

    timer_delete(_timerid);
    _state = TimerState::IDLE;
}
