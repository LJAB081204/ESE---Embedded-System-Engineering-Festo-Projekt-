/*
 * EStopRemote.h
 *
 *  Created on: 08.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_ESTOPFSM_ESTOPREMOTE_H_
#define SRC_FSMS_ESTOPFSM_ESTOPREMOTE_H_

#include "EstopBase.h"


class EStopRemote : public EStopBase{
public:
	EStopRemote();
	void entry(EStopContext& ctx)override;
	void exit(EStopContext& ctx)override;
	void eventButtonEstopPressed(EStopContext& ctx)override;
	void eventButtonEstopReleasedRemote(EStopContext& ctx)override;

};


#endif /* SRC_FSMS_ESTOPFSM_ESTOPREMOTE_H_ */
