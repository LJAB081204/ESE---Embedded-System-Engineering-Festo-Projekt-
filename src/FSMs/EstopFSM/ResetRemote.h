/*
 * ResetRemote.h
 *
 *  Created on: 08.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_ESTOPFSM_RESETREMOTE_H_
#define SRC_FSMS_ESTOPFSM_RESETREMOTE_H_


#include "EstopBase.h"


class ResetRemote : public EStopBase{
public:
	ResetRemote();
	void entry(EStopContext& ctx)override;
	void exit(EStopContext& ctx)override;
	void eventButtonResetPressed(EStopContext& ctx)override;
	void eventButtonEstopPressedRemote(EStopContext& ctx)override;
	void eventButtonEstopPressed(EStopContext& ctx)override;
};




#endif /* SRC_FSMS_ESTOPFSM_RESETREMOTE_H_ */
