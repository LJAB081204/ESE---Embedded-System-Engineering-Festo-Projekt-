/*
 * HAL.h
 *
 *  Created on: 21.04.2026
 *      Author: tayla
 */

#ifndef HAL_H_
#define HAL_H_

#include <stdint.h>
#include <thread>
#include <mutex>
#include <atomic>
#include "IHAL.h"

// ========================
// GPIO Port Register Length
// ========================
#define GPIO_REGISTER_LENGHT 0x1000
#define ADC_LENGTH 0x2000

// ========================
// GPIO Base Addresses
// ========================
#define GPIO_PORT0 0x44E07000
#define GPIO_PORT1 0x4804C000
#define GPIO_PORT2 0x481AC000
#define ADC_BASE   0x44E0D000

// ========================
// ADC Register Offsets
// ========================
#define ADC_IRQ_ENABLE_SET 0x2C
#define ADC_IRQ_ENABLE_CLR 0x30
#define ADC_IRQ_STATUS     0x28
#define ADC_CTRL           0x40
#define ADC_DATA           0x100

// ========================
// GPIO0 Interrupt Base
// ========================
#define GPIO_0_IRQ_BASE 0x300

// ========================
// GPIO Register Offsets
// ========================
#define GPIO_REVISION        0x00
#define GPIO_SYSCONFIG       0x10
#define GPIO_EOI             0x20
#define GPIO_IRQSTATUS_RAW_0 0x24
#define GPIO_IRQSTATUS_RAW_1 0x28
#define GPIO_IRQSTATUS_0     0x2C
#define GPIO_IRQSTATUS_1     0x30
#define GPIO_IRQSTATUS_SET_0 0x34
#define GPIO_IRQSTATUS_SET_1 0x38
#define GPIO_IRQSTATUS_CLR_0 0x3C
#define GPIO_IRQSTATUS_CLR_1 0x40
#define GPIO_IRQWAKEN_0      0x44
#define GPIO_IRQWAKEN_1      0x48
#define GPIO_SYSSTATUS       0x114
#define GPIO_CTRL            0x130
#define GPIO_OE              0x134
#define GPIO_DATAIN          0x138
#define GPIO_DATAOUT         0x13C
#define GPIO_LEVELDETECT0    0x140
#define GPIO_LEVELDETECT1    0x144
#define GPIO_RISINGDETECT    0x148
#define GPIO_FALLINGDETECT   0x14C
#define GPIO_DEBOUNCENABLE   0x150
#define GPIO_DEBOUNCINGTIME  0x154
#define GPIO_CLEARDATAOUT    0x190
#define GPIO_SETDATAOUT      0x194

// ========================
// GPIO Actor Registers (legacy names)
// ========================
#define CLR_REGISTER  0x190
#define SET_REGISTER  0x194
#define READ_REGISTER 0x138
#define OE_REGISTER   0x134

// ========================
// GPIO0 Input Bitmasks
// ========================
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

// ========================
// GPIO1 Output Bitmasks
// ========================
#define MOTOR_FORWARDS  0x00001000
#define MOTOR_BACKWARDS 0x00002000
#define MOTOR_SLOW      0x00004000
#define MOTOR_STOP      0x00008000
#define RED_LAMP_ON     0x00010000
#define YELLOW_LAMP_ON  0x00020000
#define GREEN_LAMP_ON   0x00040000
#define OPEN_PUSHER     0x00080000

// ========================
// GPIO2 Output Bitmasks
// ========================
#define LED_BUTTON_START 0x00000004
#define LED_BUTTON_RESET 0x00000008
#define LAMP_Q1          0x00000010
#define LAMP_Q2          0x00000020



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
	std::atomic<bool>blinkGreenLamp;
	std::atomic<bool> blinkYellowLamp;
	std::atomic<bool> blinkRedLamp;
	std::thread greenLampThread;
	std::thread yellowLampThread;
	std::thread redLampThread;
    std::mutex lampMutex;


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
    void redLampBlink(int hz) override;
    void redLampBlinkOff() override;
    void yellowLampOn() override;
    void yellowLampOff() override;
    void yellowLampBlink(int hz) override;
    void yellowLampBlinkOff() override;
    void greenLampOn() override;
    void greenLampOff() override;
    void greenLampBlink(int hz) override;
    void greenLampBlinkOff() override;

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
