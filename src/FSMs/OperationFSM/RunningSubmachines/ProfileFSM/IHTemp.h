/*
 * IHTemp.h
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_IHTEMP_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_IHTEMP_H_

#include "ProfileBase.h"

class IHTemp : public ProfileBase{
public:
	IHTemp();
	void entry(ProfileContext& ctx) override;
	void exit(ProfileContext& ctx) override;

	virtual void eventItemGone(ProfileContext& fsm)override;

	virtual void eventHeightHigh(ProfileContext& fsm)override;
	virtual void eventHeightHole(ProfileContext& fsm)override;
	virtual void eventHeightWrong(ProfileContext& fsm)override;
};


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_IHTEMP_H_ */
