/*
 * I.h
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_I_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_I_H_

#include "ProfileBase.h"

class I : public ProfileBase{
public:
	I();
	void entry(ProfileContext& ctx) override;
	void exit(ProfileContext& ctx) override;

	virtual void eventItemGone (ProfileContext& ctx)override;

	virtual void eventHeightHole(ProfileContext& ctx)override;
	virtual void eventHeightFlat(ProfileContext& ctx)override;
	virtual void eventHeightWrong(ProfileContext& ctx)override;
};


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_I_H_ */
