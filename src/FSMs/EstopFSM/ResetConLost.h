/*
 * ResetConLost.h
 *
 *  Created on: 22.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_ESTOPFSM_RESETCONLOST_H_
#define SRC_FSMS_ESTOPFSM_RESETCONLOST_H_

#include "EstopBase.h"


class ResetConLost : public EStopBase{
public:
	ResetConLost();
	void entry(EStopContext& ctx)override;
	void exit(EStopContext& ctx)override;
	void eventButtonResetPressedRemote(EStopContext& ctx)override;
	void eventButtonResetPressed(EStopContext& ctx)override;



};



#endif /* SRC_FSMS_ESTOPFSM_RESETCONLOST_H_ */
