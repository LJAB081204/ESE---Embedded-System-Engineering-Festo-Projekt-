/*
 * ProfileIH.h
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM2_PROFILEIH_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM2_PROFILEIH_H_

#include "ProfileBase.h"
namespace M2{

class ProfileIH : public ProfileBase{
public:
	ProfileIH();
	void entry(ProfileContext& ctx) override;
	void exit(ProfileContext& ctx) override;

	virtual void eventItemGone (ProfileContext& ctx)override;

	virtual void eventHeightHigh(ProfileContext& ctx)override;
	virtual void eventHeightFlat(ProfileContext& ctx)override;
	virtual void eventHeightWrong(ProfileContext& ctx)override;
};
}

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM2_PROFILEIH_H_ */
