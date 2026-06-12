/*
 * IU.h
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_IU_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_IU_H_

#include "ProfileBase.h"
namespace M1{

class IU : public ProfileBase{
public:
	IU();
	void entry(ProfileContext& ctx) override;
	void exit(ProfileContext& ctx) override;

	virtual void eventItemGone (ProfileContext& ctx)override;

	virtual void eventHeightWrong(ProfileContext& ctx)override;
};
}

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_IU_H_ */
