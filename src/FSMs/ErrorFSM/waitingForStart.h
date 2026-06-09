/*
 * waitingForStart.h
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_ERRORFSM_WAITINGFORSTART_H_
#define SRC_FSMS_ERRORFSM_WAITINGFORSTART_H_

#include "ErrorBase.h"

class waitingForStart : public ErrorBase{
public:
	waitingForStart();
	void entry(ErrorContext& ctx)override;
	void exit(ErrorContext& ctx)override;

	void eventButtonStartPressed(ErrorContext& ctx)override;
	void errorPusherTimerExpired(ErrorContext& ctx)override;

};



#endif /* SRC_FSMS_ERRORFSM_WAITINGFORSTART_H_ */
