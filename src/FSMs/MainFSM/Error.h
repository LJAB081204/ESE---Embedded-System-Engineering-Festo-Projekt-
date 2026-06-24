/*
 * Error.h
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_MAINFSM_ERROR_H_
#define SRC_FSMS_MAINFSM_ERROR_H_


#include "MainBase.h"
#include "../ErrorFSM/ErrorContext.h"

class Error : public MainBase{
	ErrorContext errorFSM;
public:
	Error(HAL& hal,dataClass& data);
	void entry(MainContext& ctx) override;
    void exit(MainContext& ctx) override;

	void eventButtonResetPressed (MainContext& ctx) override;
	void eventItemGone(MainContext& ctx) override;
	void eventItemLeftSlide(MainContext& ctx) override;
	void eventItemLeftSlideRemote(MainContext& ctx) override;
	void eventItemLeftPusher(MainContext& ctx) override;
	void eventItemLeftExit(MainContext& ctx) override;
	void eventConnectionRestored(MainContext& ctx) override;
	void errorPusherTimerExpired(MainContext& ctx) override;
	void eventButtonStartPressed(MainContext& ctx) override;
	void eventErrorResolved(MainContext& ctx)override;
	void eventGoBackIH(MainContext& ctx)override;
	void eventGoBackIM(MainContext& ctx)override;
	void eventGoBackIF(MainContext& ctx)override;
	void eventGoBackIHM(MainContext& ctx)override;
	void eventGoBackWFP(MainContext& ctx)override;
	void eventGoBack(MainContext& ctx)override;

	void eventButtonEstopPressed(MainContext& ctx)override;
	void eventButtonEstopPressedRemote(MainContext& ctx)override;
	void enterPusherTooLong() { errorFSM.errorPusherTimerExpired(); }
	void eventConnectionLost(MainContext& ctx)override;

	void eventItemLeftEntry(MainContext& ctx) override;


};


#endif /* SRC_FSMS_MAINFSM_ERROR_H_ */
