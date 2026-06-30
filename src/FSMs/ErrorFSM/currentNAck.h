/*
 * currentNAck.h
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_ERRORFSM_CURRENTNACK_H_
#define SRC_FSMS_ERRORFSM_CURRENTNACK_H_

#include "ErrorBase.h"

class currentNAck : public ErrorBase{
public:
	currentNAck();
	void entry(ErrorContext& ctx)override;
	void exit(ErrorContext& ctx)override;

	void eventItemLeftSlide(ErrorContext& ctx)override;
	void eventButtonStartPressed(ErrorContext& ctx)override;
	void eventButtonResetPressed(ErrorContext& ctx)override;
	void errorPusherTimerExpired(ErrorContext& ctx)override;
	void eventItemArrived(ErrorContext& ctx)override;
	void eventItemAtExit(ErrorContext& ctx)override;
	void eventItemAtPusher(ErrorContext& ctx)override;
};



#endif /* SRC_FSMS_ERRORFSM_CURRENTNACK_H_ */
