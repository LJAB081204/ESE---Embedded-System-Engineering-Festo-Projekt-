#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_

#include <thread>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <chrono>
#include "Config.h"
#include "dataClass.h"
#include "HAL.h"
#include "FSMs/MainFSM/MainContext.h"
#include "HeightSensor.h"
#include "FSMs/OperationFSM/RunningFactory.h"



class Dispatcher {
private:
	int ownConnectionID;
    int conCtrlConnectionID;

    HAL& hal;
    dataClass& data;
    RunningFactory& factory;

    std::chrono::steady_clock::time_point buttonPressStart;
	MainContext FSM;
    int ownChannelID;


    bool itemAtExit;
    bool waitForMessages;
    std::thread DispatcherThread;
    _pulse msg;
    void run();
public:
    Dispatcher(int conCtrlConnectionID, HAL& hal, dataClass& data, HeightSensor& hs,RunningFactory& factory);
    ~Dispatcher();
    int getConnectionID();
};
#endif /* SRC_DISPATCHER_H_ */
