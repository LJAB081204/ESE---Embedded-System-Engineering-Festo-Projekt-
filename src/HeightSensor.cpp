/*
 * HeightSensor.cpp
 *
 *  Created on: 08.05.2026
 *      Author: lewis
 */

#include "HeightSensor.h"
#include "PulseMessages.h"
#include <sys/stat.h>

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
    this->doCalibrate = false;
    chid = ChannelCreate(0);
    coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);
    this->running = false;
    this->calibrateRunning = false;
    start();
}

//Destruktor
HeightSensor::~HeightSensor(){
    stop();
    ConnectDetach(coid);
    ChannelDestroy(chid);
}

// Service Mode: Kalibriert den Sensor mit echten Messwerten fuer jede Anlage individuell
// Reihenfolge ist im Service Mode fest vorgegeben und muss eingehalten werden:
// 1. Laufband leer, 2. Hohes Werkstueck (High), 3. Bohrung im selben Werkstueck (Drilling),
// 4. High nochmal (wird ignoriert), 5. Laufband nochmal (wird ignoriert), 6. Flaches Werkstueck (Flat)
void HeightSensor::calibrate() {
    this->calibrateRunning = true; // Flag damit calibrate() von aussen (z.B. E-Stop) abgebrochen werden kann
    struct _pulse pulse;
    int state = 1; // Steuert welche Hoehe als naechstes erwartet wird

    // ---- Schritt 1: Laufband direkt mitteln ----
    // Hier wird kein Uebergang erwartet, daher reicht ein einfacher Mittelwert ueber 20 Samples
    long sum = 0;
    for(int i = 0; i < 20; i++){
        adc.sample();
        MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
        sum += pulse.value.sival_int;
    }
    refConveyor = sum / 20;
    std::cout << "Kalibrierung: refConveyor=" << refConveyor << std::endl;

    // ---- Schritt 2: Restliche Hoehen nacheinander einlesen ----
    // Schleife laeuft bis alle Hoehen geloggt sind (state == 6) oder abgebrochen wird
    while(state != 6 && calibrateRunning){

        // Sonderfall State 2 (Bohrung): die Bohrung ist zu kurz fuer ein stabiles Window,
        // daher wird hier mit einem kleineren Window (5 statt 20 Werte) gearbeitet
        if(state == 2){
            bool started = false; // wird true sobald der Wert sich von refHigh entfernt (Bohrung beginnt)
            int window[5] = {};
            int idx = 0, stableCount = 0;

            while(stableCount < 3 && calibrateRunning){
                adc.sample();
                MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
                int val = pulse.value.sival_int;

                // Erkennt den Beginn der Bohrung: Wert weicht stark von refHigh ab
                if(!started && abs(val - refHigh) >= ADC_THRESHOLD){
                    started = true;
                }

                // Erst ab hier werden Werte ins Window aufgenommen (Uebergang wird ignoriert)
                if(started){
                    window[idx % 5] = val;
                    idx++;
                    if(idx >= 5){
                        int minVal = *std::min_element(window, window+5);
                        int maxVal = *std::max_element(window, window+5);
                        // Stabil, wenn die Werte im Window nah beieinander liegen
                        if(maxVal - minVal < ADC_THRESHOLD) stableCount++;
                        else stableCount = 0; // Schwankung -> Counter zuruecksetzen
                    }
                }
            }

            // Abbruch durch E-Stop o.ae. -> Kalibrierung sofort verlassen, nichts speichern
            if(!calibrateRunning){
                return;
            }

            // Mittelwert der stabilen Bohrungswerte
            long drillingSum = 0;
            for(int i = 0; i < 5; i++) drillingSum += window[i];
            refDrilling = drillingSum / 5;
            std::cout << "Kalibrierung: refDrilling=" << refDrilling << std::endl;
            state = 3;
            continue; // zurueck zum Schleifenkopf, restlicher State-2-Code wird uebersprungen
        }

        // ---- Normales Window fuer alle anderen States (1, 3, 4, 5) ----
        int windowSize = 20;
        int measuredVal = 0;
        bool validHeight = false;

        while(!validHeight && calibrateRunning){
            int window[20] = {};
            int idx = 0, stableCount = 0;
            bool itemArrived = false; // verhindert Mehrfach-Pulse innerhalb eines Durchlaufs

            while(stableCount < 5 && calibrateRunning){
                adc.sample();
                MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
                int val = pulse.value.sival_int;

                // Sofort-Pulse sobald sich der Wert vom Laufband-Referenzwert unterscheidet.
                // Wichtig fuer die Anlagensteuerung (z.B. Band verlangsamen), noch bevor
                // die Hoehe ueber das Window stabil bestimmt wurde.
                if(!itemArrived && abs(val - refConveyor) >= ADC_THRESHOLD){
                    if(state == 1 || state == 5){ // Uebergang Conveyor -> High/Flat
                        MsgSendPulse(dispCoid, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_ARRIVED, 0);
                        itemArrived = true;
                    }
                }
                // Sofort-Pulse beim umgekehrten Fall: Werkstueck verlaesst den Sensor wieder
                else if(!itemArrived && abs(val - refConveyor) < ADC_THRESHOLD){
                    if(state == 4 || state == 6){ // Uebergang High/Flat -> Conveyor
                        MsgSendPulse(dispCoid, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_GONE, 0);
                        itemArrived = true;
                    }
                }

                window[idx % windowSize] = pulse.value.sival_int;
                idx++;
                if(idx >= windowSize){
                    int minVal = *std::min_element(window, window+windowSize);
                    int maxVal = *std::max_element(window, window+windowSize);
                    if(maxVal - minVal < ADC_THRESHOLD) stableCount++;
                    else stableCount = 0;
                }
            }

            if(!calibrateRunning){
                return;
            }

            // Mittelwert der stabilen Werte
            long stableSum = 0;
            for(int i = 0; i < windowSize; i++) stableSum += window[i];
            measuredVal = stableSum / windowSize;

            // State 4 (Laufband, wird ignoriert) wartet auf Rueckkehr zum Conveyor-Wert,
            // alle anderen States warten auf einen Wert der sich vom Conveyor unterscheidet
            if(state == 4){
                if(abs(measuredVal - refConveyor) < ADC_THRESHOLD) validHeight = true;
            } else {
                if(abs(measuredVal - refConveyor) >= ADC_THRESHOLD) validHeight = true;
            }
        }

        if(!calibrateRunning){
            return;
        }

        // Reihenfolge der Hoehen ist im Service Mode fest vorgegeben, daher kein Vergleich
        // notwendig - es wird einfach anhand des aktuellen States entschieden was geloggt wird
        switch(state){
            case 1: refHigh = measuredVal; std::cout << "Kalibrierung: refHigh=" << refHigh << std::endl; state = 2; break;
            case 3: state = 4; break;
            case 4: state = 5; break;
            case 5: refFlat = measuredVal; std::cout << "Kalibrierung: refFlat=" << refFlat << std::endl; state = 6; break;
            case 6: state = 7; break;
        }
    }

    if(!calibrateRunning){
        return;
    }

    // Alle 4 Referenzwerte erfolgreich ermittelt -> dauerhaft speichern
    saveCalibration();
    std::cout << "Kalibrierung: Abgeschlossen" << std::endl;
    return;
}

