#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_

#include <thread>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include "HAL.h"
#include "PulseMessages.h"


class Dispatcher {
private:
    name_attach_t* attach;
    int localConnectionID;
    int remoteConnectionID;

    int channelID;

    bool waitForMessages;
    std::thread DispatcherThread;
    _pulse msg;

public:
    Dispatcher(const char* localName, const char* remoteName);
    ~Dispatcher();
    void run();
    int getChannelID();
    int getConnectionID();
};

#endif /* SRC_DISPATCHER_H_ */
