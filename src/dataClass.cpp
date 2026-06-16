/*
 * dataClass.cpp
 *
 *  Created on: 02.06.2026
 *      Author: lewis
 */

#include "dataClass.h"
#include "PulseMessages.h"
#include "sys/neutrino.h"
#include <iostream>

dataClass::dataClass(TimerHandler& handler)
    : handler(&handler)
{}

void dataClass::reset(){
 // Connection
	conId = -1;

// Laufband
	nextItemID = 1;

	while (!section1.empty()){
		section1.pop();
	}

	while (!section2.empty()){
		section2.pop();
	}

	while (!section3.empty()){
		section3.pop();
	}


// Item
	itemCount = 0;
	itemCountRemote = 0;

// Festo
	readyForItem = false;
	readyForItemRemote = true;

// Weiche / Slide
	slideFull = false;
	slideFullRemote = false;

// Error
	err = ERROR_CLEARED;
	errorLoc = 0;
}

bool dataClass::moveFrom1To2(){
	if (section1.empty()){
		return false;
	}
	section1.front().timer = new DynamicTimer(section1.front().id, *handler);
	section1.front().timer->start(1000); //1000 test wert
	section2.push(section1.front());
	section1.pop();
	return true;
}

bool dataClass::moveFrom2To3(){
	 if (section2.empty()){
		 return false;
	 }
	section2.front().timer = new DynamicTimer(section2.front().id, *handler);
	section2.front().timer->start(1000); // Testwert
	section3.push(section2.front());
	section2.pop();
	return true;
}

bool dataClass::addItem(){
	itemData item;
	item.id = nextItemID++;
	itemCount++;
	item.timer = new DynamicTimer(section1.front().id, *handler);
	section1.front().timer->start(1000); //1000 testwert
	section1.push(item);
	return true;
}

bool dataClass::addTransferredItem(itemData item){
	itemCount++;
	item.timer = new DynamicTimer(item.id, *handler);
	item.timer->start(1000);
	section1.push(item);
	return true;
}

bool dataClass::removeItem(int queue){

	switch (queue){
			case 1:
				if(section1.empty()){
					return false;
				}
				delete section1.front().timer;
				section1.pop();
				itemCount--;
				if(checkReady()){
					MsgSendPulse(controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_FESTO_READY,0);
					std::cout << "PULSE READY SEND" << std::endl;

				}
				return true;
			case 2:
				if(section2.empty()){
					return false;
				}
				delete section2.front().timer;
				section2.pop();
				itemCount--;
				if(checkReady()){
					MsgSendPulse(controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_FESTO_READY,0);
					std::cout << "PULSE READY SEND" << std::endl;

				}
				return true;
			case 3:
				if(section3.empty()){
					return false;
				}
				delete section3.front().timer;
				section3.pop();
				itemCount--;
				if(checkReady()){
					MsgSendPulse(controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_FESTO_READY,0);
					std::cout << "PULSE READY SEND" << std::endl;
				}
				return true;
			default:
				return false;
	}
}

uint32_t dataClass::convertItemToInt(const itemData& item)
{
    return (static_cast<uint32_t>(item.id) << 4) |
           static_cast<uint32_t>(item.item);
}

itemData dataClass::convertIntToItem(uint32_t itemCode)
{
    itemData item;
    item.item = static_cast<itemProfile>(itemCode & 0xF); // untere 4 Bits
    item.id = itemCode >> 4;                                // obere 28 Bits

    return item;
}
bool dataClass::checkReady(){
	if(itemCount < 2 && section1.empty() && itemAtExit == false){
		return true;
	}
	else{
		return false;
	}
}

