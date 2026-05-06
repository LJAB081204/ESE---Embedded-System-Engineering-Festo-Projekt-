/*
 * Testing.cpp
 *
 *  Created on: 22.04.2026
 *      Author: tayla
 */

#include "testing.h"
#include <unistd.h>
#include <iostream>

testing::testing(){
}

void testing::testMotor(){
    HAL hal;

    std::cout << "Testing Motor..." << std::endl;

    hal.getActors()->motorForwards();
    usleep(1000*1000);

    hal.getActors()->motorSlow();
    usleep(1000*1000);

    hal.getActors()->motorStop();
    usleep(1000*1000);

    hal.getActors()->motorBackwards();
    usleep(1000*1000);

    hal.getActors()->motorStop();

    std::cout << "Motor test completed!" << std::endl;
}

void testing::testLamps(){
    HAL hal;

    std::cout << "Testing Lamps..." << std::endl;

    hal.getActors()->redLampOn();
    hal.getActors()->yellowLampOn();
    hal.getActors()->greenLampOn();
    usleep(1000*1000);

    hal.getActors()->redLampOff();
    hal.getActors()->yellowLampOff();
    hal.getActors()->greenLampOff();

    std::cout << "Lamps test completed!" << std::endl;
}

void testing::testSensors(){
    HAL hal;

    std::cout << "Testing Sensors..." << std::endl;

    std::cout << "Item at entry: " << hal.getSensors()->isItemAtEntry() << std::endl;
    usleep(1000*1000);

    std::cout << "Item in height measurement: " << hal.getSensors()->isItemInHeightMeas() << std::endl;
    usleep(1000*1000);

    std::cout << "Item in pusher: " << hal.getSensors()->isItemInPusher() << std::endl;
    usleep(1000*1000);

    std::cout << "Item height OK: " << hal.getSensors()->isItemHeightOk() << std::endl;
    usleep(1000*1000);

    std::cout << "Item is metal: " << hal.getSensors()->isItemMetal() << std::endl;
    usleep(1000*1000);

    std::cout << "Pusher open: " << hal.getSensors()->isPusherOpen() << std::endl;
    usleep(1000*1000);

    std::cout << "Item at exit: " << hal.getSensors()->isItemAtExit() << std::endl;
    usleep(1000*1000);

    std::cout << "Slide full: " << hal.getSensors()->isSlideFull() << std::endl;

    std::cout << "Sensors test completed!" << std::endl;
}

void testing::testButtons(){
    HAL hal;

    std::cout << "Testing Buttons..." << std::endl;
    std::cout << "Press Reset Button..." << std::endl;

    while(!hal.getSensors()->isButtonResetPressed()){
    }
    std::cout << "Reset Button pressed!" << std::endl;
    usleep(500*1000);

    std::cout << "Press Start Button..." << std::endl;
    while(!hal.getSensors()->isButtonStartPressed()){
    }
    std::cout << "Start Button pressed!" << std::endl;
    usleep(500*1000);

    std::cout << "Press Stop Button..." << std::endl;
    while(!hal.getSensors()->isButtonStopPressed()){
    }
    std::cout << "Stop Button pressed!" << std::endl;
    usleep(500*1000);

    std::cout << "Trigger E-Stop..." << std::endl;
    while(!hal.getSensors()->isEStopTriggered()){
    }
    std::cout << "E-Stop triggered!" << std::endl;

    std::cout << "Buttons test completed!" << std::endl;
}
