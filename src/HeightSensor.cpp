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
	this->offset = 0;
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

//Kalibriert den Höhensensor -> Band 2 als Referenz für die Defines
void HeightSensor::calibrate() {
	struct _pulse pulse;
	for(int i = 0; i<20;i++){
    adc.sample();
    MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
    offset += pulse.value.sival_int - ADC_CONVEYOR_REF;	//Offset auf die defines gerechnet
	}
	offset = offset/20;

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
	adc.registerAdcISR(coid, ADC_TYPE);	//Registrieren der ISR
	calibrate();
	struct _pulse pulse;
	while(running){
		adc.sample();				//adc Wert holen
		MsgReceivePulse(chid,&pulse,sizeof(pulse),NULL); //"" erhalten
		detectHeight(pulse.value.sival_int);
	}
	 adc.unregisterAdcISR();	//Abmelden der ISR
}

void HeightSensor::detectHeight(int value){
	//std::cout << "Raw value: " << value << std::endl;
	int conveyor = ADC_CONVEYOR_REF + offset;
	int drilling = conveyor - ADC_DRILLING_DIFF;
	int flat     = conveyor - ADC_WORKPIECE_FLAT_DIFF;
	int high     = conveyor - ADC_WORKPIECE_HIGH_DIFF;
	int adcMax   = high - ADC_THRESHOLD;
	int conveyorMin = conveyor - 150;

	//wenn wert kleiner ist als kleinster adc wert für Hohen Baustein
	if(value < adcMax ){
	    currentHeight = HEIGHT_TOO_HIGH;
	}
	//Wenn wert- Define + offset kleiner als Threshold && größer als minimu  ADC Wert vom Laufband
	else if(abs(value - conveyor) < ADC_THRESHOLD && value > conveyorMin){
        currentHeight = HEIGHT_CONVEYOR;
    }
	//Wenn Wert - Define kleiner als Threshold
    else if(abs(value - drilling) < ADC_THRESHOLD){
        currentHeight = HEIGHT_DRILLING;
    }
	//Wenn Wert - Define kleiner als Threshold und größer als ADC Max
    else if(abs(value - high) < ADC_THRESHOLD && value > (adcMax + offset) ){
        currentHeight = HEIGHT_WORKPIECE_HIGH;
    }

	//Wenn Wert - Define kleiner als threshold
    else if(abs(value - flat) < ADC_THRESHOLD){
        currentHeight = HEIGHT_WORKPIECE_FLAT;
    }

    else{
        currentHeight = HEIGHT_UNKNOWN;
    }

	//if-Abfrage für gleiche Höhe von vorherigem und aktuellem wert erhöhen des puffers
    if(currentHeight == lastCandidate){
            heightCounter++;
        }
    //sonst reseten
    else {
            lastCandidate = currentHeight;
            heightCounter = 0;
        }

    //If Abfrage für puffer, damit bei kurzem schwanken nicht neue Höhe detektiert wird
    if(heightCounter > 70 && currentHeight != previousHeight){
        switch(currentHeight) {
            case HEIGHT_CONVEYOR:      MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT,HEIGHT_CONVEYOR);break;
            case HEIGHT_DRILLING:      MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT,HEIGHT_DRILLING);break;
            case HEIGHT_WORKPIECE_HIGH:MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT,HEIGHT_WORKPIECE_HIGH); break;
            case HEIGHT_WORKPIECE_FLAT:MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT,HEIGHT_WORKPIECE_FLAT); break;
            case HEIGHT_TOO_HIGH: 	   MsgSendPulse(dispCoid,SIGEV_PULSE_PRIO_INHERIT,PULSE_NEW_HEIGHT,HEIGHT_TOO_HIGH); break;
            default: break;
        }
        //setzen der vorherigen Höhe, wenn nicht unbekannt
        if(currentHeight != HEIGHT_UNKNOWN){
        previousHeight = currentHeight;
        }
        //reset des Puffers
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
