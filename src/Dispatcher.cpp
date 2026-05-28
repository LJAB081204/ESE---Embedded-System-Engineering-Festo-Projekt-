/*
 * Dispatcher.cpp
 *
 *  Created on: 06.05.2026
 *      Author: lewis
 */
#include "Dispatcher.h"
#include "HeightSensor.h"
#include "PulseMessages.h"
#include "Config.h"
#include <sys/dispatch.h>
using namespace std;

Dispatcher::Dispatcher(const char* localName, const char* remoteName, HAL& hal)
    : hal(hal) {
#ifndef LOCAL_VERSION
    //GNS Version
    attach = name_attach(NULL, localName, NAME_FLAG_ATTACH_GLOBAL);
    if (attach == NULL){
        cout << "name_attach failed" << endl;
        return;
    }
    waitForMessages = true;
    DispatcherThread = thread(&Dispatcher::run, this);
    remoteConnectionID = -1;
    while ((remoteConnectionID = name_open(remoteName, NAME_FLAG_ATTACH_GLOBAL)) == -1) {
        sleep(1);
        cout << "trying to connect" << endl;
    }
    cout << "connection established" << endl;
    localConnectionID = ConnectAttach(0,0,attach->chid,_NTO_SIDE_CHANNEL,0);
    if(localConnectionID == -1){
        cout << "connectAttach failed" << endl;
    }
#else
    // Lokale Version — eine Anlage, kein Qnet
    if ((channelID = ChannelCreate(0)) == -1){
        cout << "ChannelCreate failed" << endl;
    }
    localConnectionID = ConnectAttach(0, 0, channelID, _NTO_SIDE_CHANNEL, 0);
    if (localConnectionID == -1){
        cout << "ConnectAttach failed" << endl;
    }
    remoteConnectionID = -1;
    waitForMessages = true;
    DispatcherThread = thread(&Dispatcher::run, this);
#endif

}
	Dispatcher::~Dispatcher(){
		MsgSendPulse(localConnectionID,-1,PULSE_STOP_RECEIVING,0);
		DispatcherThread.join();
#ifndef LOCAL_VERSION
		name_detach(attach,0);
		name_close(remoteConnectionID);
#else
		ConnectDetach(localConnectionID);
		ChannelDestroy(channelID);
#endif
	}

	void Dispatcher::run(){
	    ThreadCtl(_NTO_TCTL_IO, 0);

		while(waitForMessages){
			#ifndef LOCAL_VERSION
			//GLOBAL VERSION
			int rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), nullptr);
			#else
			//LOCAL VERSION
			int rcvid = MsgReceive(channelID, &msg, sizeof(msg), nullptr);
			#endif
			if (rcvid == 0){
				//Pulse
				switch(msg.code){
				    case PULSE_STOP_RECEIVING:
				        waitForMessages = false;
				        break;

				    case PULSE_BTN_ESTOP_LOCAL:
				        cout << "ESTOP Pulse reached dispatcher" << endl;
				        if(msg.value.sival_int == 0){
				            hal.getActors()->motorForwards();
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_ESTOP_REMOTE, 0);
				        } else {
				            hal.getActors()->motorStop();
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_ESTOP_REMOTE, 1);
				        }
				        break;

				    case PULSE_BTN_ESTOP_REMOTE:
				        cout << "ESTOP Remote Pulse reached dispatcher" << endl;
				        if(msg.value.sival_int == 0){
				            hal.getActors()->motorForwards();
				        } else {
				            hal.getActors()->motorStop();
				        }
				        break;

				    case PULSE_BTN_START_LOCAL:
				        if(msg.value.sival_int == 1){
				            cout << "Start pressed!" << endl;
				            //Zeitstamp 1
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_START_REMOTE, 1);
				        } else {
				            cout << "Start released!" << endl;
				            //diff = Zeitstamp 2 - Zeitstamp 1
				            //je nach diff long oder short press
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_START_REMOTE, 0);
				        }
				        break;

				    case PULSE_BTN_START_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Start pressed on remote!" << endl;
				        } else {
				            cout << "Start released on remote!" << endl;
				        }
				        break;

				    case PULSE_BTN_RESET_LOCAL:
				        if(msg.value.sival_int == 1){
				            cout << "Reset pressed!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_RESET_REMOTE, 1);
				        } else {
				            cout << "Reset released!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_RESET_REMOTE, 0);
				        }
				        break;

				    case PULSE_BTN_RESET_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Reset pressed on remote!" << endl;
				        } else {
				            cout << "Reset released on remote!" << endl;
				        }
				        break;

				    case PULSE_BTN_STOP_LOCAL:
				        if(msg.value.sival_int == 1){
				            cout << "Stop pressed!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_STOP_REMOTE, 1);
				        } else {
				            cout << "Stop released!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_STOP_REMOTE, 0);
				        }
				        break;

				    case PULSE_BTN_STOP_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Stop pressed on remote!" << endl;
				        } else {
				            cout << "Stop released on remote!" << endl;
				        }
				        break;

				    case PULSE_ITEM_AT_ENTRY_LOCAL:
				        if(msg.value.sival_int == 1){
				            cout << "Item at entry!" << endl;
				            hal.getActors()->motorForwards();
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_ENTRY_REMOTE, 1);
				        } else {
				            cout << "Item left entry!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_ENTRY_REMOTE, 0);
				        }
				        break;

				    case PULSE_ITEM_AT_ENTRY_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Item at entry on remote!" << endl;
				        } else {
				            cout << "Item left entry on remote!" << endl;
				        }
				        break;

				    case PULSE_ITEM_AT_EXIT_LOCAL:
				        if(msg.value.sival_int == 1){
				            cout << "Item at exit!" << endl;
				            hal.getActors()->motorStop();
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_EXIT_REMOTE, 1);
				        } else {
				            cout << "Item left exit!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_EXIT_REMOTE, 0);
				        }
				        break;

				    case PULSE_ITEM_AT_EXIT_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Item at exit on remote!" << endl;
				        } else {
				            cout << "Item left exit on remote!" << endl;
				        }
				        break;

				    case PULSE_ITEM_AT_PUSHER_LOCAL:
				        if(msg.value.sival_int == 1){
				            cout << "Item in pusher!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_PUSHER_REMOTE, 1);
				        } else {
				            cout << "Item left pusher!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_PUSHER_REMOTE, 0);
				        }
				        break;

				    case PULSE_ITEM_AT_PUSHER_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Item in pusher on remote!" << endl;
				        } else {
				            cout << "Item left pusher on remote!" << endl;
				        }
				        break;

				    case PULSE_ITEM_METAL_LOCAL:
				        if(msg.value.sival_int == 1){
				            cout << "Item is metal!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_METAL_REMOTE, 1);
				        } else {
				            cout << "Item is not metal!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_METAL_REMOTE, 0);
				        }
				        break;

				    case PULSE_ITEM_METAL_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Item is metal on remote!" << endl;
				        } else {
				            cout << "Item is not metal on remote!" << endl;
				        }
				        break;

				    case PULSE_PUSHER_OPEN_LOCAL:
				        if(msg.value.sival_int == 1){
				            cout << "Pusher is open!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_PUSHER_OPEN_REMOTE, 1);
				        } else {
				            cout << "Pusher is closed!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_PUSHER_OPEN_REMOTE, 0);
				        }
				        break;

				    case PULSE_PUSHER_OPEN_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Pusher is open on remote!" << endl;
				        } else {
				            cout << "Pusher is closed on remote!" << endl;
				        }
				        break;

				    case PULSE_SLIDE_FULL_LOCAL:
				        if(msg.value.sival_int == 1){
				            cout << "Slide is full!" << endl;
				            hal.getActors()->yellowLampOn();
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_SLIDE_FULL_REMOTE, 1);
				        } else {
				            cout << "Slide cleared!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_SLIDE_FULL_REMOTE, 0);
				        }
				        break;

				    case PULSE_SLIDE_FULL_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Slide is full on remote!" << endl;
				        } else {
				            cout << "Slide cleared on remote!" << endl;
				        }
				        break;

				    case PULSE_NEW_HEIGHT_LOCAL:
				    	cout << "Height change recevied" << endl;
				    	switch(msg.value.sival_int){
				    	case HEIGHT_CONVEYOR: // Height Conveyer
			            	std::cout << "HEIGHT_CONVEYOR" << std::endl;
			            	break;
				    	case HEIGHT_DRILLING:
			            	std::cout << "HEIGHT_DRILLING" << std::endl;
			            	break;
				    	case HEIGHT_WORKPIECE_HIGH:
			            	std::cout << "HEIGHT_WORKPIECE_HIGH" << std::endl;
			            	break;
				    	case HEIGHT_WORKPIECE_FLAT:
			            	std::cout << "HEIGHT_WORKPIECE_FLAT" << std::endl;
			            	break;
				    	case HEIGHT_TOO_HIGH:
			            	std::cout << "HEIGHT_TOO_HIGH" << std::endl;
			            	break;
				    	default: break;
				    	}
					case PULSE_TIMER_EXPIRED:
							cout << "Timer " << msg.value.sival_int << " abgelaufen" << endl;
						break;
				}
			}
			else{
				//Software Message
				MsgReply(rcvid,0,nullptr,0);
			}
		}

	}

	int Dispatcher::getConnectionID(){
		return localConnectionID;
	}






