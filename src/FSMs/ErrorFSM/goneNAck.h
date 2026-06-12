/*
 * goneNack.h
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_ERRORFSM_GONENACK_H_
#define SRC_FSMS_ERRORFSM_GONENACK_H_

#include "ErrorBase.h"

class goneNAck : public ErrorBase{
public:
	goneNAck();
	void entry(ErrorContext& ctx)override;
	void exit(ErrorContext& ctx)override;

	void eventButtonResetPressed(ErrorContext& ctx)override;
	void errorPusherTimerExpired(ErrorContext& ctx)override;

};



#endif /* SRC_FSMS_ERRORFSM_GONENACK_H_ */
