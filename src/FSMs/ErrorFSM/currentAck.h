/*
 * currentAck.h
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_ERRORFSM_CURRENTACK_H_
#define SRC_FSMS_ERRORFSM_CURRENTACK_H_

#include "ErrorBase.h"

class currentAck : public ErrorBase{
public:
	currentAck();
	void entry(ErrorContext& ctx)override;
	void exit(ErrorContext& ctx)override;

	void eventItemLeftSlide(ErrorContext& ctx)override;
	void eventItemGone(ErrorContext& ctx)override;
	void eventItemLeftSlideRemote(ErrorContext& ctx)override;
	void errorPusherTimerExpired(ErrorContext& ctx)override;
	void eventItemLeftPusher(ErrorContext& ctx)override;
	void eventItemLeftExit(ErrorContext& ctx)override;
	void eventConnectionRestored(ErrorContext& ctx)override;


	void raiseItemAtPusher(ErrorContext& ctx);
	void raiseItemGone(ErrorContext& ctx);
};



#endif /* SRC_FSMS_ERRORFSM_CURRENTACK_H_ */
