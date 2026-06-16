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

// Service Mode: Kalibriert den Sensor mit echten Messwerten
// Reihenfolge ist fest vorgegeben:
// 1. Laufband leer, 2. Hohes Werkstück, 3. Bohrung, 4. High(skip), 5. Laufband(skip), 6. Flat
void HeightSensor::calibrate() {
	this->calibrateRunning = true;
	//int tmpConveyor, tmpHigh, tmpDrilling,tmpFlat;
    struct _pulse pulse;
    int state = 1;

    // Schritt 1: Laufband direkt mitteln
    long sum = 0;
    for(int i = 0; i < 20; i++){
        adc.sample();
        MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
        sum += pulse.value.sival_int;
    }
    refConveyor = sum / 20;
    std::cout << "Kalibrierung: refConveyor=" << refConveyor << std::endl;

    while(state != 6 && calibrateRunning){

        if(state == 2){
            bool started = false;
            int window[5] = {};
            int idx = 0, stableCount = 0;
            while(stableCount < 3 && calibrateRunning){
                adc.sample();
                MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
                int val = pulse.value.sival_int;
                if(!started && abs(val - refHigh) >= ADC_THRESHOLD){
                    started = true;
                }
                if(started){
                    window[idx % 5] = val;
                    idx++;
                    if(idx >= 5){
                        int minVal = *std::min_element(window, window+5);
                        int maxVal = *std::max_element(window, window+5);
                        if(maxVal - minVal < ADC_THRESHOLD) stableCount++;
                        else stableCount = 0;
                    }
                }
            }
            if(!calibrateRunning){
                	return;
            }

            long drillingSum = 0;
            for(int i = 0; i < 5; i++) drillingSum += window[i];
            refDrilling = drillingSum / 5;
            std::cout << "Kalibrierung: refDrilling=" << refDrilling << std::endl;
            state = 3;
            continue;
        }

        int windowSize = 20;
        int measuredVal = 0;
        bool validHeight = false;
        while(!validHeight && calibrateRunning){
            int window[20] = {};
            int idx = 0, stableCount = 0;
            bool itemArrived = false;
            //int requiredStable = (state == 3 || state == 4) ? 1 : 5;
            while(stableCount < 5 && calibrateRunning){
                adc.sample();
               MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
                int val = pulse.value.sival_int;
                // Sofort Pulse wenn Werkstück erkannt
			    if(!itemArrived && abs(val - refConveyor) >= ADC_THRESHOLD){
			    	if(state ==1 || state ==5){		//von conveyor auf high/flat
				    MsgSendPulse(dispCoid, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_ARRIVED, 0);
				    itemArrived = true;
			    	}

			    }
			    else if(!itemArrived && abs(val - refConveyor) < ADC_THRESHOLD){
			    	if(state ==4 || state ==6){		//von flat/high auf conveyor
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

            long stableSum = 0;
            for(int i = 0; i < windowSize; i++) stableSum += window[i];
            measuredVal = stableSum / windowSize;
            if(state == 4){
                if(abs(measuredVal - refConveyor) < ADC_THRESHOLD) validHeight = true;
            } else {
                if(abs(measuredVal - refConveyor) >= ADC_THRESHOLD) validHeight = true;
            }
        }

        if(!calibrateRunning){
            	return;
        }

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

    /*refConveyor = tmpConveyor;
    refHigh = tmpHigh;
    refDrilling = tmpDrilling;
    refFlat = tmpFlat;*/

    saveCalibration();
    std::cout << "Kalibrierung: Abgeschlossen" << std::endl;
    //MsgSendPulse(dispCoid, SIGEV_PULSE_PRIO_INHERIT, PULSE_CALIBRATION_DONE, 0);
    return;

}

// Schreibt die Referenzwerte in die calibration.cfg
void HeightSensor::saveCalibration(){
    mkdir("/etc/heightsensor", 0777);
    std::ofstream file(CALIBRATION_FILE);
    if(file.is_open()){
        file << "calibrated=1\n";
        file << "refConveyor=" << refConveyor << "\n";
        file << "refHigh="     << refHigh     << "\n";
        file << "refDrilling=" << refDrilling << "\n";
        file << "refFlat="     << refFlat     << "\n";
        file.close();
        // Verifizieren
        std::ifstream verify(CALIBRATION_FILE);
        std::string line;
        while(std::getline(verify, line)){
            std::cout << "Gespeichert: '" << line << "'" << std::endl;
        }
    }
}

bool HeightSensor::loadCalibration(){
    std::ifstream file(CALIBRATION_FILE);
    if(!file.is_open()){
       // std::cerr << "Fehler: Keine Kalibrierungsdatei gefunden, Service Mode noetig" << std::endl;
        return false;
    }
    bool calibrated = false;
    std::string line;
    while(std::getline(file, line)){
       // std::cout << "Lese: '" << line << "'" << std::endl;
        if(!line.empty() && line.back() == '\r') line.pop_back();
        try {
            if(line.find("calibrated=")  == 0) calibrated  = std::stoi(line.substr(11));
            if(line.find("refConveyor=") == 0) refConveyor = std::stoi(line.substr(12));
            if(line.find("refHigh=")     == 0) refHigh     = std::stoi(line.substr(8));
            if(line.find("refDrilling=") == 0) refDrilling = std::stoi(line.substr(12));
            if(line.find("refFlat=") == 0) refFlat = std::stoi(line.substr(8));
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
    // Kalibrierungswerte laden, falls nicht vorhanden -> Service Mode nötig
   /*if(!loadCalibration){
	   std::cout << "Kalibrierung fehlt, bitte in den Service Mode!" << std::endl;
   }*/
   while(!loadCalibration()){
	   while(!doCalibrate){
		   std::this_thread::sleep_for(std::chrono::milliseconds(100));
	   }
	   doCalibrate = false;
	   calibrate();
    }
    struct _pulse pulse;
    while(running){
    	if(doCalibrate){
    		doCalibrate = false;
    		calibrate();
    	}
        adc.sample();                                    // ADC Wert holen
        MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
        detectHeight(pulse.value.sival_int);
    }
    adc.unregisterAdcISR();  // Abmelden der ISR
}

void HeightSensor::detectHeight(int value){
    if(value < refHigh - ADC_THRESHOLD){
        currentHeight = HEIGHT_TOO_HIGH;
    }
    else if(abs(value - refConveyor)  < ADC_THRESHOLD) currentHeight = HEIGHT_CONVEYOR;
    else if(abs(value - refHigh)      < ADC_THRESHOLD) currentHeight = HEIGHT_WORKPIECE_HIGH;
    else if(abs(value - refDrilling)  < ADC_THRESHOLD) currentHeight = HEIGHT_DRILLING;
    else if(abs(value - refFlat)      < ADC_THRESHOLD) currentHeight = HEIGHT_WORKPIECE_FLAT;
    else                                               currentHeight = HEIGHT_UNKNOWN;

    if(currentHeight == lastCandidate){
        heightCounter++;
    } else {
        lastCandidate = currentHeight;
        heightCounter = 0;
    }

    if(heightCounter > 10 && currentHeight != previousHeight){
        //std::cout << "Neue Hoehe: " << currentHeight << " rawValue=" << value << std::endl;
        if(previousHeight == HEIGHT_CONVEYOR && currentHeight != HEIGHT_CONVEYOR){
        	std::cout << "Item arrived" << std::endl;
            MsgSendPulse(dispCoid, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_ARRIVED, currentHeight);
        }
        if(previousHeight != HEIGHT_CONVEYOR && currentHeight == HEIGHT_CONVEYOR){
        	std::cout << "Item Gone" << std::endl;
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
        if(currentHeight != HEIGHT_UNKNOWN){
            previousHeight = currentHeight;
        }
        heightCounter = 0;
        lastCandidate = currentHeight;
    }
}

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

void HeightSensor::startCalibration(){
	this->doCalibrate = true;
}

void HeightSensor::stopCalibration(){
	if(calibrateRunning){
		calibrateRunning = false;
		doCalibrate=false;
	}

}

