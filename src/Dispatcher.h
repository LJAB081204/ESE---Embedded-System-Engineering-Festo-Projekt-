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
    int localConnectionID;
    int remoteConnectionID;
    std::chrono::steady_clock::time_point buttonPressStart;
    HAL& hal;
    dataClass& data;
    MainContext FSM;

#ifndef LOCAL_VERSION
    //GLOBAL VERSION
    name_attach_t* attach;
#else
    //LOCAL VERSION
    int channelID;
#endif


    bool waitForMessages;
    std::thread DispatcherThread;
    _pulse msg;
    void run();
public:
    Dispatcher(const char* localName, const char* remoteName,HAL& hal,dataClass& data);
    ~Dispatcher();
    int getConnectionID();
};
#endif /* SRC_DISPATCHER_H_ */
