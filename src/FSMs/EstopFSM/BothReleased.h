/*
 * BothReleased.h
 *
 *  Created on: 08.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_ESTOPFSM_BOTHRELEASED_H_
#define SRC_FSMS_ESTOPFSM_BOTHRELEASED_H_

#include "EstopBase.h"


class BothReleased : public EStopBase{
public:
	BothReleased();
	void entry(EStopContext& ctx)override;
	void exit(EStopContext& ctx)override;
	void eventButtonResetPressed(EStopContext& ctx)override;
	void eventButtonResetPressedRemote(EStopContext& ctx)override;
	void eventButtonEstopPressed(EStopContext& ctx)override;
	void eventButtonEstopPressedRemote(EStopContext& ctx)override;

};



#endif /* SRC_FSMS_ESTOPFSM_BOTHRELEASED_H_ */
