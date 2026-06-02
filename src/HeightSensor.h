/*
 * HeightSensor.h
 *
 *  Created on: 07.05.2026
 *      Author: lewis
 */

#ifndef SRC_ADC_SOURCES_HEIGHTSENSOR_H_
#define SRC_ADC_SOURCES_HEIGHTSENSOR_H_

#define ADC_CONVEYOR_REF        3641
#define ADC_DRILLING_DIFF       406
#define ADC_WORKPIECE_FLAT_DIFF 1130
#define ADC_WORKPIECE_HIGH_DIFF 1366
#define ADC_THRESHOLD           50

enum Height {
	HEIGHT_UNINITIALIZED,
    HEIGHT_UNKNOWN,
    HEIGHT_CONVEYOR,
    HEIGHT_DRILLING,
    HEIGHT_WORKPIECE_HIGH,
    HEIGHT_WORKPIECE_FLAT,
	HEIGHT_TOO_HIGH
};

#include "ADC_Sources/ADC.h"
#include <sys/neutrino.h>
#include <thread>
#include <cstdlib>
#include <iostream>

class HeightSensor{

public:
	HeightSensor(TSCADC& tscadc, int dispCoid);
	~HeightSensor();

	void start();	//startet den Thread
	void stop();	//stoppt den Thread
	int getCoid();
	int getChid();

private:
	int offset; // wird bei Kalibrierung gesetzt
	TSCADC& tscadc;   // mapped die Hardware
	ADC adc; // nutzt das Mapping
	Height previousHeight;
	Height currentHeight;
	Height lastCandidate;
	int heightCounter;
	int dispCoid; //Connection-ID des Dispatchers
	int chid;
	int coid;
	std::thread threadHS; // Thread für Höhensensor
	bool running;

	void threadFunc();
	void calibrate();
	void detectHeight(int value);

};

#endif /* SRC_ADC_SOURCES_HEIGHTSENSOR_H_ */
