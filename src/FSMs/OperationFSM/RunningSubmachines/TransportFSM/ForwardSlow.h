/*
 * ForwardSlow.h
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_FORWARDSLOW_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_FORWARDSLOW_H_

#include "TransportBase.h"
namespace M1{

class ForwardSlow : public TransportBase{
public:
	ForwardSlow();
	void entry(TransportContext& ctx)override;
	void exit(TransportContext& ctx)override;

	void eventItemGone(TransportContext& ctx)override;
	void eventItemAtExit(TransportContext& ctx)override;
	void eventItemAtEntry(TransportContext& ctx)override;
	void eventItemAtEntryRemote(TransportContext& ctx)override;
	void eventItemInSlide(TransportContext& ctx)override;
	void eventItemLeftEntry(TransportContext& ctx)override;
	void eventItemAtPusher(TransportContext& ctx)override;


	void raiseRequestTransfer(TransportContext& ctx);
};


}
#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_FORWARDSLOW_H_ */
