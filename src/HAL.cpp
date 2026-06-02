/*
 * HAL.cpp
 *
 *  Created on: 21.04.2026
 *      Author: tayla
 */
//  TODO Funktion für Start Kurz oder Lang
#include "HAL.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <stdexcept>
#include <sys/mman.h>
#include <hw/inout.h>

using namespace std;


	// Sensor Wrapper Implementation

	SensorWrapper::SensorWrapper(){
	    gpio_bank_0 = mmap_device_io(GPIO_REGISTER_LENGHT, (uint64_t) GPIO_PORT0);
	    if (gpio_bank_0 == MAP_DEVICE_FAILED) {
	        throw std::runtime_error("Failed to map GPIO0");
	    }
		adcBaseAddr = mmap_device_io(ADC_LENGTH, (uint64_t) ADC_BASE);
	    if (adcBaseAddr == MAP_DEVICE_FAILED) {
	        throw std::runtime_error("Failed to map ADC_BASE");
	    }
	}

	SensorWrapper::~SensorWrapper(){
        munmap_device_io(gpio_bank_0, GPIO_REGISTER_LENGHT);
        munmap_device_io(adcBaseAddr, ADC_LENGTH);

		}



    bool SensorWrapper::isItemAtEntry(){
    	return !(in32(gpio_bank_0 + READ_REGISTER) & ITEM_AT_ENTRY);
    }
    bool SensorWrapper::isItemInHeightMeas(){
    	return !(in32(gpio_bank_0 + READ_REGISTER) & ITEM_IN_HEIGHT_MEAS);
    }
    bool SensorWrapper::isItemHeightOk(){
    	return (in32(gpio_bank_0 + READ_REGISTER) & ITEM_HEIGHT_OK);
    }
    bool SensorWrapper::isItemInPusher(){
    	return !(in32(gpio_bank_0 + READ_REGISTER) & ITEM_IN_PUSHER);
    }
    bool SensorWrapper::isItemMetal(){
    	return (in32(gpio_bank_0 + READ_REGISTER) & ITEM_METAL);
    }
    bool SensorWrapper::isPusherOpen(){
    	return (in32(gpio_bank_0 + READ_REGISTER) & PUSHER_OPEN);
    }
    bool SensorWrapper::isSlideFull(){
    	return !(in32(gpio_bank_0 + READ_REGISTER) & SLIDE_FULL);
    }
    bool SensorWrapper::isItemAtExit(){
    	return !(in32(gpio_bank_0 + READ_REGISTER) & ITEM_AT_EXIT);
    }
    bool SensorWrapper::isButtonStartPressed(){
      	return (in32(gpio_bank_0 + READ_REGISTER) & BUTTON_START);
    }
    bool SensorWrapper::isButtonStopPressed(){
    	return !(in32(gpio_bank_0 + READ_REGISTER) & BUTTON_STOP);
    }
    bool SensorWrapper::isButtonResetPressed(){
    	return (in32(gpio_bank_0 + READ_REGISTER) & BUTTON_RESET);
    }
    bool SensorWrapper::isEStopTriggered(){
    	return !(in32(gpio_bank_0 + READ_REGISTER) & ESTOP);
    }

    // Actor Wrapper Implementation

    ActorWrapper::ActorWrapper(){
		gpio_bank_1 = mmap_device_io(GPIO_REGISTER_LENGHT, (uint64_t) GPIO_PORT1);
	    if (gpio_bank_1 == MAP_DEVICE_FAILED) {
	        throw std::runtime_error("Failed to map GPIO1");
	    }

		gpio_bank_2 = mmap_device_io(GPIO_REGISTER_LENGHT, (uint64_t) GPIO_PORT2);
	    if (gpio_bank_2 == MAP_DEVICE_FAILED) {
	        throw std::runtime_error("Failed to map GPIO2");
	    }
    	out32((uintptr_t) (gpio_bank_2 + OE_REGISTER),0);

    	blinkGreenLamp = false;
    	blinkYellowLamp = false;
    	blinkRedLamp = false;

    }

    ActorWrapper::~ActorWrapper(){
    	greenLampBlinkOff();
    	yellowLampBlinkOff();
    	redLampBlinkOff();
        munmap_device_io(gpio_bank_1, GPIO_REGISTER_LENGHT);
        munmap_device_io(gpio_bank_2, GPIO_REGISTER_LENGHT);

    }
    void ActorWrapper::motorForwards(){
    	out32((uintptr_t) (gpio_bank_1 + CLR_REGISTER),MOTOR_BACKWARDS);
    	out32((uintptr_t) (gpio_bank_1 + CLR_REGISTER),MOTOR_STOP);
    	out32((uintptr_t) (gpio_bank_1 + CLR_REGISTER),MOTOR_SLOW);
    	out32((uintptr_t) (gpio_bank_1 + SET_REGISTER), MOTOR_FORWARDS);
    }
    void ActorWrapper::motorBackwards(){
    	out32((uintptr_t) (gpio_bank_1 + CLR_REGISTER),
    			MOTOR_FORWARDS | MOTOR_STOP | MOTOR_SLOW);
    	out32((uintptr_t) (gpio_bank_1 + SET_REGISTER), MOTOR_BACKWARDS);
    }
    void ActorWrapper::motorSlow(){
    	out32((uintptr_t) (gpio_bank_1 + SET_REGISTER), MOTOR_SLOW);
    }
    void ActorWrapper::motorStop(){
    	out32((uintptr_t) (gpio_bank_1 + SET_REGISTER), MOTOR_STOP);
    }

    void ActorWrapper::greenLampOn(){
    	std::lock_guard<std::mutex> lock(lampMutex);
    	out32((uintptr_t) (gpio_bank_1 + SET_REGISTER), GREEN_LAMP_ON);
    }
    void ActorWrapper::greenLampOff(){
    	std::lock_guard<std::mutex> lock(lampMutex);
      	out32((uintptr_t) (gpio_bank_1 + CLR_REGISTER), GREEN_LAMP_ON);
      }

    void ActorWrapper::greenLampBlink(int ms) {
    	greenLampBlinkOff();
        blinkGreenLamp = true;
        greenLampThread = thread([this, ms](){
            while(blinkGreenLamp){
                greenLampOn();
                this_thread::sleep_for(chrono::milliseconds(ms));
                greenLampOff();
                this_thread::sleep_for(chrono::milliseconds(ms));
            }
        });
    }
    void ActorWrapper::greenLampBlinkOff(){
    	blinkGreenLamp = false;
    	if(greenLampThread.joinable()){
    		greenLampThread.join();
    	}
    }


    void ActorWrapper::yellowLampOn(){
    	std::lock_guard<std::mutex> lock(lampMutex);
    	out32((uintptr_t) (gpio_bank_1 + SET_REGISTER), YELLOW_LAMP_ON);
    }
    void ActorWrapper::yellowLampOff(){
    	std::lock_guard<std::mutex> lock(lampMutex);
    	out32((uintptr_t) (gpio_bank_1 + CLR_REGISTER), YELLOW_LAMP_ON);
    }

    void ActorWrapper::yellowLampBlink(int ms){
    	yellowLampBlinkOff();
    	blinkYellowLamp = true;
    	yellowLampThread = thread([this,ms](){
        	while(blinkYellowLamp){
    			yellowLampOn();
    			this_thread::sleep_for(chrono::milliseconds(ms));
    			yellowLampOff();
    			this_thread::sleep_for(chrono::milliseconds(ms));

        	}
    	});
    }
    void ActorWrapper::yellowLampBlinkOff(){
    	blinkYellowLamp = false;
    	if(yellowLampThread.joinable()){
    		yellowLampThread.join();
    	}
    }


    void ActorWrapper::redLampOn(){
    	std::lock_guard<std::mutex> lock(lampMutex);
    	out32((uintptr_t) (gpio_bank_1 + SET_REGISTER), RED_LAMP_ON);
    }
    void ActorWrapper::redLampOff(){
    	std::lock_guard<std::mutex> lock(lampMutex);
    	out32((uintptr_t) (gpio_bank_1 + CLR_REGISTER), RED_LAMP_ON);
    }

    void ActorWrapper::redLampBlink(int ms){
    	redLampBlinkOff();
    	blinkRedLamp = true;
    	redLampThread = thread([this,ms](){
    	while(blinkRedLamp){
    		redLampOn();
			this_thread::sleep_for(chrono::milliseconds(ms));
    		redLampOff();
			this_thread::sleep_for(chrono::milliseconds(ms));

    	}
    });
    }
    void ActorWrapper::redLampBlinkOff(){
    	blinkRedLamp = false;
    	if(redLampThread.joinable()){
    		redLampThread.join();
    	}
    }

    void ActorWrapper::openPusher(){

    	out32((uintptr_t) (gpio_bank_1 + SET_REGISTER), OPEN_PUSHER);
    }
    void ActorWrapper::closePusher(){
    	out32((uintptr_t) (gpio_bank_1 + CLR_REGISTER), OPEN_PUSHER);
    }

    void ActorWrapper::ledButtonStart(){
    	out32((uintptr_t) (gpio_bank_2 + SET_REGISTER), LED_BUTTON_START);
    }
    void ActorWrapper::ledButtonStartOff(){
    	out32((uintptr_t) (gpio_bank_2 + CLR_REGISTER), LED_BUTTON_START);
    }
    void ActorWrapper::ledButtonReset(){
    	out32((uintptr_t) (gpio_bank_2 + SET_REGISTER), LED_BUTTON_RESET);
    }
    void ActorWrapper::ledButtonResetOff(){
    	out32((uintptr_t) (gpio_bank_2 + CLR_REGISTER), LED_BUTTON_RESET);
    }
    void ActorWrapper::lampQ1(){
    	out32((uintptr_t) (gpio_bank_2 + SET_REGISTER), LAMP_Q1);
    }
    void ActorWrapper::lampQ1Off(){
    	out32((uintptr_t) (gpio_bank_2 + CLR_REGISTER), LAMP_Q1);
    }
    void ActorWrapper::lampQ2(){
    	out32((uintptr_t) (gpio_bank_2 + SET_REGISTER), LAMP_Q2);
    }
    void ActorWrapper::lampQ2Off(){
    	out32((uintptr_t) (gpio_bank_2 + CLR_REGISTER), LAMP_Q2);
    }



    HAL::HAL()
    	: sensors(), actors(){}




