/*
 * waitingForStart2.h
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_ERRORFSM_WAITINGFORSTART2_H_
#define SRC_FSMS_ERRORFSM_WAITINGFORSTART2_H_

#include "ErrorBase.h"

class waitingForStart2 : public ErrorBase{
public:
	waitingForStart2();
	void entry(ErrorContext& ctx)override;
	void exit(ErrorContext& ctx)override;

	void eventButtonStartPressed(ErrorContext& ctx)override;

};



#endif /* SRC_FSMS_ERRORFSM_WAITINGFORSTART2_H_ */
