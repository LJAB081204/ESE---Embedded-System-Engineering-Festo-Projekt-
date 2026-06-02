/*
 * TransportBase.h
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_TRANSPORTBASE_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_TRANSPORTBASE_H_

class TransportContext;  // Vorwärtsdekleration

class TransportBase{
public:
	virtual ~TransportBase() = default;

	virtual void eventItemAtEntry(TransportContext& fsm) 		 {}
	virtual void eventItemInSlide(TransportContext& fsm)         {}
	virtual void eventItemAtEntryRemote(TransportContext& fsm)   {}
	virtual void eventItemAtExit(TransportContext& fsm)          {}
	virtual void eventItemLeftExitRemote(TransportContext& fsm)  {}
	virtual void eventItemInSlideRemote(TransportContext& fsm)   {}
	virtual void eventItemGone(TransportContext& fsm)            {}
	virtual void eventItemArrived(TransportContext& fsm)         {}

	virtual void entry(TransportContext& fsm){}
	virtual void exit(TransportContext& fsm){}
};



#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_TRANSPORTBASE_H_ */
