/*
 * Forward.h
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_FORWARD_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_FORWARD_H_

#include "TransportBase.h"

class Forward : public TransportBase{
public:
	Forward();
	void entry(TransportContext& ctx)override;
	void exit(TransportContext& ctx)override;

	void eventItemAtExit(TransportContext& ctx)override;
	void eventItemArrived(TransportContext& ctx)override;
	void eventItemLeftExit(TransportContext& ctx)override;
	void eventFestoEmpty(TransportContext& ctx)override;
	void eventItemAtEntry(TransportContext& ctx)override;
	void eventItemInSlide(TransportContext& ctx)override;
};



#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_FORWARD_H_ */
