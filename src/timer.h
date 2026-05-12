#pragma once

#include <cstdint>
#include <mutex>
#include <time.h>
#include <signal.h>
#include <sys/neutrino.h>

#include "PulseMessages.h"

// ─────────────────────────────────────────────
// TimerType
// ─────────────────────────────────────────────

enum class TimerType
{
    OneShot,  // Läuft einmalig ab
    Periodic  // Wiederholt sich bis zum expliziten stop()
};

// ─────────────────────────────────────────────
// TimerState
//
// Erlaubte Übergänge:
//   IDLE    → startOnce / startPeriodic → RUNNING
//   RUNNING → pause()                  → PAUSED
//   RUNNING → stop()                   → IDLE
//   RUNNING → abgelaufen (OneShot)     → EXPIRED  (automatisch)
//   EXPIRED → startOnce / startPeriodic→ RUNNING
//   PAUSED  → resume()                 → RUNNING
//   PAUSED  → stop()                   → IDLE
// ─────────────────────────────────────────────

enum class TimerState
{
    IDLE,
    RUNNING,
    PAUSED,
    EXPIRED
};

// ─────────────────────────────────────────────
// Timer
//
// Kapselt einen POSIX-Timer. Ereignisse werden als Pulses
// direkt an den Dispatcher gesendet (über dispatcherConid).
//
// Thread-Sicherheit:
//   Alle öffentlichen Methoden sind durch _mtx geschützt.
// ─────────────────────────────────────────────

class Timer
{
public:
    // dispatcherConid: Connection-ID des Dispatchers (Dispatcher::getConnectionID())
    Timer(int id, int dispatcherConid);

    // Stoppt den Timer automatisch falls noch aktiv (RAII).
    ~Timer();

    // Startet den Timer einmalig. Erlaubt in: IDLE, EXPIRED.
    void startOnce(uint64_t durationMs);

    // Startet den Timer periodisch. Erlaubt in: IDLE, EXPIRED.
    void startPeriodic(uint64_t intervalMs);

    // Pausiert den laufenden Timer. Erlaubt in: RUNNING.
    void pause();

    // Setzt einen pausierten Timer fort. Erlaubt in: PAUSED.
    void resume();

    // Stoppt den Timer. Erlaubt in: RUNNING, PAUSED.
    void stop();

    int        getId()    const { return _id; }
    TimerState getState() const { return _state; }

private:
    // Legt den Kernel-Timer an. Der Kernel sendet Pulses direkt
    // an den Dispatcher-Channel.
    // Voraussetzung: _mtx wird vom Aufrufer gehalten.
    void createTimer(int pulseCode);

    // Setzt Ablaufzeit und Wiederholintervall (Nanosekunden).
    // Voraussetzung: _mtx wird vom Aufrufer gehalten.
    void arm(uint64_t initialNs, uint64_t intervalNs);

    // Deaktiviert den Timer ohne ihn zu löschen.
    // Voraussetzung: _mtx wird vom Aufrufer gehalten.
    void disarm();

    // Räumt den Kernel-Timer auf und setzt den Zustand auf IDLE.
    // Voraussetzung: _mtx wird vom Aufrufer gehalten.
    void cleanup();

private:
    int _id;
    int _dispatcherConid;

    timer_t    _timerid {};
    TimerState _state   { TimerState::IDLE };
    TimerType  _type    { TimerType::OneShot };

    uint64_t _remainingNs { 0 };  // Gespeicherte Restzeit bei Pause (Nanosekunden)
    uint64_t _intervalNs  { 0 };  // Wiederholintervall, nur bei Periodic (Nanosekunden)

    std::mutex _mtx;
};
