/*
 * Free.h
 *
 *  Created on: 08.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_FREE_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_FREE_H_


#include "SlideBase.h"
class Free : public SlideBase{
public:
	Free();
	void entry(SlideContext& ctx)override;
	void exit(SlideContext& ctx)override;

	void eventItemAtSlide(SlideContext& ctx)override;
};


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_FREE_H_ */
