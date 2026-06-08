/*
 * EStop.h
 *
 *  Created on: 08.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_ESTOPFSM_ESTOPSTATE_H_
#define SRC_FSMS_ESTOPFSM_ESTOPSTATE_H_

#include "EstopBase.h"


class EStopState : public EStopBase{
public:
	EStopState();
	void entry(EStopContext& ctx)override;
	void exit(EStopContext& ctx)override;
	void eventButtonEstopPressedRemote(EStopContext& ctx)override;
	void eventButtonEstopReleased(EStopContext& ctx)override;
};


#endif /* SRC_FSMS_ESTOPFSM_ESTOPSTATE_H_ */
