#include <iostream>
#include <thread>
#include <atomic>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>

#include "ConnectionController.h"
#include "PulseMessages.h"

using namespace std;

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------
ConnectionController::ConnectionController(const char* localName,
                                           const char* remoteName)
{
    cout << "ConCon created!" << endl;

    // Remote INPUT
    remoteIn = name_attach(NULL, localName, NAME_FLAG_ATTACH_GLOBAL);
    if (!remoteIn) { cout << "name_attach failed" << endl; return; }
    cout << "name_attach success, chid: " << remoteIn->chid << endl;

    wakeRemoteCoid = ConnectAttach(0, 0, remoteIn->chid, _NTO_SIDE_CHANNEL, 0);
    if (wakeRemoteCoid == -1) cout << "wakeRemoteCoid failed" << endl;

    // Dispatcher INPUT channel
    dispatcherInChid = ChannelCreate(0);
    cout << "ChannelCreate: " << dispatcherInChid << endl;
    if (dispatcherInChid == -1) cout << "ChannelCreate failed" << endl;

    wakeDispatcherCoid = ConnectAttach(0, 0, dispatcherInChid, _NTO_SIDE_CHANNEL, 0);
    if (wakeDispatcherCoid == -1) cout << "wakeDispatcherCoid failed" << endl;

    cout << "starting name_open for: " << remoteName << endl;
	remoteOutCoid = -1;
	while ((remoteOutCoid = name_open(remoteName, NAME_FLAG_ATTACH_GLOBAL)) == -1) {
		sleep(1);
		cout << "trying to connect..." << endl;
	}
	cout << "connection established, coid: " << remoteOutCoid << endl;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------
ConnectionController::~ConnectionController()
{
    running = false;

    // Wake both blocking MsgReceive calls
    MsgSendPulse(wakeRemoteCoid, -1, PULSE_STOP_RECEIVING, 0);
    MsgSendPulse(wakeDispatcherCoid, -1, PULSE_STOP_RECEIVING, 0);

    if (tRemote.joinable())
        tRemote.join();

    if (tDispatcher.joinable())
        tDispatcher.join();

    name_detach(remoteIn, 0);
    name_close(remoteOutCoid);
}

// ------------------------------------------------------------
// Start threads
// ------------------------------------------------------------
void ConnectionController::start(int dispatcherOut)
{
    dispatcherOutCoid = dispatcherOut;

    tRemote    = thread(&ConnectionController::runReceiveFromRemote, this);
    tDispatcher = thread(&ConnectionController::runReceiveFromDisp, this);
}

// ------------------------------------------------------------
// Dispatcher -> Remote
// ------------------------------------------------------------
void ConnectionController::runReceiveFromDisp()
{
    while (running){
        int rcvid = MsgReceive(dispatcherInChid, &msgFromDispatcher, sizeof(msgFromDispatcher), nullptr);

        if (rcvid == -1){
            continue;
        }

        if (rcvid == 0)
        {
            if (msgFromDispatcher.code == PULSE_STOP_RECEIVING)
            {
                running = false;
                break;
            }

            MsgSendPulse(remoteOutCoid, SIGEV_PULSE_PRIO_INHERIT, msgFromDispatcher.code, msgFromDispatcher.value.sival_int);
        }
    }
}

// ------------------------------------------------------------
// Remote -> Dispatcher
// ------------------------------------------------------------
void ConnectionController::runReceiveFromRemote()
{
    while (running)
    {
        int rcvid = MsgReceive(remoteIn->chid, &msgFromRemote, sizeof(msgFromRemote), nullptr);

        if (rcvid == -1){
            continue;
        }

        if (rcvid == 0)
        {
            if (msgFromRemote.code == PULSE_STOP_RECEIVING)
            {
                running = false;
                break;
            }

            MsgSendPulse( dispatcherOutCoid, SIGEV_PULSE_PRIO_INHERIT, msgFromRemote.code, msgFromRemote.value.sival_int);
        }
    }
}

// ------------------------------------------------------------
// Getter for Dispatcher connection
// ------------------------------------------------------------
int ConnectionController::getConnectionID()
{
    return dispatcherInChid;
}
