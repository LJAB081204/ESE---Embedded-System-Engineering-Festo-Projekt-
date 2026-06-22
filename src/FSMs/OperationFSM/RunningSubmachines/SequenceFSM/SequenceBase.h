/*
 * SequenceBase.h
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_SEQUENCEBASE_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_SEQUENCEBASE_H_
namespace M1{

class SequenceContext;	//Vorwärtsdeklaration

class SequenceBase{
public:
	virtual ~SequenceBase() = default;

	virtual void eventUpdate(SequenceContext &fsm)			{}
	virtual void eventItemCorrect(SequenceContext &fsm)		{}
	virtual void eventItemWrong(SequenceContext &fsm)		{}
	virtual	void eventGoBackWFP(SequenceContext& ctx)		{}
	virtual void eventGoBackIH(SequenceContext& ctx)		{}
	virtual	void eventGoBackIM(SequenceContext& ctx)		{}
	virtual void eventGoBackIF(SequenceContext& ctx)		{}
	virtual void eventGoBackIHM(SequenceContext& ctx)		{}



	virtual void entry(SequenceContext &fsm){}
	virtual void exit(SequenceContext &fsm) {}
};

}
#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_SEQUENCEBASE_H_ */
