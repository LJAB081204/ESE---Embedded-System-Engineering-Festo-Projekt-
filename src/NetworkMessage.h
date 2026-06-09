/*
 * NetworkMessage.h
 *
 * Gemeinsame Message-Struct fuer Anlage 1 <-> Anlage 2.
 * Diese Datei muss auf BEIDEN BBBs im Include-Pfad liegen
 * (einfach in beide Projekte kopieren).
 *
 * QNX MsgSend/MsgReceive uebertraegt rohe Bytes - beide Seiten
 * muessen exakt dieselbe Struct verwenden (kein virtual, kein
 * std::string, keine Pointer - nur Plain-Old-Data).
 */

#ifndef NETWORKMESSAGE_H_
#define NETWORKMESSAGE_H_

#include <stdint.h>

// ─── Message-Typen ────────────────────────────────────────────────────────────
// Anlage 1 sendet einen dieser Typen wenn ein IRQ ausgeloest wird.
// Anlage 2 wertet msg.type im switch-case aus.

#define NET_MSG_LICHTSCHRANKE_1   0x01   // Lichtschranke 1 blockiert/frei
#define NET_MSG_LICHTSCHRANKE_2   0x02   // Lichtschranke 2 blockiert/frei
#define NET_MSG_ESTOP             0x10   // E-Stop ausgeloest
#define NET_MSG_RESET             0x11   // Reset-Taster gedrueckt
#define NET_MSG_START             0x12   // Start-Taster gedrueckt

// ─── State-Werte (in msg.state) ───────────────────────────────────────────────
#define NET_STATE_ACTIVE          1      // Sensor blockiert / Taster gedrueckt
#define NET_STATE_CLEAR           0      // Sensor frei    / Taster losgelassen

// ─── Message-Struct ───────────────────────────────────────────────────────────
struct NetworkMessage {
    uint16_t type;    // NET_MSG_* - welcher Sensor/Aktor hat ausgeloest
    uint16_t state;   // NET_STATE_ACTIVE oder NET_STATE_CLEAR
    uint32_t data;    // optional: Rohdaten / Zeitstempel / Sequenznummer
};

#endif /* NETWORKMESSAGE_H_ */
