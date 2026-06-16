/*
 * SequenceReceive.h
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_WAITINGFORPROFILE_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_WAITINGFORPROFILE_H_

#include "SequenceBase.h"
namespace M1{

class waitingForProfile : public SequenceBase{
public:
	waitingForProfile();
	void entry(SequenceContext& ctx)override;
	void exit(SequenceContext& ctx)override;

	void eventUpdate(SequenceContext& ctx)override;
	void raiseItemWrong(SequenceContext& ctx);
	void raiseItemCorrect(SequenceContext& ctx);
};

}
#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_WAITINGFORPROFILE_H_ */
