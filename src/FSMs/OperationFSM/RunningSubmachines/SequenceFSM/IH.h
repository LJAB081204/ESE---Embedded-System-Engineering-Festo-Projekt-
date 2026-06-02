/*
 * IH.h
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_IH_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_IH_H_

#include "SequenceBase.h"

class IH : public SequenceBase{
public:
	IH();
	void entry(SequenceContext& ctx)override;
	void exit(SequenceContext& ctx)override;

	void eventUpdate(SequenceContext& ctx)override;
	void eventItemWrong(SequenceContext& ctx)override;
	void eventItemCorrect(SequenceContext& ctx)override;
};



#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_IH_H_ */
