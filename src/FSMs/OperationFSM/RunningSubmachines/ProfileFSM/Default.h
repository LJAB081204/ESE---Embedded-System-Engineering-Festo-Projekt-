/*
 * Default.h
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_DEFAULT_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_DEFAULT_H_

#include "ProfileBase.h"

class Default : public ProfileBase{
public:
	Default();
	void entry(ProfileContext& ctx) override;
	void exit(ProfileContext& ctx) override;
	virtual void eventHeightHigh(ProfileContext& ctx)override;
	virtual void eventHeightFlat(ProfileContext& ctx)override;
	virtual void eventHeightWrong(ProfileContext& ctx)override;
};

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_DEFAULT_H_ */
