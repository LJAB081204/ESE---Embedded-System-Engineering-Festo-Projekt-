/*
 * Operation.h
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_MAINFSM_OPERATION_H_
#define SRC_FSMS_MAINFSM_OPERATION_H_

#include "MainBase.h"
#include "../OperationFSM/OperationContext.h"
#include "../../dataClass.h"


class Operation : public MainBase{
	OperationContext& operationFSM;
public:
    Operation(OperationContext& fsm) : operationFSM(fsm) {}
	void entry(MainContext& ctx) override;
	void exit(MainContext& ctx) override;

	void eventButtonStartPressed(MainContext& ctx) override;
    void eventButtonStartPressedRemote(MainContext& ctx) override;

	void eventButtonStartPressedLong(MainContext& ctx) override;
    void eventButtonStartPressedLongRemote(MainContext& ctx) override;


	void eventButtonStopPressed(MainContext& ctx) override;
    void eventButtonStopPressedRemote(MainContext& ctx) override;

	void eventButtonResetPressed(MainContext& ctx) override;
	void eventButtonResetPressedRemote(MainContext& ctx) override;

	void eventButtonEstopPressed(MainContext& ctx) override;
	void eventButtonEstopPressedRemote(MainContext& ctx) override;

    void eventItemAtEntry(MainContext& ctx) override;
    void eventItemAtEntryRemote(MainContext& ctx) override;

    void eventItemLeftEntry(MainContext& ctx) override;
    void eventItemLeftEntryRemote(MainContext& ctx) override;

    void eventItemAtExit(MainContext& ctx) override;
    void eventItemAtExitRemote(MainContext& ctx) override;

    void eventItemLeftExit(MainContext& ctx) override;
    void eventItemLeftExitRemote(MainContext& ctx) override;


    void eventItemAtPusher(MainContext& ctx) override;
    void eventItemAtPusherRemote(MainContext& ctx) override;

    void eventItemLeftPusher(MainContext& ctx) override;
    void eventItemLeftPusherRemote(MainContext& ctx) override;


    void eventItemInSlide(MainContext& ctx) override;
    void eventItemInSlideRemote(MainContext& ctx) override;

    void eventItemLeftSlide(MainContext& ctx) override;
    void eventItemLeftSlideRemote(MainContext& ctx) override;


    void eventItemMetal(MainContext& ctx) override;
    void eventItemMetalRemote(MainContext& ctx) override;


    void eventError(MainContext& ctx) override;

    void eventItemGone(MainContext& ctx) override;
    void eventItemArrived(MainContext& ctx) override;

    void eventFestoReady(MainContext& ctx) override;
    void eventFestoNotReady(MainContext& ctx) override;

    void eventHeightHigh(MainContext& ctx) override;
    void eventHeightFlat(MainContext& ctx) override;
    void eventHeightHole(MainContext& ctx) override;
    void eventHeightWrong(MainContext& ctx) override;

    void eventItemWrong(MainContext& ctx) override;
    void eventItemCorrect(MainContext& ctx) override;
    void eventUpdate(MainContext& ctx) override;

    void eventTimerExpired(MainContext& ctx) override;




    void eventSlideFull(MainContext& ctx) override;
    void eventSlideFullRemote(MainContext& ctx) override;

    void eventSlideTimerExpired(MainContext& ctx) override;

    void eventGoBack(MainContext& ctx) override;

    void eventGoBackWFP(MainContext& ctx) override;
    void eventGoBackIH(MainContext& ctx) override;
    void eventGoBackIM(MainContext& ctx) override;
    void eventGoBackIF(MainContext& ctx) override;
    void eventGoBackIHM(MainContext& ctx) override;

    void errorPusherTimerExpired(MainContext& ctx) override;

    void eventConnectionLost(MainContext& ctx) override;

};



#endif /* SRC_FSMS_MAINFSM_OPERATION_H_ */
