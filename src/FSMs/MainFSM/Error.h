/*
 * Error.h
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_MAINFSM_ERROR_H_
#define SRC_FSMS_MAINFSM_ERROR_H_


#include "MainBase.h"
#include "../ErrorFSM/ErrorContext.h"

class Error : public MainBase{
	ErrorContext errorFSM;
public:
	Error(HAL& hal,dataClass& data);
	void entry(MainContext& ctx) override;
    void exit(MainContext& ctx) override;

};


#endif /* SRC_FSMS_MAINFSM_ERROR_H_ */
