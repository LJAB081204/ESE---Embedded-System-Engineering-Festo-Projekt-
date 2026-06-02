# Workpiece Sorting System — ESEP 2026 SoSe · Team 3-2

A real-time embedded control system for an automated workpiece sorting conveyor, developed as part of the **Embedded Systems Engineering Project (ESEP)** at HAW Hamburg.

The system runs on **QNX Neutrino RTOS** deployed on two networked **BeagleBone Black** units, each controlling one conveyor belt module. Workpieces are identified by type and height, sorted into a defined sequence, and rejected via pusher or diverter mechanisms if they do not match the expected order.

---

## System Overview

The sorting plant consists of two conveyor belt modules (FM1 and FM2) operating in tandem:

| Module | Role |
|--------|------|
| **FM1** | Workpiece intake, initial height measurement, first-pass sorting |
| **FM2** | Secondary verification, final sorting, sequential output |

Workpieces are classified into four types and must arrive at the end of FM2 in a fixed cyclic sequence:

```
IH → IM → IF → IHM → IH → ...
```

Any workpiece that does not match the expected next type — or whose height measurement on FM2 contradicts the FM1 result — is ejected via the reject mechanism (diverter or pusher, depending on the hardware variant installed).

---

## Architecture

The software is structured around the **State Pattern (GoF)** for hierarchical finite state machine (FSM) implementation, modelled in **Itemis Create** (YAKINDU statecharts).

```
┌──────────────────────────────────────────────────┐
│                   Application                    │
│  ┌─────────────┐          ┌─────────────────┐   │
│  │   Main FSM  │◄────────►│   Error FSM     │   │
│  └──────┬──────┘          └─────────────────┘   │
│         │                                        │
│  ┌──────▼──────┐                                 │
│  │  Dispatcher │  ← QNX pulse routing            │
│  └──────┬──────┘                                 │
│         │                                        │
│  ┌──────▼──────────────────────────────────┐    │
│  │            HAL (Hardware Abstraction)    │    │
│  │  ISensor (interrupt-driven)              │    │
│  │  IActor  (motor, lamps, pusher)          │    │
│  └──────────────────────────────────────────┘   │
└──────────────────────────────────────────────────┘
```

**Key design decisions:**

- Sensors are **interrupt-driven** (GPIO0 on BeagleBone Black) — no polling. Each interrupt fires a QNX pulse to the Dispatcher.
- Inter-module communication uses QNX pulses over the network. All pulse codes follow the naming convention `PULSE_<CATEGORY>_<NAME>_<LOCAL|REMOTE>`.
- The height sensor reads ADC values and classifies workpieces via a dedicated `HeightSensor` thread.
- Error signalling uses a four-state model: *pending unacknowledged → pending acknowledged → gone unacknowledged → OK*.

---

## Hardware

| Component | Details |
|-----------|---------|
| SBC | BeagleBone Black |
| RTOS | QNX Neutrino |
| Sensors | Photoelectric barriers, inductive sensor, limit switches |
| Height measurement | ADC via TSCADC |
| Actuators | DC motor (forward / slow / stop), 3-colour lamp, pusher solenoid |
| Reject mechanism | Diverter (Weiche) **or** pusher (Auswerfer) — software supports both variants |
| Control panel | Start / Stop / Reset buttons, E-Stop, LED indicators |

---

## Workpiece Types

| Code | Description |
|------|-------------|
| `IH` | Workpiece — high |
| `IM` | Workpiece — metallic |
| `IF` | Workpiece — flat |
| `IHM` | Workpiece — high & metallic |
| `IU` | Unknown / unclassifiable |

---

## Status Signalling

The 3-colour lamp reflects the current system state:

| Lamp | State | Meaning |
|------|-------|---------|
| 🟢 steady | Running | Normal operation |
| 🟢 blinking | Service mode | Calibration / self-test active |
| 🟡 blinking | Warning | Non-critical issue |
| 🔴 off | No error | System OK |
| 🔴 fast blink (1 Hz) | Error — pending unacknowledged | New unacknowledged fault |
| 🔴 steady | Error — pending acknowledged | Fault acknowledged, not yet resolved |
| 🔴 slow blink (0.5 Hz) | Error — gone unacknowledged | Fault self-resolved, not yet acknowledged |

---

---

## Project Structure

```
ESEP/
├── src/
│   ├── HAL/            # Hardware abstraction (ISensor, IActor implementations)
│   ├── FSM/            # State machine implementations
│   ├── Dispatcher/     # QNX pulse routing
│   ├── ADC_Sources/    # Height sensor & ADC driver
│   ├── IHAL.h          # Sensor & actor interfaces
│   ├── PulseMessages.h # Pulse code definitions
│   └── HeightSensor.h  # Height classification
├── Weiche1.ysc         # Itemis Create statechart — diverter module
└── NamingConventions.docx
```

---

## Team

| Name |
|------|
| Taylan Türkoglu |
| Lewis Ashton Braun |
| Mika Brüggemann |
| Oliver Vodegel |

---

## Course

**Embedded Systems Engineering Project (ESEP)**  
Summer Semester 2026 · HAW Hamburg  