// Schreibt die ermittelten Referenzwerte dauerhaft in die calibration.cfg,
// damit nach einem Neustart nicht erneut kalibriert werden muss
void HeightSensor::saveCalibration(){
    mkdir("/etc/heightsensor", 0777); // legt den Ordner an falls er noch nicht existiert
    std::ofstream file(CALIBRATION_FILE);
    if(file.is_open()){
        file << "calibrated=1\n";
        file << "refConveyor=" << refConveyor << "\n";
        file << "refHigh="     << refHigh     << "\n";
        file << "refDrilling=" << refDrilling << "\n";
        file << "refFlat="     << refFlat     << "\n";
        file.close();

        // Kontrollausgabe: liest die Datei direkt wieder ein und zeigt sie an
        std::ifstream verify(CALIBRATION_FILE);
        std::string line;
        while(std::getline(verify, line)){
            std::cout << "Gespeichert: '" << line << "'" << std::endl;
        }
    }
}

// Liest die Referenzwerte aus der calibration.cfg.
// Gibt false zurueck wenn die Datei fehlt oder calibrated != 1 ist -> Service Mode noetig
bool HeightSensor::loadCalibration(){
    std::ifstream file(CALIBRATION_FILE);
    if(!file.is_open()){
        return false;
    }
    bool calibrated = false;
    std::string line;
    while(std::getline(file, line)){
        // Windows-Zeilenende entfernen falls vorhanden
        if(!line.empty() && line.back() == '\r') line.pop_back();
        try {
            if(line.find("calibrated=")  == 0) calibrated  = std::stoi(line.substr(11));
            if(line.find("refConveyor=") == 0) refConveyor = std::stoi(line.substr(12));
            if(line.find("refHigh=")     == 0) refHigh     = std::stoi(line.substr(8));
            if(line.find("refDrilling=") == 0) refDrilling = std::stoi(line.substr(12));
            if(line.find("refFlat=")     == 0) refFlat     = std::stoi(line.substr(8));
        } catch(std::exception& e){
            std::cerr << "Fehler beim Parsen: '" << line << "' -> " << e.what() << std::endl;
        }
    }
    file.close();
    if(calibrated != 1){
        std::cerr << "Fehler: Noch nicht kalibriert, Service Mode noetig" << std::endl;
        return false;
    }
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

    // Solange keine gueltige Kalibrierung vorliegt, wird auf den Service Mode gewartet.
    // Erst wenn dort der Knopf gedrueckt wird (doCalibrate = true), wird kalibriert.
    while(!loadCalibration()){
        while(!doCalibrate){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        doCalibrate = false;
        calibrate();
    }

    // Normaler Betrieb
    struct _pulse pulse;
    while(running){
        // Im laufenden Betrieb kann ueber den Service Mode jederzeit neu kalibriert werden
        if(doCalibrate){
            doCalibrate = false;
            calibrate();
        }
        adc.sample();
        MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
        detectHeight(pulse.value.sival_int);
    }
    adc.unregisterAdcISR();
}

void HeightSensor::detectHeight(int value){
    // Direkt gegen die kalibrierten Referenzwerte vergleichen
    if(value < refHigh - ADC_THRESHOLD){
        currentHeight = HEIGHT_TOO_HIGH;
    }
    else if(abs(value - refConveyor)  < ADC_THRESHOLD) currentHeight = HEIGHT_CONVEYOR;
    else if(abs(value - refHigh)      < ADC_THRESHOLD) currentHeight = HEIGHT_WORKPIECE_HIGH;
    else if(abs(value - refDrilling)  < ADC_THRESHOLD) currentHeight = HEIGHT_DRILLING;
    else if(abs(value - refFlat)      < ADC_THRESHOLD) currentHeight = HEIGHT_WORKPIECE_FLAT;
    else                                               currentHeight = HEIGHT_UNKNOWN;

    // Puffer: gleiche Hoehe wie der vorherige Kandidat -> hochzaehlen, sonst zuruecksetzen
    if(currentHeight == lastCandidate){
        heightCounter++;
    } else {
        lastCandidate = currentHeight;
        heightCounter = 0;
    }


    // Frueher Pulse: schon nach 3 stabilen Werten, basierend auf demselben Counter
       if(heightCounter == 3){
           if(previousHeight == HEIGHT_CONVEYOR && currentHeight != HEIGHT_CONVEYOR){
               std::cout << "Item arrived" << std::endl;
               MsgSendPulse(dispCoid, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_ARRIVED, currentHeight);
           }
           if(previousHeight != HEIGHT_CONVEYOR && currentHeight == HEIGHT_CONVEYOR){
               std::cout << "Item Gone" << std::endl;
               MsgSendPulse(dispCoid, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_GONE, 0);
           }
       }

    // Erst nach mehreren aufeinanderfolgenden gleichen Werten gilt die Hoehe als sicher erkannt,
    // damit kurzes Schwanken (z.B. an Kanten) keine falsche Hoehenaenderung meldet
    if(heightCounter > 10 && currentHeight != previousHeight){
        // Werkstueck ist aufgetaucht (Wechsel weg vom Laufband)

        // Aktuelle Hoehe an den Dispatcher melden
        switch(currentHeight) {
            case HEIGHT_CONVEYOR:       MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT_LOCAL,HEIGHT_CONVEYOR);      break;
            case HEIGHT_DRILLING:       MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT_LOCAL,HEIGHT_DRILLING);      break;
            case HEIGHT_WORKPIECE_HIGH: MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT_LOCAL,HEIGHT_WORKPIECE_HIGH);break;
            case HEIGHT_WORKPIECE_FLAT: MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT_LOCAL,HEIGHT_WORKPIECE_FLAT);break;
            case HEIGHT_TOO_HIGH:       MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT_LOCAL,HEIGHT_TOO_HIGH);      break;
            case HEIGHT_UNKNOWN:        MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT_LOCAL,HEIGHT_UNKNOWN);       break;
            default: break;
        }

        // previousHeight nur aktualisieren wenn die neue Hoehe nicht UNKNOWN ist,
        // damit kurze Stoerwerte nicht als "letzte gueltige Hoehe" uebernommen werden
        if(currentHeight != HEIGHT_UNKNOWN){
            previousHeight = currentHeight;
        }

        // Puffer fuer die naechste Erkennung zuruecksetzen
        heightCounter = 0;
        lastCandidate = currentHeight;
    }
}

