#pragma once

#include <cstdint>
#include <mutex>
#include <time.h>

class Timer
{
public:
    Timer(int id, int dispatcherConid);
    ~Timer();

    void     startOnce    (uint64_t durationMs);
    void     startPeriodic(uint64_t intervalMs);
    void     pause ();
    void     resume();
    uint64_t stop  ();   // gibt verbleibende Zeit in ms zurück

private:
    void arm  (uint64_t initialNs, uint64_t intervalNs);
    void disarm();

    const int  _id;
    const int  _dispatcherConid;

    timer_t    _timerid     {};
    bool       _running     { false };
    bool       _paused      { false };
    bool       _periodic    { false };
    uint64_t   _remainingNs { 0 };
    uint64_t   _intervalNs  { 0 };

    std::mutex _mtx;
};
