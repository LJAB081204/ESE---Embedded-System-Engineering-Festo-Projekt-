/*
 * IHAL.h
 *
 *  Created on: 27.04.2026
 *      Author: tayla
 */

#ifndef IHAL_H_
#define IHAL_H_


// Sensor Interface

class ISensor {
public:
	virtual ~ISensor() = default;

    // Sensors - Item (WS)
    virtual bool isItemAtEntry() = 0;
    virtual bool isItemInHeightMeas() = 0;
    virtual bool isItemHeightOk() = 0;
    virtual bool isItemInPusher() = 0;
    virtual bool isItemMetal() = 0;
    virtual bool isItemAtExit() = 0;

    // Sensors - Pusher and Slide
    virtual bool isPusherOpen() = 0;
    virtual bool isSlideFull() = 0;

    // Buttons
    virtual bool isButtonStartPressed() = 0;
    virtual bool isButtonStopPressed() = 0;
    virtual bool isButtonResetPressed() = 0;
    virtual bool isEStopTriggered() = 0;
};

// Class Interface
class IActor {
public:
	virtual ~IActor() = default;

    // Motor
    virtual void motorForwards() = 0;
    virtual void motorBackwards() = 0;
    virtual void motorSlow() = 0;
    virtual void motorStop() = 0;

    // Lamps
    virtual void redLampOn() = 0;
    virtual void redLampOff() = 0;
    virtual void redLampBlink(int) = 0;
    virtual void redLampBlinkOff() = 0;
    virtual void yellowLampOn() = 0;
    virtual void yellowLampOff() = 0;
    virtual void yellowLampBlink(int) = 0;
    virtual void yellowLampBlinkOff() = 0;
    virtual void greenLampOn() = 0;
    virtual void greenLampOff() = 0;
    virtual void greenLampBlink(int) = 0;
    virtual void greenLampBlinkOff() = 0;

    // Pusher
    virtual void openPusher() = 0;
    virtual void closePusher() = 0;
    virtual void sortItemOut() = 0;
    virtual void letItemPass() = 0;

    // LEDs
    virtual void ledButtonStart() = 0;
    virtual void ledButtonStartOff() = 0;
    virtual void ledButtonReset() = 0;
    virtual void ledButtonResetOff() = 0;
    virtual void lampQ1() = 0;
    virtual void lampQ1Off() = 0;
    virtual void lampQ2() = 0;
    virtual void lampQ2Off() = 0;

    virtual bool isJunction() = 0;
};





#endif /* IHAL_H_ */
