/*
 * HeightSensor.cpp
 *
 *  Created on: 08.05.2026
 *      Author: lewis
 */

#include "HeightSensor.h"
#include "PulseMessages.h"

//Konstruktor
HeightSensor::HeightSensor(TSCADC& tscadc, int dispCoid)
    : tscadc(tscadc), adc(tscadc)
{
    this->refConveyor = 0;
    this->refHigh = 0;
    this->refDrilling = 0;
    this->refFlat = 0;
    this->previousHeight = HEIGHT_UNINITIALIZED;
    this->currentHeight = HEIGHT_CONVEYOR;
    this->lastCandidate = HEIGHT_UNINITIALIZED;
    this->heightCounter = 0;
    this->dispCoid = dispCoid;
    chid = ChannelCreate(0);
    coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);
    this->running = false;
    start();
}

//Destruktor
HeightSensor::~HeightSensor(){
    stop();
    ConnectDetach(coid);
    ChannelDestroy(chid);
}

// Service Mode: Kalibriert den Sensor mit echten Messwerten
// Reihenfolge ist fest vorgegeben:
// 1. Laufband leer, 2. Hohes Werkstück, 3. Bohrung, 4. High(skip), 5. Laufband(skip), 6. Flat
void HeightSensor::calibrate() {
    struct _pulse pulse;
    int state = 1;

    // Schritt 1: Laufband direkt mitteln, kein Übergang zu erwarten
    long sum = 0;
    for(int i = 0; i < 20; i++){
        adc.sample();
        MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
        sum += pulse.value.sival_int;
    }
    refConveyor = sum / 20;

    // Schritt 2: Restliche Höhen per Window
    // Solange nicht alle Höhen geloggt (state 6 = fertig)
    while(state != 6){

        // Window befüllen und auf Stabilität warten
        // Übergangswerte (Wand etc.) werden so automatisch ignoriert
        int window[20] = {};
        int idx = 0, stableCount = 0;
        while(stableCount < 5){
            adc.sample();
            MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
            window[idx % 20] = pulse.value.sival_int;  // Ringpuffer
            idx++;
            if(idx >= 20){  // Erst auswerten wenn Window voll
                int minVal = *std::min_element(window, window+20);
                int maxVal = *std::max_element(window, window+20);
                if(maxVal - minVal < ADC_THRESHOLD) stableCount++;  // Stabil -> hochzählen
                else stableCount = 0;                                // Instabil -> reset
            }
        }

        // Mittelwert über die 20 stabilen Window-Werte
        long stableSum = 0;
        for(int i = 0; i < 20; i++) stableSum += window[i];
        int measuredVal = stableSum / 20;

        // Reihenfolge ist im Service Mode fest vorgegeben
        switch(state){
            case 1: refHigh     = measuredVal; state = 2; break; // Hohes Werkstück -> loggen
            case 2: refDrilling = measuredVal; state = 3; break; // Bohrung -> loggen
            case 3: /* High nochmal -> bereits geloggt, ignorieren */ state = 4; break;
            case 4: /* Laufband -> bereits geloggt, ignorieren */     state = 5; break;
            case 5: refFlat     = measuredVal; state = 6; break; // Flaches Werkstück -> loggen, fertig
        }
    }

    // Kalibrierungswerte in Datei speichern
    saveCalibration();
}

// Schreibt die Referenzwerte in die calibration.cfg
void HeightSensor::saveCalibration(){
    std::ofstream file(CALIBRATION_FILE);
    if(file.is_open()){
        file << "refConveyor=" << refConveyor << "\n";
        file << "refHigh="     << refHigh     << "\n";
        file << "refDrilling=" << refDrilling << "\n";
        file << "refFlat="     << refFlat     << "\n";
        file.close();
    } else {
        std::cerr << "Fehler: Kalibrierungsdatei konnte nicht geschrieben werden" << std::endl;
    }
}

