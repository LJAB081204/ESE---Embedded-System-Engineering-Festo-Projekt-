/*
 * Forward2.h
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM2_FORWARD_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM2_FORWARD_H_

#include "TransportBase2.h"
namespace M2{
class Forward2 : public TransportBase2{
public:
	Forward2();
	void entry(TransportContext2& ctx)override;
	void exit(TransportContext2& ctx)override;

	void eventItemAtExit(TransportContext2& ctx)override;
	void eventItemAtSlide(TransportContext2& ctx)override;
	void eventItemArrived(TransportContext2& ctx)override;
};

}

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM2_FORWARD_H_ */
