/*
 * Demultiplexer.cpp
 *
 *  Created on: 30.04.2026
 *      Author: tayla
 */
#include "Demultiplexer.h"
#include "InterruptHandler.h"
#include "HAL.h"
using namespace std;

	Demultiplexer::Demultiplexer(const std::string& name){
		gpio_bank_0 = mmap_device_io(GPIO_REGISTER_LENGHT, (uint64_t) GPIO_PORT0);
		if((channelID = ChannelCreate(0)) == -1){
			std::cout << "creating channel failed" << std::endl;

		}
		if((connectionID = ConnectAttach(0,0,channelID,_NTO_SIDE_CHANNEL,0)) == -1){
		std::cout << "Connecting to channel failed" << std::endl;
		}
		waitForMessages = true;
		receivingThread = thread(&Demultiplexer::run,this,channelID);

	}

	Demultiplexer::~Demultiplexer(){
		MsgSendPulse(connectionID,-1,1,0);
		receivingThread.join();
		ConnectDetach_r(connectionID);
		ChannelDestroy(channelID);
		munmap_device_io(gpio_bank_0,GPIO_REGISTER_LENGHT);
	}

	void Demultiplexer::run(int channelID){
		HAL hal;
		ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.
		_pulse msg;
		while(waitForMessages){
			std::cout << "Waiting for pulse..." << std::endl;

		    int recvid = MsgReceivePulse(channelID, &msg, sizeof(_pulse), nullptr);  //TODO vlt umodellieren auf MsgReceive

		    	std::cout << "Pulse received: " << msg.code << std::endl;
				switch(msg.code){

				case PULSE_E_STOP_PRESSED:
					std::cout << "E-Stop has been pressed!" << std::endl;
					hal.getActors()->motorStop();	//TODO LOGIK AUSLAGERN
					hal.getActors()->redLampOn();
					out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),ESTOP);

					InterruptUnmask((GPIO_0_IRQ_BASE+27),msg.value.sival_int);
					break;

				case PULSE_RESET_PRESSED:
					std::cout << "Reset has been pressed!" << std::endl;
					out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),BUTTON_RESET);
					InterruptUnmask((GPIO_0_IRQ_BASE+26),msg.value.sival_int);
					break;

				case PULSE_STOP_PRESSED:
					std::cout << "Stop has been pressed!" << std::endl;
					hal.getActors()->greenLampOff();
					out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),BUTTON_STOP);
					InterruptUnmask((GPIO_0_IRQ_BASE+23),msg.value.sival_int);
					break;

				case PULSE_START_PRESSED:
					std::cout << "Start has been pressed!" << std::endl;
					hal.getActors()->greenLampOn();
					out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),BUTTON_START);
					InterruptUnmask((GPIO_0_IRQ_BASE+22),msg.value.sival_int);
					break;
				case PULSE_ITEM_AT_ENTRY:
					std::cout << "Item at entry!" << std::endl;
					hal.getActors()->motorForwards();

					out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),ITEM_AT_ENTRY);
					InterruptUnmask((GPIO_0_IRQ_BASE+2),msg.value.sival_int);
					break;

				case PULSE_ITEM_AT_EXIT:
					std::cout << "Item at exit!" << std::endl;
					hal.getActors()->motorStop();

					out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),ITEM_AT_EXIT);
					InterruptUnmask((GPIO_0_IRQ_BASE+20),msg.value.sival_int);
					break;


				case PULSE_ITEM_IN_HEIGHT_MEAS:
				    std::cout << "Item in height measurement!" << std::endl;
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), ITEM_IN_HEIGHT_MEAS);
				    InterruptUnmask((GPIO_0_IRQ_BASE+3), msg.value.sival_int);
				    break;

				case PULSE_ITEM_IN_PUSHER:
				    std::cout << "Item in pusher!" << std::endl;
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), ITEM_IN_PUSHER);
				    InterruptUnmask((GPIO_0_IRQ_BASE+5), msg.value.sival_int);
				    break;

				case PULSE_ITEM_METAL:
				    std::cout << "Item is metal!" << std::endl;
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), ITEM_METAL);
				    InterruptUnmask((GPIO_0_IRQ_BASE+7), msg.value.sival_int);
				    break;

				case PULSE_PUSHER_OPEN:
				    std::cout << "Pusher is open!" << std::endl;
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), PUSHER_OPEN);
				    InterruptUnmask((GPIO_0_IRQ_BASE+14), msg.value.sival_int);
				    break;

				case PULSE_SLIDE_FULL:
				    std::cout << "Slide is full!" << std::endl;
				    out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), SLIDE_FULL);
				    InterruptUnmask((GPIO_0_IRQ_BASE+15), msg.value.sival_int);
				    break;
				case PULSE_STOP_RUNNING:
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





