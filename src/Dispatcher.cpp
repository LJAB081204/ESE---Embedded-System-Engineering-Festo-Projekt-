/*
 * Dispatcher.cpp
 *
 *  Created on: 06.05.2026
 *      Author: lewis
 */
#include "Dispatcher.h"
#include <iostream>
#include <unistd.h>
#include "PulseMessages.h"
using namespace std;

	Dispatcher::Dispatcher(const char* localName, const char* remoteName,HAL& hal)
		:hal(hal){
		// GNS Version

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

		// Lokale Version — eine Anlage, kein Qnet
		/*
		 if ((channelID = ChannelCreate(0)) == -1){
			 cout << "ChannelCreate failed" << endl;
		 }
		 localConnectionID = ConnectAttach(0, 0, channelID, _NTO_SIDE_CHANNEL, 0);
		 if (localConnectionID == -1){
			 cout << "ConnectAttach failed" << endl;
		 }
		 remoteConnectionID = -1;  // kein Remote
		 waitForMessages = true;
		 DispatcherThread = thread(&Dispatcher::run, this);
		*/

	}
	Dispatcher::~Dispatcher(){
		MsgSendPulse(localConnectionID,-1,PULSE_STOP_RUNNING,0);
		if(DispatcherThread.joinable()){
			DispatcherThread.join();
		}
		name_detach(attach,0);
		name_close(remoteConnectionID);
	}

	void Dispatcher::run(){
	    ThreadCtl(_NTO_TCTL_IO, 0);

		while(waitForMessages){
			//GLOBAL VERSION
			int rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), nullptr);
			//LOCAL VERSION
			//int rcvid = MsgReceive(channelID, &msg, sizeof(msg), nullptr);

			if (rcvid == 0){
				//Pulse
				switch(msg.code){
				    case PULSE_STOP_RUNNING:
				        waitForMessages = false;
				        break;

				    case PULSE_E_STOP:
				        cout << "ESTOP Pulse reached dispatcher" << endl;
				        if(msg.value.sival_int == 0){
				            hal.getActors()->motorForwards();
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_E_STOP_REMOTE, 0);
				        } else {
				            hal.getActors()->motorStop();
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_E_STOP_REMOTE, 1);
				        }
				        break;

				    case PULSE_E_STOP_REMOTE:
				        cout << "ESTOP Remote Pulse reached dispatcher" << endl;
				        if(msg.value.sival_int == 0){
				            hal.getActors()->motorForwards();
				        } else {
				            hal.getActors()->motorStop();
				        }
				        break;

				    case PULSE_START:
				        if(msg.value.sival_int == 1){
				            cout << "Start pressed!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_START_REMOTE, 1);
				        } else {
				            cout << "Start released!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_START_REMOTE, 0);
				        }
				        break;

				    case PULSE_START_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Start pressed on remote!" << endl;
				        } else {
				            cout << "Start released on remote!" << endl;
				        }
				        break;

				    case PULSE_RESET:
				        if(msg.value.sival_int == 1){
				            cout << "Reset pressed!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_RESET_REMOTE, 1);
				        } else {
				            cout << "Reset released!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_RESET_REMOTE, 0);
				        }
				        break;

				    case PULSE_RESET_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Reset pressed on remote!" << endl;
				        } else {
				            cout << "Reset released on remote!" << endl;
				        }
				        break;

				    case PULSE_STOP:
				        if(msg.value.sival_int == 1){
				            cout << "Stop pressed!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_STOP_REMOTE, 1);
				        } else {
				            cout << "Stop released!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_STOP_REMOTE, 0);
				        }
				        break;

				    case PULSE_STOP_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Stop pressed on remote!" << endl;
				        } else {
				            cout << "Stop released on remote!" << endl;
				        }
				        break;

				    case PULSE_ITEM_AT_ENTRY:
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

				    case PULSE_ITEM_AT_EXIT:
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

				    case PULSE_ITEM_IN_PUSHER:
				        if(msg.value.sival_int == 1){
				            cout << "Item in pusher!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_IN_PUSHER_REMOTE, 1);
				        } else {
				            cout << "Item left pusher!" << endl;
				            MsgSendPulse(remoteConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_IN_PUSHER_REMOTE, 0);
				        }
				        break;

				    case PULSE_ITEM_IN_PUSHER_REMOTE:
				        if(msg.value.sival_int == 1){
				            cout << "Item in pusher on remote!" << endl;
				        } else {
				            cout << "Item left pusher on remote!" << endl;
				        }
				        break;

				    case PULSE_ITEM_METAL:
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

				    case PULSE_PUSHER_OPEN:
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

				    case PULSE_SLIDE_FULL:
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





