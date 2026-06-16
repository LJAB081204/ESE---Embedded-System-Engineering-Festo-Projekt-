/*
 * errorPusherTooLong.h
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_ERRORFSM_ERRORPUSHERTOOLONG_H_
#define SRC_FSMS_ERRORFSM_ERRORPUSHERTOOLONG_H_


#include "ErrorBase.h"

class errorPusherTooLong : public ErrorBase{
public:
	errorPusherTooLong();
	void entry(ErrorContext& ctx)override;
	void exit(ErrorContext& ctx)override;

	void eventButtonResetPressed(ErrorContext& ctx)override;

};



#endif /* SRC_FSMS_ERRORFSM_ERRORPUSHERTOOLONG_H_ */
