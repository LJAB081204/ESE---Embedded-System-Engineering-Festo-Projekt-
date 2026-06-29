/*
 * ErrorBase.h
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_ERRORFSM_ERRORBASE_H_
#define SRC_FSMS_ERRORFSM_ERRORBASE_H_

class ErrorContext;

class ErrorBase {
public:
	virtual ~ErrorBase() = default;

	virtual void entry(ErrorContext& ctx) {}
	virtual void exit(ErrorContext& ctx) {}

	virtual void eventButtonResetPressed(ErrorContext& ctx) {}
	virtual void eventButtonResetPressedRemote(ErrorContext& ctx) {}
	virtual void eventItemGone(ErrorContext& ctx) {}
	virtual void eventItemLeftSlide(ErrorContext& ctx) {}
	virtual void eventItemLeftSlideRemote(ErrorContext& ctx) {}
	virtual void eventItemLeftPusher(ErrorContext& ctx) {}
	virtual void eventItemLeftExit(ErrorContext& ctx) {}
	virtual void eventConnectionRestored(ErrorContext& ctx) {}
	virtual void errorPusherTimerExpired(ErrorContext& ctx) {}
	virtual void eventButtonStartPressed(ErrorContext& ctx) {}
	virtual void eventItemLeftEntry(ErrorContext& ctx){}
	virtual void eventButtonStartPressedRemote(ErrorContext& ctx){}
	virtual void eventItemArrived(ErrorContext& ctx){}
	virtual void eventItemAtExit(ErrorContext& ctx){}
	virtual void eventItemAtPusher(ErrorContext& ctx){}

};




#endif /* SRC_FSMS_ERRORFSM_ERRORBASE_H_ */