// Setzt die Kalibrierungsdatei zurueck (calibrated=0, alle Refs=0).
// Wird benutzt um im Service Mode eine erzwungene Neukalibrierung zu signalisieren
void HeightSensor::resetCalibration(){
    std::ofstream file(CALIBRATION_FILE);
    if(file.is_open()){
        file << "calibrated=0\n";
        file << "refConveyor=0\n";
        file << "refHigh=0\n";
        file << "refDrilling=0\n";
        file << "refFlat=0\n";
        file.close();
    }
}

int HeightSensor::getCoid(){
    return coid;
}

int HeightSensor::getChid(){
    return chid;
}

void HeightSensor::setCoId(int CoId){
    this->dispCoid = CoId;
}

// Wird vom Service Mode aufgerufen wenn der Kalibrier-Knopf gedrueckt wird
void HeightSensor::setStartCalibration(){
    this->doCalibrate = true;
}

// Bricht eine laufende Kalibrierung ab (z.B. bei E-Stop oder Verlassen des Service-Mode-Zustands).
// doCalibrate wird NICHT gesetzt, damit calibrate() erst beim naechsten manuellen Knopfdruck
// im Service Mode wieder aufgerufen wird
void HeightSensor::stopCalibration(){
    if(calibrateRunning){
        calibrateRunning = false;
        doCalibrate = false;
    }
}
