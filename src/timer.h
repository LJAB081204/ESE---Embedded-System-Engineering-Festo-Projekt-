#pragma once

#include <cstdint>
#include <mutex>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <time.h>
#include <signal.h>
#include <sys/neutrino.h>

// ─────────────────────────────────────────────
// Pulse-Codes
//
// Nur TICK und EXPIRED werden an den Server gesendet.
// PULSE_SERVER_STOP dient ausschließlich dem internen
// Beenden des Server-Threads.
// ─────────────────────────────────────────────

static const int PULSE_TIMER_TICK    = _PULSE_CODE_MINAVAIL + 1;
static const int PULSE_TIMER_EXPIRED = _PULSE_CODE_MINAVAIL + 2;
static const int PULSE_SERVER_STOP   = _PULSE_CODE_MINAVAIL + 3;

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

class TimerServer;

// ─────────────────────────────────────────────
// Timer
//
// Kapselt einen POSIX-Timer. Ereignisse werden als Pulses
// an den TimerServer gesendet. Status-Ausgaben (start, stop,
// pause, resume, Fehler) gehen auf die Konsole.
//
// Thread-Sicherheit:
//   Alle öffentlichen Methoden sind durch _mtx geschützt.
//   MsgSendPulse wird stets außerhalb des Locks aufgerufen.
// ─────────────────────────────────────────────

class Timer
{
public:
    Timer(int id, TimerServer& server);

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
    // Legt den Kernel-Timer an und registriert den Pulse-Code.
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

    // Wird vom TimerServer aufgerufen wenn ein OneShot abläuft.
    // Setzt den Zustand auf EXPIRED ohne Log-Ausgabe.
    // Voraussetzung: _mtx darf NICHT vom Aufrufer gehalten werden.
    void onExpired();

    // TimerServer darf onExpired() und interne Members aufrufen.
    friend class TimerServer;

private:
    int          _id;
    TimerServer& _server;

    timer_t    _timerid {};
    TimerState _state   { TimerState::IDLE };
    TimerType  _type    { TimerType::OneShot };

    uint64_t  _remainingNs { 0 };  // Gespeicherte Restzeit bei Pause (Nanosekunden)
    uint64_t  _intervalNs  { 0 };  // Wiederholintervall, nur bei Periodic (Nanosekunden)

    std::mutex _mtx;
};

// ─────────────────────────────────────────────
// TimerServer
//
// Empfängt Pulses von Timer-Objekten über einen QNX-Channel.
// Die Verarbeitung läuft in einem eigenen Thread.
//
// Lebensdauer: muss länger leben als alle zugehörigen Timer.
// ─────────────────────────────────────────────

class TimerServer
{
public:
    TimerServer();
    ~TimerServer();

    int getChid()  const { return _chid; }
    int getConid() const { return _conid; }

    // Timer für EXPIRED-Benachrichtigung registrieren.
    void registerTimer(Timer& timer);
    void unregisterTimer(int id);

private:
    void run();

private:
    int _chid  { -1 };
    int _conid { -1 };

    std::thread       _thread;
    std::atomic<bool> _running { true };

    std::unordered_map<int, Timer*> _timers;
    std::mutex                      _timersMtx;
};
