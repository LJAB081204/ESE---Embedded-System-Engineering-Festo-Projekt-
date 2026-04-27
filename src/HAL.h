/*
 * HAL.h
 *
 *  Created on: 21.04.2026
 *      Author: tayla
 */

#ifndef HAL_H_
#define HAL_H_

#include <stdint.h>
#include <stdexcept>
#include <sys/mman.h>
#include <hw/inout.h>
#include <sys/neutrino.h>
#include <iostream>
using namespace std;
#include "IHAL.h"

// GPIO port registers length
#define GPIO_REGISTER_LENGHT 0x1000
#define ADC_LENGTH 0x2000

// GPIO Base Adresses (spruh731.pdf page
#define GPIO_PORT0 0x44E07000
#define GPIO_PORT1 0x4804C000
#define GPIO_PORT2 0x481AC000
#define ADC_BASE 0x44E0D000


//  TSC_ADC register offsets (spruh73l.pdf S.1747)
#define ADC_IRQ_ENABLE_SET 0x2c
#define ADC_IRQ_ENABLE_CLR 0x30
#define ADC_IRQ_STATUS 0x28
#define ADC_CTRL 0x40
#define ADC_DATA 0x100

//GPIO Registers - Offsets (spruh73l.pdf page 4877)
#define CLR_REGISTER 0x190
#define SET_REGISTER 0x194
#define READ_REGISTER 0x138

// Bitmasken für GPIO0 (EINGANG)
#define ITEM_AT_ENTRY       0x00000004
#define ITEM_IN_HEIGHT_MEAS 0x00000008
#define ITEM_HEIGHT_OK      0x00000010
#define ITEM_IN_PUSHER      0x00000020
#define ITEM_METAL          0x00000080
#define PUSHER_OPEN         0x00004000
#define SLIDE_FULL          0x00008000
#define ITEM_AT_EXIT        0x00100000
#define BUTTON_START        0x00400000
#define BUTTON_STOP         0x00800000
#define BUTTON_RESET        0x04000000
#define ESTOP               0x08000000

// Bitmasken für GPIO1 (AUSGANG)
#define MOTOR_FORWARDS  	0x00001000
#define MOTOR_BACKWARDS 	0x00002000
#define MOTOR_SLOW      	0x00004000
#define MOTOR_STOP      	0x00008000
#define RED_LAMP_ON  		0x00010000
#define YELLOW_LAMP_ON 		0x00020000
#define GREEN_LAMP_ON   	0x00040000
#define OPEN_PUSHER   		0x00080000

// Bitmasken für GPIO2 (AUSGANG)

#define LED_BUTTON_START     0x00000004
#define LED_BUTTON_RESET     0x00000008
#define LAMP_Q1    			 0x00000010
#define LAMP_Q2			     0x00000020


class SensorWrapper : public ISensor {

private:
    uintptr_t gpio_bank_0;
    uintptr_t adcBaseAddr;


public:
	SensorWrapper();
	virtual ~SensorWrapper();

	// ISensor Implementation
	bool isItemAtEntry() override;
	bool isItemInHeightMeas() override;
	bool isItemHeightOk() override;
	bool isItemInPusher() override;
	bool isItemMetal() override;
	bool isItemAtExit() override;

	bool isPusherOpen() override;
	bool isSlideFull() override;

	bool isButtonStartPressed() override;
	bool isButtonStopPressed() override;
	bool isButtonResetPressed() override;
	bool isEStopTriggered() override;
};

class ActorWrapper : public IActor {
private:
    uintptr_t gpio_bank_1;
    uintptr_t gpio_bank_2;


public:
    ActorWrapper();
    virtual ~ActorWrapper();

    // IActor Implementation
    void motorForwards() override;
    void motorBackwards() override;
    void motorSlow() override;
    void motorStop() override;

    void redLampOn() override;
    void redLampOff() override;
    void redLampBlink() override;
    void yellowLampOn() override;
    void yellowLampOff() override;
    void yellowLampBlink() override;
    void greenLampOn() override;
    void greenLampOff() override;
    void greenLampBlink() override;

    void openPusher() override;
    void closePusher() override;

    void ledButtonStart() override;
    void ledButtonStartOff() override;
    void ledButtonReset() override;
    void ledButtonResetOff() override;
    void lampQ1() override;
    void lampQ1Off() override;
    void lampQ2() override;
    void lampQ2Off() override;
};

//HAL Hauptklasse
class HAL {
private:
	SensorWrapper sensors;
	ActorWrapper actors;

public:
	HAL();
	virtual ~HAL() = default;
	// Getter für Wrapper evtl Code in cpp moven
	ISensor* getSensors()  {return &sensors;}
	IActor*  getActors ()  {return &actors;}
};

#endif /* HAL_H_ */
