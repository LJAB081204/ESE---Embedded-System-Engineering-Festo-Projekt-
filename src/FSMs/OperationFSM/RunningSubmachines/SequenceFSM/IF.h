/*
 * IF.h
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_IF_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_IF_H_

#include "SequenceBase.h"

class IF : public SequenceBase{
public:
	IF();
	void entry(SequenceContext& ctx)override;
	void exit(SequenceContext& ctx)override;

	void eventUpdate(SequenceContext& ctx)override;
	void eventItemWrong(SequenceContext& ctx)override;
	void eventItemCorrect(SequenceContext& ctx)override;
};



#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_IF_H_ */
