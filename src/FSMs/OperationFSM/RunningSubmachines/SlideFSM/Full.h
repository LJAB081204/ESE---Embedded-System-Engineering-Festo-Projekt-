/*
 * Full.h
 *
 *  Created on: 08.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_FULL_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_FULL_H_


#include "SlideBase.h"
class Full : public SlideBase{
public:
	Full();
	void entry(SlideContext& ctx)override;
	void exit(SlideContext& ctx)override;

	void eventItemLeftSlide(SlideContext& ctx)override;

};


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_FULL_H_ */
