/*
 * Dispatcher.cpp
 *
 *  Created on: 06.05.2026
 *      Author: lewis
 */
#include "Dispatcher.h"
using namespace std;

Dispatcher::Dispatcher(const char* localName, const char* remoteName){

    // GNS Version (auskommentiert bis Qnet läuft)
    /*
    attach = name_attach(NULL, localName, NAME_FLAG_ATTACH_GLOBAL);
    if (attach == NULL){
        cout << "name_attach failed" << endl;
    }
    while ((remoteConnectionID = name_open(remoteName, NAME_FLAG_ATTACH_GLOBAL)) == -1) {
        sleep(1);
    }
    localConnectionID = ConnectAttach(0, 0, attach->chid, _NTO_SIDE_CHANNEL, 0);
    */

    // Lokale Version ohne GNS
    if ((channelID = ChannelCreate(0)) == -1) {
        cout << "ChannelCreate failed" << endl;
    }
    localConnectionID = ConnectAttach(0, 0, channelID, _NTO_SIDE_CHANNEL, 0);
    if (localConnectionID == -1) {
        cout << "ConnectAttach failed" << endl;
    }

    waitForMessages = true;
    DispatcherThread = thread(&Dispatcher::run, this);
}

	Dispatcher::~Dispatcher(){
		MsgSendPulse(localConnectionID,-1,PULSE_STOP_RUNNING,0);
		DispatcherThread.join();
		name_detach(attach,0);
		name_close(remoteConnectionID);
	}

	void Dispatcher::run(){
		HAL hal;
		while(waitForMessages){
			//int rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), nullptr);
			int rcvid = MsgReceive(channelID, &msg, sizeof(msg), nullptr);

			if (rcvid == 0){
				//Pulse
				switch(msg.code){
					case PULSE_STOP_RUNNING:
						waitForMessages = false;
						break;
					case PULSE_E_STOP:
						cout << "ESTOP Pulse reached dispatcher";
						if(msg.value.sival_int == 0){
							hal.getActors()->motorForwards();
						}
						else{
							hal.getActors()->motorStop();
						}
						break;
					case PULSE_START:
					    if(msg.value.sival_int == 1){
					        cout << "Start pressed!" << endl;
					    } else {
					        cout << "Start released!" << endl;
					    }
					    break;

					case PULSE_RESET:
					    if(msg.value.sival_int == 1){
					        cout << "Reset pressed!" << endl;
					    } else {
					        cout << "Reset released!" << endl;
					    }
					    break;

					case PULSE_STOP:
					    if(msg.value.sival_int == 1){
					        cout << "Stop pressed!" << endl;
					    } else {
					        cout << "Stop released!" << endl;
					    }
					    break;

					case PULSE_ITEM_AT_ENTRY:
					    if(msg.value.sival_int == 1){
					        cout << "Item at entry!" << endl;
					    } else {
					        cout << "Item left entry!" << endl;
					    }
					    break;

					case PULSE_ITEM_AT_EXIT:
					    if(msg.value.sival_int == 1){
					        cout << "Item at exit!" << endl;
					    } else {
					        cout << "Item left exit!" << endl;
					    }
					    break;

					case PULSE_ITEM_IN_PUSHER:
					    if(msg.value.sival_int == 1){
					        cout << "Item in pusher!" << endl;
					    } else {
					        cout << "Item left pusher!" << endl;
					    }
					    break;

					case PULSE_ITEM_METAL:
					    if(msg.value.sival_int == 1){
					        cout << "Item is metal!" << endl;
					    } else {
					        cout << "Item is not metal!" << endl;
					    }
					    break;

					case PULSE_PUSHER_OPEN:
					    if(msg.value.sival_int == 1){
					        cout << "Pusher is open!" << endl;
					    } else {
					        cout << "Pusher is closed!" << endl;
					    }
					    break;

					case PULSE_SLIDE_FULL:
					    if(msg.value.sival_int == 1){
					        cout << "Slide is full!" << endl;
					    } else {
					        cout << "Slide cleared!" << endl;
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






