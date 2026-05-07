#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_

#include <thread>
#include <iostream>
#include <sys/neutrino.h>
#include "HAL.h"
#include "PulseMessages.h"

struct DispatcherMsg {
    struct _pulse pulse;
    int type;
    int data;
};

class Dispatcher {
private:
    int channelID;
    int connectionID;
    bool waitForMessages;
    std::thread receivingThread;
    _pulse msg;

public:
    Dispatcher(const std::string& name);
    ~Dispatcher();
    void run();
    int getChannelID();
    int getConnectionID();
};

#endif /* SRC_DISPATCHER_H_ */
