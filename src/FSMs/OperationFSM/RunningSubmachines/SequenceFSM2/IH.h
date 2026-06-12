/*
 * IH.h
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM2_IH_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM2_IH_H_

#include "SequenceBase.h"
namespace M2{

class IH : public SequenceBase{
public:
	IH();
	void entry(SequenceContext& ctx)override;
	void exit(SequenceContext& ctx)override;

	//void eventItemAtPusher(SequenceContext& ctx) override;
	void eventUpdate(SequenceContext& ctx)override;
	void eventGoBack(SequenceContext& ctx)override;
	void raiseItemWrong(SequenceContext& ctx);
	void raiseItemCorrect(SequenceContext& ctx);
	void raiseGoBackIH(SequenceContext& ctx);

private:
	itemProfile item;
};


}
#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM2_IH_H_ */
