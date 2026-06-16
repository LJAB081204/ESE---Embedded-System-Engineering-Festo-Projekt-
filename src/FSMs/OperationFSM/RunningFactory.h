/*
 * RunningFactory.h
 *
 *  Created on: 11.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGFACTORY_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGFACTORY_H_


class OperationBase;
class HAL;
class dataClass;
class TimerHandler;

class RunningFactory {
public:
	virtual ~RunningFactory() = default;
	virtual OperationBase* createRunning(HAL& hal,dataClass& data,TimerHandler& timerHandler)=0;
};


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGFACTORY_H_ */
