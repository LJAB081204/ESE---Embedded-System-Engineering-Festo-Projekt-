/*
 * ProfileBase.h
 *
 *  Created on: 01.06.2026
 *      Author: oliver
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_PROFILEBASE_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_PROFILEBASE_H_
namespace M2{

class ProfileContext;

class ProfileBase{
public:
	virtual ~ProfileBase() = default;
    virtual void entry(ProfileContext& ctx) {}
    virtual void exit(ProfileContext& ctx)  {}

	virtual void eventItemGone (ProfileContext& ctx){}

	virtual void eventHeightHigh(ProfileContext& ctx){}
	virtual void eventHeightFlat(ProfileContext& ctx){}
	virtual void eventHeightHole(ProfileContext& ctx){}
	virtual void eventHeightWrong(ProfileContext& ctx){}

	virtual void eventError(ProfileContext& ctx) {}
};
}

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_PROFILEBASE_H_ */
