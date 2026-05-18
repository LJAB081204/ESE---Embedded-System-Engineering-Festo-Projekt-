#include "timer.h"

#include <iostream>
#include <sys/neutrino.h>

#include "PulseMessages.h"

Timer::Timer(int id, int dispatcherConid)
    : _id(id), _dispatcherConid(dispatcherConid)
{}

Timer::~Timer()
{
    std::lock_guard<std::mutex> lock(_mtx);
    if (_running || _paused)
        disarm();
}

// ─────────────────────────────────────────────

void Timer::startOnce(uint64_t durationMs)
{
    std::lock_guard<std::mutex> lock(_mtx);

    if (_running || _paused)
    {
        std::cerr << "[Timer " << _id << "] Fehler: erst stop() aufrufen\n";
        return;
    }

    sigevent sev {};
    SIGEV_PULSE_INIT(&sev, _dispatcherConid, SIGEV_PULSE_PRIO_INHERIT, PULSE_TIMER_EXPIRED, _id);
    timer_create(CLOCK_MONOTONIC, &sev, &_timerid);

    _periodic    = false;
    _intervalNs  = 0;
    _remainingNs = durationMs * 1000000ULL;

    arm(_remainingNs, 0);

    _running = true;
    _paused  = false;
    std::cout << "[Timer " << _id << "] gestartet (OneShot, " << durationMs << " ms)\n";
}

void Timer::startPeriodic(uint64_t intervalMs)
{
    std::lock_guard<std::mutex> lock(_mtx);

    if (_running || _paused)
    {
        std::cerr << "[Timer " << _id << "] Fehler: erst stop() aufrufen\n";
        return;
    }

    sigevent sev {};
    SIGEV_PULSE_INIT(&sev, _dispatcherConid, SIGEV_PULSE_PRIO_INHERIT, PULSE_TIMER_TICK, _id);
    timer_create(CLOCK_MONOTONIC, &sev, &_timerid);

    _periodic    = true;
    _intervalNs  = intervalMs * 1000000ULL;
    _remainingNs = _intervalNs;

    arm(_remainingNs, _intervalNs);

    _running = true;
    _paused  = false;
    std::cout << "[Timer " << _id << "] gestartet (Periodic, " << intervalMs << " ms)\n";
}

void Timer::pause()
{
    std::lock_guard<std::mutex> lock(_mtx);

    if (!_running)
    {
        std::cerr << "[Timer " << _id << "] Fehler: Timer läuft nicht (pause)\n";
        return;
    }

    itimerspec ts {};
    timer_gettime(_timerid, &ts);
    _remainingNs = static_cast<uint64_t>(ts.it_value.tv_sec)  * 1000000000ULL
                 + static_cast<uint64_t>(ts.it_value.tv_nsec);

    disarm();

    _running = false;
    _paused  = true;
    std::cout << "[Timer " << _id << "] pausiert\n";
}

void Timer::resume()
{
    std::lock_guard<std::mutex> lock(_mtx);

    if (!_paused)
    {
        std::cerr << "[Timer " << _id << "] Fehler: Timer nicht pausiert (resume)\n";
        return;
    }

    arm(_remainingNs, _intervalNs);   // _intervalNs == 0 bei OneShot

    _running = true;
    _paused  = false;
    std::cout << "[Timer " << _id << "] fortgesetzt\n";
}

uint64_t Timer::stop()
{
    std::lock_guard<std::mutex> lock(_mtx);

    if (!_running && !_paused)
    {
        std::cerr << "[Timer " << _id << "] Fehler: Timer nicht aktiv (stop)\n";
        return 0;
    }

    uint64_t remainingMs = 0;

    if (_running)
    {
        itimerspec ts {};
        timer_gettime(_timerid, &ts);
        uint64_t ns = static_cast<uint64_t>(ts.it_value.tv_sec)  * 1000000000ULL
                    + static_cast<uint64_t>(ts.it_value.tv_nsec);
        remainingMs = ns / 1000000ULL;
        disarm();
    }
    else // paused
    {
        remainingMs = _remainingNs / 1000000ULL;
    }

    timer_delete(_timerid);

    _running = false;
    _paused  = false;
    std::cout << "[Timer " << _id << "] gestoppt (" << remainingMs << " ms verbleibend)\n";
    return remainingMs;
}

// ─────────────────────────────────────────────

void Timer::arm(uint64_t initialNs, uint64_t intervalNs)
{
    itimerspec ts {};
    ts.it_value.tv_sec     = static_cast<time_t>(initialNs  / 1000000000ULL);
    ts.it_value.tv_nsec    = static_cast<long>  (initialNs  % 1000000000ULL);
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