// Liest die Referenzwerte aus der calibration.cfg
// Gibt false zurück wenn Datei nicht existiert -> Service Mode nötig
bool HeightSensor::loadCalibration(){
    std::ifstream file(CALIBRATION_FILE);
    if(!file.is_open()){
        std::cerr << "Fehler: Keine Kalibrierungsdatei gefunden, Service Mode noetig" << std::endl;
        return false;
    }
    std::string line;
    while(std::getline(file, line)){
        if(line.find("refConveyor=") == 0) refConveyor = std::stoi(line.substr(12));
        if(line.find("refHigh=")     == 0) refHigh     = std::stoi(line.substr(8));
        if(line.find("refDrilling=") == 0) refDrilling = std::stoi(line.substr(12));
        if(line.find("refFlat=")     == 0) refFlat     = std::stoi(line.substr(7));
    }
    file.close();
    return true;
}

//Start des HS-Threads
void HeightSensor::start(){
    running = true;
    threadHS = std::thread(&HeightSensor::threadFunc, this);
}

//Stop des HS-Threads
void HeightSensor::stop(){
    running = false;
    if(threadHS.joinable()) {
        threadHS.join();
    }
}

//HS Thread Funktion
void HeightSensor::threadFunc(){
    adc.registerAdcISR(coid, ADC_TYPE);  // Registrieren der ISR
    // Kalibrierungswerte laden, falls nicht vorhanden -> Service Mode nötig
    if(!loadCalibration()){
        running = false;
        adc.unregisterAdcISR();
        return;
    }
    struct _pulse pulse;
    while(running){
        adc.sample();                                    // ADC Wert holen
        MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
        detectHeight(pulse.value.sival_int);
    }
    adc.unregisterAdcISR();  // Abmelden der ISR
}

void HeightSensor::detectHeight(int value){
    // Direkt gegen kalibrierte Referenzwerte vergleichen
    // TOO_HIGH: Wert kleiner als refHigh - Threshold (kein Referenzwert vorhanden)
    if(value < refHigh - ADC_THRESHOLD){
        currentHeight = HEIGHT_TOO_HIGH;
    }
    else if(abs(value - refConveyor)  < ADC_THRESHOLD) currentHeight = HEIGHT_CONVEYOR;
    else if(abs(value - refHigh)      < ADC_THRESHOLD) currentHeight = HEIGHT_WORKPIECE_HIGH;
    else if(abs(value - refDrilling)  < ADC_THRESHOLD) currentHeight = HEIGHT_DRILLING;
    else if(abs(value - refFlat)      < ADC_THRESHOLD) currentHeight = HEIGHT_WORKPIECE_FLAT;
    else                                               currentHeight = HEIGHT_UNKNOWN;

    // Puffer: gleiche Höhe wie vorheriger Wert -> hochzählen, sonst reset
    if(currentHeight == lastCandidate){
        heightCounter++;
    } else {
        lastCandidate = currentHeight;
        heightCounter = 0;
    }

    // Erst nach 20 stabilen Werten neue Höhe melden
    if(heightCounter > 15 && currentHeight != previousHeight){
        // Werkstück aufgetaucht
        if(previousHeight == HEIGHT_CONVEYOR && currentHeight != HEIGHT_CONVEYOR){
            MsgSendPulse(dispCoid, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_ARRIVED, currentHeight);
        }
        // Werkstück weg
        if(previousHeight != HEIGHT_CONVEYOR && currentHeight == HEIGHT_CONVEYOR){
            MsgSendPulse(dispCoid, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_GONE, 0);
        }
        switch(currentHeight) {
            case HEIGHT_CONVEYOR:       MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT_LOCAL,HEIGHT_CONVEYOR);      break;
            case HEIGHT_DRILLING:       MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT_LOCAL,HEIGHT_DRILLING);      break;
            case HEIGHT_WORKPIECE_HIGH: MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT_LOCAL,HEIGHT_WORKPIECE_HIGH);break;
            case HEIGHT_WORKPIECE_FLAT: MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT_LOCAL,HEIGHT_WORKPIECE_FLAT);break;
            case HEIGHT_TOO_HIGH:       MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT_LOCAL,HEIGHT_TOO_HIGH);      break;
            case HEIGHT_UNKNOWN:        MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT_LOCAL,HEIGHT_UNKNOWN);       break;
            default: break;
        }
        // Vorherige Höhe setzen, wenn nicht unbekannt
        if(currentHeight != HEIGHT_UNKNOWN){
            previousHeight = currentHeight;
        }
        // Puffer reset
        heightCounter = 0;
        lastCandidate = currentHeight;
    }
}

int HeightSensor::getCoid(){
    return coid;
}

int HeightSensor::getChid(){
    return chid;
}
