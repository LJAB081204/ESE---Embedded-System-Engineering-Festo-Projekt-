/*
 * IM.h
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM2_IM_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM2_IM_H_

#include "SequenceBase.h"
namespace M2{

class IM : public SequenceBase{
public:
	IM();
	void entry(SequenceContext& ctx)override;
	void exit(SequenceContext& ctx)override;

	void eventUpdate(SequenceContext& ctx)override;
	void eventGoBack(SequenceContext& ctx)override;
	void raiseItemWrong(SequenceContext& ctx);
	void raiseItemCorrect(SequenceContext& ctx);
	void raiseGoBackIM(SequenceContext& ctx);

private:
	itemProfile item;
};


}
#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM2_IM_H_ */
