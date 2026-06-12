/*
 * Blocked.h
 *
 *  Created on: 08.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_BLOCKED_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_BLOCKED_H_
namespace M1{

#include "SlideBase.h"
class Blocked : public SlideBase{
public:
	Blocked();
	void entry(SlideContext& ctx)override;
	void exit(SlideContext& ctx)override;

	void eventSlideTimerExpired(SlideContext& ctx)override;
	void eventItemLeftSlide(SlideContext& ctx)override;
};

}


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_BLOCKED_H_ */
