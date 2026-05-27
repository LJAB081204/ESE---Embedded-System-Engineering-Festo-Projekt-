#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_

#include <thread>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include "HAL.h"


class Dispatcher {
private:
    name_attach_t* attach;
    int localConnectionID;
    int remoteConnectionID;
    HAL& hal;

    //LOCAL VERSION
   // int channelID;



    bool waitForMessages;
    std::thread DispatcherThread;
    _pulse msg;
    void run();
public:
    Dispatcher(const char* localName, const char* remoteName,HAL& hal);
    ~Dispatcher();
    int getConnectionID();
};
#endif /* SRC_DISPATCHER_H_ */
