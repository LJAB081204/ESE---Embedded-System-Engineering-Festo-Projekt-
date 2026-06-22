/*
 * Running2.h
 *
 *  Created on: 28.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_Running2_H_
#define SRC_FSMS_OPERATIONFSM_Running2_H_

#include "OperationBase.h"
#include "../../dataClass.h"
#include "../../timer.h"


namespace M2{

class TransportContext2;
class WeicheContext;
class ProfileContext;
class SequenceContext;
class SlideContext;

class Running2 : public OperationBase{
	TransportContext2* transportFSM;
	WeicheContext* weicheFSM;
	SequenceContext* sequenceFSM;
	ProfileContext* profileFSM;
	SlideContext* slideFSM;
public:
	Running2(HAL& hal,dataClass& data,SequenceContext& seq,TimerHandler& timerHandler);

	void onRestore(HistorySource source,OperationContext& ctx)override;
	void entry(OperationContext& ctx)override;
	void exit(OperationContext& ctx)override;
	// Buttons
	void eventButtonStopPressed(OperationContext& ctx) override;
	void eventButtonStopPressedRemote(OperationContext& ctx) override;

	// Sensoren lokal
	void eventItemAtEntry(OperationContext& ctx) override;
	void eventItemLeftEntry(OperationContext& ctx) override;
	void eventItemAtExit(OperationContext& ctx) override;
	void eventItemLeftExit(OperationContext& ctx) override;
	void eventItemAtPusher(OperationContext& ctx) override;
	void eventItemLeftPusher(OperationContext& ctx) override;
	void eventItemInSlide(OperationContext& ctx) override;
	void eventItemLeftSlide(OperationContext& ctx) override;
	void eventItemMetal(OperationContext& ctx) override;
	void eventItemGone(OperationContext& ctx) override;
	void eventItemArrived(OperationContext& ctx) override;

	// Sensoren remote
	void eventItemAtEntryRemote(OperationContext& ctx) override;
	void eventItemLeftEntryRemote(OperationContext& ctx) override;
	void eventItemAtExitRemote(OperationContext& ctx) override;
	void eventItemLeftExitRemote(OperationContext& ctx) override;
	void eventItemAtPusherRemote(OperationContext& ctx) override;
	void eventItemLeftPusherRemote(OperationContext& ctx) override;
	void eventItemInSlideRemote(OperationContext& ctx) override;
	void eventItemLeftSlideRemote(OperationContext& ctx) override;
	void eventItemMetalRemote(OperationContext& ctx) override;
	void eventSlideFullRemote(OperationContext& ctx) override;

	void eventSlideTimerExpired(OperationContext& ctx) override;

	// Höhensensor / Profil
	void eventHeightHigh(OperationContext& ctx) override;
	void eventHeightFlat(OperationContext& ctx) override;
	void eventHeightHole(OperationContext& ctx) override;
	void eventHeightWrong(OperationContext& ctx) override;

	// Festo
	void eventFestoReady(OperationContext& ctx) override;
	void eventFestoNotReady(OperationContext& ctx) override;

	// Logik
	void eventItemCorrect(OperationContext& ctx) override;
	void eventItemWrong(OperationContext& ctx) override;
	void eventUpdate(OperationContext& ctx) override;
	void eventError(OperationContext& ctx) override;
	void eventTimerExpired(OperationContext& ctx) override;
	void eventItemTransfer(uint32_t itemCode, OperationContext& ctx) override;
	void eventGoBack(OperationContext& ctx) override;


};

}
#endif /* SRC_FSMS_OPERATIONFSM_Running2_H_ */
