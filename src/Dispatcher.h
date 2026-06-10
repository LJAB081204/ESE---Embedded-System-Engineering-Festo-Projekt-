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



class Dispatcher {
private:
	int ownConnectionID;
    int conCtrlConnectionID;
    std::chrono::steady_clock::time_point buttonPressStart;
    HAL& hal;
    dataClass& data;
    MainContext FSM;

    int ownChannelID;



    bool waitForMessages;
    std::thread DispatcherThread;
    _pulse msg;
    void run();
public:
    Dispatcher(int conCtrlConnectionID, HAL& hal, dataClass& data);
    ~Dispatcher();
    int getConnectionID();
};
#endif /* SRC_DISPATCHER_H_ */
