/*
 * MainBase.h
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_MAINFSM_MAINBASE_H_
#define SRC_FSMS_MAINFSM_MAINBASE_H_

class MainContext; // forward declaration

class MainBase {
public:
    virtual ~MainBase() = default;
    virtual void entry(MainContext& ctx) {}
    virtual void exit(MainContext& ctx)  {}

	virtual void eventButtonStartPressed(MainContext& fsm)	  {}
	virtual void eventButtonStartPressedRemote(MainContext& fsm)	  {}

	virtual void eventButtonStartPressedLong(MainContext& fsm){}
	virtual void eventButtonStartPressedLongRemote(MainContext& fsm){}

	virtual void eventButtonStopPressed(MainContext& fsm)     {}
	virtual void eventButtonStopPressedRemote(MainContext& fsm) {}

	virtual void eventButtonResetPressed(MainContext& fsm)    {}
	virtual void eventButtonResetPressedRemote(MainContext& fsm)    {}

	virtual void eventButtonEstopPressed(MainContext& fsm)    {}
	virtual void eventButtonEstopPressedRemote(MainContext& fsm)    {}

	virtual void eventButtonEstopReleased(MainContext& fsm)   {}
	virtual void eventButtonEstopReleasedRemote(MainContext& fsm)   {}

	virtual void eventItemAtEntry(MainContext& fsm)   {}
	virtual void eventItemAtEntryRemote(MainContext& fsm)    {}

	virtual void eventItemLeftEntry(MainContext& fsm) {}
	virtual void eventItemLeftEntryRemote(MainContext& fsm) {}

	virtual void eventItemAtExit(MainContext& fsm) 	  {}
	virtual void eventItemAtExitRemote(MainContext& fsm)     {}

	virtual void eventItemLeftExit(MainContext& fsm)  {}
	virtual void eventItemLeftExitRemote(MainContext& fsm)   {}


	virtual void eventItemAtPusher(MainContext& fsm)  {}
	virtual void eventItemAtPusherRemote(MainContext& fsm)   {}

	virtual void eventItemLeftPusher(MainContext& fsm){}
	virtual void eventItemLeftPusherRemote(MainContext& fsm){}

	virtual void eventItemInSlide(MainContext& fsm)   {}
	virtual void eventItemInSlideRemote(MainContext& fsm)    {}

	virtual void eventItemLeftSlide(MainContext& fsm) {}
	virtual void eventItemLeftSlideRemote(MainContext& fsm) {}


	virtual void eventItemMetal(MainContext& fsm){}
	virtual void eventItemMetalRemote(MainContext& fsm)      {}

	virtual void eventItemArrived (MainContext& fsm){}
	virtual void eventItemGone (MainContext& fsm){}

	virtual void eventFestoReady (MainContext& fsm){}
	virtual void eventFestoNotReady (MainContext& fsm){}

	virtual void eventHeightHigh(MainContext& fsm){}
	virtual void eventHeightFlat(MainContext& fsm){}
	virtual void eventHeightHole(MainContext& fsm){}
	virtual void eventHeightWrong(MainContext& fsm){}

	virtual void eventItemWrong(MainContext& fsm){}
	virtual void eventItemCorrect(MainContext& fsm){}
	virtual void eventUpdate(MainContext& fsm){}

	virtual void eventTimerExpired(MainContext& fsm){}
	virtual void eventError(MainContext& fsm) {}


	virtual void eventSlideFull(MainContext& fsm)      {}
	virtual void eventSlideFullRemote(MainContext& fsm)      {}

	virtual void eventSlideTimerExpired(MainContext& fsm)      {}

};


#endif /* SRC_FSMS_MAINFSM_MAINBASE_H_ */
