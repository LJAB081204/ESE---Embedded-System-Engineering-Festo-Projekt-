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
	OperationContext operationFSM;
public:
	Operation(HAL& hal,dataClass& data) : operationFSM(hal,data) {}
	void entry(MainContext& ctx) override;
	void exit(MainContext& ctx) override;
	void eventButtonStartPressed(MainContext& ctx) override;
	void eventButtonStartPressedLong(MainContext& ctx) override;
	void eventButtonStopPressed(MainContext& ctx) override;
	void eventButtonResetPressed(MainContext& ctx) override;
	void eventButtonEstopPressed(MainContext& ctx) override;

    void eventItemAtEntry(MainContext& ctx) override;
    void eventItemLeftEntry(MainContext& ctx) override;

    void eventItemAtExit(MainContext& ctx) override;
    void eventItemLeftExit(MainContext& ctx) override;

    void eventItemAtPusher(MainContext& ctx) override;
    void eventItemLeftPusher(MainContext& ctx) override;

    void eventItemInSlide(MainContext& ctx) override;
    void eventItemLeftSlide(MainContext& ctx) override;

    void eventItemMetal(MainContext& ctx) override;

    void eventError(MainContext& ctx) override;

    void eventItemGone(MainContext& ctx) override;

    void eventHeightHigh(MainContext& ctx) override;
    void eventHeightFlat(MainContext& ctx) override;
    void eventHeightHole(MainContext& ctx) override;
    void eventHeightWrong(MainContext& ctx) override;

    void eventItemWrong(MainContext& ctx) override;
    void eventItemCorrect(MainContext& ctx) override;
    void eventUpdate(MainContext& ctx) override;

    void eventTimerExpired(MainContext& ctx) override;
};



#endif /* SRC_FSMS_MAINFSM_OPERATION_H_ */
