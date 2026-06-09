/*
 * BothEStop.h
 *
 *  Created on: 08.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_ESTOPFSM_BOTHESTOP_H_
#define SRC_FSMS_ESTOPFSM_BOTHESTOP_H_

#include "EstopBase.h"

class BothEStop : public EStopBase{
public:
	BothEStop();
	void entry(EStopContext& ctx)override;
	void exit(EStopContext& ctx)override;
	void eventButtonEstopReleased(EStopContext& ctx)override;
	void eventButtonEstopReleasedRemote(EStopContext& ctx)override;

};



#endif /* SRC_FSMS_ESTOPFSM_BOTHESTOP_H_ */
