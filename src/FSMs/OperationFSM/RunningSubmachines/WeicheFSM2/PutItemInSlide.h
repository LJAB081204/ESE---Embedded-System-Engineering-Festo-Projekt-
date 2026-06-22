/*
 * PutItemInSlide.h
 *
 *  Created on: 02.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM2_PUTITEMINSLIDE_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM2_PUTITEMINSLIDE_H_


#include "WeicheBase.h"
namespace M2{

class PutItemInSlide : public WeicheBase{
public:
	PutItemInSlide();
	void entry(WeicheContext& ctx)override;
	//void eventItemInSlide(WeicheContext& ctx)override;
	void eventItemLeftPusher(WeicheContext& ctx)override;
};
}

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM2_PUTITEMINSLIDE_H_ */
