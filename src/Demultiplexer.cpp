/*
 * Demultiplexer.cpp
 *
 *  Created on: 30.04.2026
 *      Author: tayla test test
 */
#include "Demultiplexer.h"
#include "HAL.h"
#include "PulseMessages.h"
#include <sys/mman.h>
#include <hw/inout.h>
#include <iostream>
#include <sys/neutrino.h>
using namespace std;

	Demultiplexer::Demultiplexer(int DispatcherConnectionID){
		gpio_bank_0 = mmap_device_io(GPIO_REGISTER_LENGHT, (uint64_t) GPIO_PORT0);
		if((channelID = ChannelCreate(0)) == -1){
			std::cout << "creating channel failed" << std::endl;

		}
		if((connectionID = ConnectAttach(0,0,channelID,_NTO_SIDE_CHANNEL,0)) == -1){
		std::cout << "Connecting to channel failed" << std::endl;
		}
		waitForMessages = true;
		dispatcherID = DispatcherConnectionID;
		receivingThread = thread(&Demultiplexer::run,this,channelID);

	}

	Demultiplexer::~Demultiplexer(){
		MsgSendPulse(connectionID,-1,1,0);
		if(receivingThread.joinable()){
			receivingThread.join();
		}
		ConnectDetach_r(connectionID);
		ChannelDestroy(channelID);
		munmap_device_io(gpio_bank_0,GPIO_REGISTER_LENGHT);
	}

	void Demultiplexer::run(int channelID){
		ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.
		_pulse msg;
		while(waitForMessages){
			//std::cout << "Waiting for pulse..." << std::endl;

		    int recvid = MsgReceivePulse(channelID, &msg, sizeof(_pulse), nullptr);

		    	//std::cout << "Pulse received: " << msg.code << std::endl;
				switch(msg.code){

				case PULSE_BTN_ESTOP_LOCAL:
				    if(in32(gpio_bank_0 + GPIO_DATAIN) & ESTOP) {
				        cout << "E-Stop released!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_BTN_ESTOP_LOCAL,0);
				    } else {
				        cout << "E-Stop pressed!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_BTN_ESTOP_LOCAL,1);
				    }
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),ESTOP);
				    InterruptUnmask((GPIO_0_IRQ_BASE+27),msg.value.sival_int);
				    break;

				case PULSE_BTN_RESET_LOCAL:
				    if(in32(gpio_bank_0 + GPIO_DATAIN) & BUTTON_RESET) {
				        cout << "Reset pressed!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_BTN_RESET_LOCAL,1);
				    } else {
				        cout << "Reset released!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_BTN_RESET_LOCAL,0);
				    }
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),BUTTON_RESET);
				    InterruptUnmask((GPIO_0_IRQ_BASE+26),msg.value.sival_int);
				    break;

				case PULSE_BTN_STOP_LOCAL:
				    if(in32(gpio_bank_0 + GPIO_DATAIN) & BUTTON_STOP) {
				        cout << "Stop released!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_BTN_STOP_LOCAL,0);
				    } else {
				        cout << "Stop pressed!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_BTN_STOP_LOCAL,1);
				    }
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),BUTTON_STOP);
				    InterruptUnmask((GPIO_0_IRQ_BASE+23),msg.value.sival_int);
				    break;
				case PULSE_BTN_START_LOCAL:
				    if(in32(gpio_bank_0 + GPIO_DATAIN) & BUTTON_START) {
				        cout << "Start pressed!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_BTN_START_LOCAL,1);
				    } else {
				        cout << "Start released!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_BTN_START_LOCAL,0);
				    }
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),BUTTON_START);
				    InterruptUnmask((GPIO_0_IRQ_BASE+22),msg.value.sival_int);
				    break;
				case PULSE_ITEM_AT_ENTRY_LOCAL:
				    if(in32(gpio_bank_0 + GPIO_DATAIN) & ITEM_AT_ENTRY) {
				        cout << "Item at entry cleared!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_ITEM_AT_ENTRY_LOCAL,0);
				    } else {
				        cout << "Item at entry!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_ITEM_AT_ENTRY_LOCAL,1);
				    }
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),ITEM_AT_ENTRY);
				    InterruptUnmask((GPIO_0_IRQ_BASE+2),msg.value.sival_int);
				    break;
				case PULSE_ITEM_AT_EXIT_LOCAL:
				    if(in32(gpio_bank_0 + GPIO_DATAIN) & ITEM_AT_EXIT) {
				        cout << "Item at exit cleared!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_ITEM_AT_EXIT_LOCAL,0);
				    } else {
				        cout << "Item at exit!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_ITEM_AT_EXIT_LOCAL,1);
				    }
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),ITEM_AT_EXIT);
				    InterruptUnmask((GPIO_0_IRQ_BASE+20),msg.value.sival_int);
				    break;

				case PULSE_ITEM_AT_PUSHER_LOCAL:
				    if(in32(gpio_bank_0 + GPIO_DATAIN) & ITEM_IN_PUSHER) {
				        cout << "Item left pusher!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_ITEM_AT_PUSHER_LOCAL,0);
				    } else {
				        cout << "Item in pusher!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_ITEM_AT_PUSHER_LOCAL,1);
				    }
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), ITEM_IN_PUSHER);
				    InterruptUnmask((GPIO_0_IRQ_BASE+5), msg.value.sival_int);
				    break;
				case PULSE_ITEM_METAL_LOCAL:
				    if(in32(gpio_bank_0 + GPIO_DATAIN) & ITEM_METAL) {
				        cout << "Item is metal!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_ITEM_METAL_LOCAL,1);
				    } else {
				        cout << "Item is not metal!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_ITEM_METAL_LOCAL,0);

				    }
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), ITEM_METAL);
				    InterruptUnmask((GPIO_0_IRQ_BASE+7), msg.value.sival_int);
				    break;
				case PULSE_PUSHER_OPEN_LOCAL:
				    if(in32(gpio_bank_0 + GPIO_DATAIN) & PUSHER_OPEN) {
				        cout << "Pusher is open!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_PUSHER_OPEN_LOCAL,1);
				    } else {
				        cout << "Pusher is closed!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_PUSHER_OPEN_LOCAL,0);
				    }
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), PUSHER_OPEN);
				    InterruptUnmask((GPIO_0_IRQ_BASE+14), msg.value.sival_int);
				    break;

				case PULSE_SLIDE_FULL_LOCAL:
				    if(in32(gpio_bank_0 + GPIO_DATAIN) & SLIDE_FULL) {
				        cout << "Slide cleared!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_SLIDE_FULL_LOCAL,0);
				    } else {
				        cout << "Slide is full!" << endl;
				        MsgSendPulse(dispatcherID,-1,PULSE_SLIDE_FULL_LOCAL,1);
				    }
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), SLIDE_FULL);
				    InterruptUnmask((GPIO_0_IRQ_BASE+15), msg.value.sival_int);
				    break;
				case PULSE_STOP_RECEIVING:
				    waitForMessages = false;
				    break;

		    }

		}
	}

	int Demultiplexer::getChannelID(){
		return channelID;
	}
	int Demultiplexer::getConnectionID(){
		return connectionID;
	}





