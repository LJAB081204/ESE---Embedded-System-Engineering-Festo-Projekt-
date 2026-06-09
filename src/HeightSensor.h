#ifndef SRC_ADC_SOURCES_HEIGHTSENSOR_H_
#define SRC_ADC_SOURCES_HEIGHTSENSOR_H_

#define ADC_THRESHOLD 50
#define CALIBRATION_FILE "calibration.cfg" // Pfad anpassen

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
#include <fstream>

class HeightSensor{
public:
    HeightSensor(TSCADC& tscadc, int dispCoid);
    ~HeightSensor();
    void start();
    void stop();
    void calibrate();    // jetzt public für Service Mode
    int getCoid();
    int getChid();
private:
    // Referenzwerte aus Kalibrierung
    int refConveyor;
    int refHigh;
    int refDrilling;
    int refFlat;

    TSCADC& tscadc;
    ADC adc;
    Height previousHeight;
    Height currentHeight;
    Height lastCandidate;
    int heightCounter;
    int dispCoid;
    int chid;
    int coid;
    std::thread threadHS;
    bool running;
    void threadFunc();
    void detectHeight(int value);
    void saveCalibration();   // Refs in Datei schreiben
    bool loadCalibration();   // Refs aus Datei lesen, false wenn Datei nicht existiert
};
#endif
