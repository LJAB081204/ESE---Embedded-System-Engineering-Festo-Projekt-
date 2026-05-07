/*
 * Dispatcher.cpp
 *
 *  Created on: 06.05.2026
 *      Author: lewis
 */
#include "Dispatcher.h"
using namespace std;

Dispatcher::Dispatcher(const std::string& name){
	if((channelID = ChannelCreate(0)) == -1){
				std::cout << "creating channel failed" << std::endl;
			}
	if((connectionID = ConnectAttach(0,0,channelID,_NTO_SIDE_CHANNEL,0)) == -1){
			std::cout << "Connecting to channel failed" << std::endl;
			}
	waitForMessages = true;
	receivingThread = thread(&Dispatcher::run,this);
}

Dispatcher::~Dispatcher(){
	MsgSendPulse(connectionID,-1,1,0);
	receivingThread.join();
	ConnectDetach_r(connectionID);
	ChannelDestroy(channelID);
}

void Dispatcher::run(){
	while(waitForMessages){
		int rcvid = MsgReceive(channelID, &msg, sizeof(msg), nullptr);
		if (rcvid == 0){
			//Pulse
			switch(msg.code){

			}
		}
		else{
			//Software Message
			MsgReply(rcvid,0,nullptr,0);
		}
	}

}





