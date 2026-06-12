/*
 * TransportBase2.h
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM2_TRANSPORTBASE_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM2_TRANSPORTBASE_H_
namespace M2{

class TransportContext2;  // Vorwärtsdekleration

class TransportBase2{
public:
	virtual ~TransportBase2() = default;

	virtual void eventItemAtEntry(TransportContext2& fsm) 		 {}
	virtual void eventItemInSlide(TransportContext2& fsm)         {}
	virtual void eventItemAtEntryRemote(TransportContext2& fsm)   {}
	virtual void eventItemAtExit(TransportContext2& fsm)          {}
	virtual void eventItemLeftExit(TransportContext2& fsm)  {}
	virtual void eventItemLeftExitRemote(TransportContext2& fsm)  {}
	virtual void eventItemInSlideRemote(TransportContext2& fsm)   {}
	virtual void eventItemGone(TransportContext2& fsm)            {}
	virtual void eventItemArrived(TransportContext2& fsm)         {}

	virtual void eventFestoEmpty(TransportContext2& fsm)         {}

	virtual void eventFestoReady(TransportContext2& fsm)         {}
	virtual void eventFestoNotReady(TransportContext2& fsm)         {}

	virtual void entry(TransportContext2& fsm){}
	virtual void exit(TransportContext2& fsm){}

	virtual void eventItemAtSlide(TransportContext2& fsm){}
};
}


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM2_TRANSPORTBASE_H_ */
