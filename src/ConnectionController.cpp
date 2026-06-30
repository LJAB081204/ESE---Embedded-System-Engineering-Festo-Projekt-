#include <iostream>
#include <thread>
#include <atomic>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>

#include "ConnectionController.h"
#include "PulseMessages.h"

#define PULSE_HEARTBEAT_PING _PULSE_CODE_MINAVAIL + 90
#define PULSE_HEARTBEAT_TIMER_EXPIRED _PULSE_CODE_MINAVAIL + 91

using namespace std;

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------
ConnectionController::ConnectionController(
    const char* localName, const char* remoteName)
    : remoteName(remoteName)
{
	// Heartbeat-Namen ableiten
	    std::string localHbStr  = std::string(localName)  + "_hb";
	    std::string remoteHbStr = std::string(remoteName) + "_hb";
	    const char* localHbName  = localHbStr.c_str();
	    remoteHbName = remoteHbStr.c_str();

    // --- normaler Channel  ---
    remoteIn = name_attach(NULL, localName, NAME_FLAG_ATTACH_GLOBAL);
    if (!remoteIn) { cout << "name_attach failed" << endl; return; }

    dispatcherInChid = ChannelCreate(0);
    wakeDispatcherCoid = dispatcherInCoid;
    dispatcherInCoid = ConnectAttach(0, 0, dispatcherInChid, _NTO_SIDE_CHANNEL, 0);
    wakeDispatcherCoid = dispatcherInCoid;
    wakeRemoteCoid   = ConnectAttach(0, 0, remoteIn->chid,   _NTO_SIDE_CHANNEL, 0);

    tRemote = thread(&ConnectionController::runReceiveFromRemote, this);

    remoteOutCoid = -1;
    while ((remoteOutCoid = name_open(remoteName, NAME_FLAG_ATTACH_GLOBAL)) == -1) {
        sleep(1);
        cout << "Verbindung wird aufgebaut..." << endl;
    }
    connected = true;
    cout << "Verbindung hergestellt."<< endl;

    // --- Heartbeat-Channel ---
    heartbeatIn = name_attach(NULL, localHbName, NAME_FLAG_ATTACH_GLOBAL);
    if (!heartbeatIn) { cout << "heartbeat name_attach failed" << endl; return; }

    wakeHeartbeatCoid = ConnectAttach(0, 0, heartbeatIn->chid, _NTO_SIDE_CHANNEL, 0);

    tHeartbeatReceive = thread(&ConnectionController::runReceiveFromHeartbeat, this);

    heartbeatOutCoid = -1;
    while ((heartbeatOutCoid = name_open(remoteHbName, NAME_FLAG_ATTACH_GLOBAL)) == -1) {
        sleep(1);
//        cout << "trying to connect heartbeat..." << endl;
    }
//    cout << "heartbeat connection established, coid: " << heartbeatOutCoid << endl;
}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------
ConnectionController::~ConnectionController()
{
    running = false;

    MsgSendPulse(wakeRemoteCoid,    -1, PULSE_STOP_RECEIVING, 0);
    MsgSendPulse(wakeDispatcherCoid,-1, PULSE_STOP_RECEIVING, 0);
    MsgSendPulse(wakeHeartbeatCoid, -1, PULSE_STOP_RECEIVING, 0);

    if (tRemote.joinable())          tRemote.join();
    if (tDispatcher.joinable())      tDispatcher.join();
    if (tHeartbeat.joinable())       tHeartbeat.join();
    if (tHeartbeatReceive.joinable()) tHeartbeatReceive.join();

    name_detach(remoteIn, 0);
    name_detach(heartbeatIn, 0);
    name_close(remoteOutCoid);
    name_close(heartbeatOutCoid);
}

// ------------------------------------------------------------
// Start threads
// ------------------------------------------------------------
void ConnectionController::start(int dispatcherOut){
    dispatcherOutCoid = dispatcherOut;
    tDispatcher = thread(&ConnectionController::runReceiveFromDisp, this);
    tHeartbeat = thread(&ConnectionController::runHeartbeat, this);
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
            //if (connected){
            	MsgSendPulse(remoteOutCoid, SIGEV_PULSE_PRIO_INHERIT, msgFromDispatcher.code, msgFromDispatcher.value.sival_int);
            //}
        }
        else{
            MsgReply(rcvid, 0, nullptr, 0);
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

        if (rcvid == -1) continue;

        if (rcvid == 0)
        {
            if (msgFromRemote.code == PULSE_STOP_RECEIVING) { running = false; break; }

            MsgSendPulse(dispatcherOutCoid, SIGEV_PULSE_PRIO_INHERIT,
                         msgFromRemote.code, msgFromRemote.value.sival_int);
        }
        else { MsgReply(rcvid, 0, nullptr, 0); }
    }
}

void ConnectionController::runReceiveFromHeartbeat()
{
    SimpleTimer heartbeatTimer(wakeHeartbeatCoid, PULSE_HEARTBEAT_TIMER_EXPIRED);
    heartbeatTimer.start(100000);

    _pulse msg;
    while (running)
    {
        int rcvid = MsgReceive(heartbeatIn->chid, &msg, sizeof(msg), nullptr);

        if (rcvid == -1) continue;

        if (rcvid == 0)
        {
            if (msg.code == PULSE_STOP_RECEIVING) { running = false; break; }

            else if (msg.code == PULSE_HEARTBEAT_PING)
            {
                if (!connected)
                {
                    connected = true;
                    MsgSendPulse(dispatcherOutCoid, SIGEV_PULSE_PRIO_INHERIT,
                                 PULSE_CONNECTION_RESTORED, 0);
                    cout << "Connection RESTORED!" << endl;
                }
                heartbeatTimer.stop();
                heartbeatTimer.start(heartbeatTimeoutMs);
            }
            else if (msg.code == PULSE_HEARTBEAT_TIMER_EXPIRED)
            {
                if (connected)
                {
                    connected = false;
                    MsgSendPulse(dispatcherOutCoid, SIGEV_PULSE_PRIO_INHERIT,
                                 PULSE_CONNECTION_LOST, 0);
                    cout << "Connection LOST!" << endl;
                }
            }
        }
        else { MsgReply(rcvid, 0, nullptr, 0); }
    }
}

// ------------------------------------------------------------
// Ping senden
// ------------------------------------------------------------
void ConnectionController::runHeartbeat()
{
    while (running)
    {
        if (!connected)
        {
//            if (heartbeatOutCoid != -1)
//            {
//                name_close(heartbeatOutCoid);
//                heartbeatOutCoid = -1;
//            }
//            int coid = name_open(remoteHbName, NAME_FLAG_ATTACH_GLOBAL);
//            if (coid != -1)
//            {
//                heartbeatOutCoid = coid;
//                cout << "Heartbeat reconnected, coid: " << heartbeatOutCoid << endl;
//            }
//            else
//            {
//                cout << "Heartbeat reconnect failed, retrying..." << endl;
//            }
            this_thread::sleep_for(chrono::milliseconds(1000));
            continue;
        }

        MsgSendPulse(heartbeatOutCoid, SIGEV_PULSE_PRIO_INHERIT, PULSE_HEARTBEAT_PING, 0);
        this_thread::sleep_for(chrono::milliseconds(heartbeatIntervalMs));
    }
}

// ------------------------------------------------------------
// Getter for Dispatcher connection
// ------------------------------------------------------------
int ConnectionController::getConnectionID()
{
    return dispatcherInCoid;
}
