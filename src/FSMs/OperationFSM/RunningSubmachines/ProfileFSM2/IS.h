/*
 * IS.h
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_IS_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_IS_H_

#include "ProfileBase.h"
namespace M2{

class IS : public ProfileBase{
public:
	IS();
	void entry(ProfileContext& ctx) override;
	void exit(ProfileContext& ctx) override;

	virtual void eventItemGone(ProfileContext& ctx)override;

};

}
#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_IS_H_ */
