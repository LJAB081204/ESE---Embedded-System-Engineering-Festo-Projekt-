/*
 * waitingForResolve.h
 *
 *  Created on: 23.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_ERRORFSM_WAITINGFORRESOLVE_H_
#define SRC_FSMS_ERRORFSM_WAITINGFORRESOLVE_H_

#include "ErrorBase.h"

class waitingForResolve : public ErrorBase{
public:
	waitingForResolve();
	void entry(ErrorContext& ctx)override;
	void exit(ErrorContext& ctx)override;
};



#endif /* SRC_FSMS_ERRORFSM_WAITINGFORRESOLVE_H_ */
