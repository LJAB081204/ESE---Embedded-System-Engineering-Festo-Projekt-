/*
 * Reset.h
 *
 *  Created on: 08.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_ESTOPFSM_RESET_H_
#define SRC_FSMS_ESTOPFSM_RESET_H_


#include "EstopBase.h"


class Reset : public EStopBase{
public:
	Reset();
	void entry(EStopContext& ctx)override;
	void exit(EStopContext& ctx)override;
	void eventButtonResetPressedRemote(EStopContext& ctx)override;
	void eventButtonEstopPressedRemote(EStopContext& ctx)override;
	void eventButtonEstopPressed(EStopContext& ctx)override;


};

#endif
