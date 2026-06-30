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
#include <fstream>
#include <string>

dataClass::dataClass(TimerHandler& handler): handler(&handler){
	entryTime = 0;
	entryTimeSlow = 0;
	slowFactor = 0;
	heightTime = 0;
	pusherTime = 0;
	exitTime = 0;
	setTimes();

	if(entryTime == 0 || entryTimeSlow == 0 || heightTime == 0 || pusherTime == 0 || exitTime == 0){
		metalDelayJunction = 1;
		meatlDelayPusher = 1;
		maxEntryTime = 1;
		maxPusherTime = 1;
		maxExitTime = 1;
		minEntryTime = 1;
		minPusherTime = 1;
		minExitTime = 1;
	}
	else{
		slowFactor = (double) entryTime / (double) entryTimeSlow;
		minTimeEntry = entryTime / 2;
		metalDelayJunction = (int) pusherTime/10;
		meatlDelayPusher = (int) pusherTime/30;
		maxEntryTime = (int) entryTime * 1.4;
		maxPusherTime = (int) pusherTime * 1.6;
		maxExitTime = (int) exitTime * 1.2;
		minEntryTime = (int) entryTime * 0.9;
		minPusherTime = (int) pusherTime * 0.9;
		minExitTime = (int) exitTime * 0.65;
	}

}

void dataClass::reset(){
 // Connection
//	conId = -1;

// Laufband
	nextItemID = 1;

	while (!section1.empty()){
		if (section1.front().timer != nullptr){
			section1.front().timer->stop();
			delete section1.front().timer;
		}
		section1.pop();
	}

	while (!section2.empty()){
		if (section2.front().timer != nullptr){
			section2.front().timer->stop();
			delete section2.front().timer;
		}
		section2.pop();
	}

	while (!section3.empty()){
		if (section3.front().timer != nullptr){
			section3.front().timer->stop();
			delete section3.front().timer;
		}
		section3.pop();
	}
	while (!nextItem.empty()){
		if (nextItem.front().timer != nullptr){
			nextItem.front().timer->stop();
			delete nextItem.front().timer;
		}
		nextItem.pop();
	}

	if (minTimeEntryTimer != nullptr){
		minTimeEntryTimer->stop();
		delete minTimeEntryTimer;
		minTimeEntryTimer = nullptr;
	}

// Item
	itemCount = 0;
	itemCountRemote = 0;

// Festo
	readyForItem = false;
	readyForItemRemote = true;
	itemAtExit = false;

// Weiche / Slide
	slideFull = false;
	slideFullRemote = false;

// Error
	err = ERROR_CLEARED;
	errorLoc = 0;
	pusherErrorActive = false;
	isErrorLocal = false;
	oneRound = false;
	returnToError = false;

// State Weiche
	weiche = LET_ITEM_PASS_STATE;

	readyForEntry = true;
	setTimes();
	if(entryTime != 0 || entryTimeSlow != 0 || heightTime != 0 || pusherTime != 0 || exitTime != 0){
		slowFactor = (double) entryTime / (double) entryTimeSlow;
		std::cout << "Slow Factor: " << slowFactor << std::endl;
		minTimeEntry = entryTime / 2;
		metalDelayJunction = (int) pusherTime/10;
		meatlDelayPusher = (int) pusherTime/60;
		maxEntryTime = (int) entryTime * 1.4;
		maxPusherTime = (int) pusherTime * 1.6;
		maxExitTime = (int) exitTime * 1.2;
		minEntryTime = (int) entryTime * 0.8;
		minPusherTime = (int) pusherTime * 0.9;
		minExitTime = (int) exitTime * 0.65;
	}
}

bool dataClass::moveFrom1To2(){
	std::cout << "SIZE:" << section1.size() << std::endl;
	if (section1.empty()){
		return false;
	}
	section2.push(section1.front());
	section1.front().timer->stop();
	delete section1.front().timer;
	section2.back().timer = new DynamicTimer(section2.back().id, *handler);
	section2.back().timer->start(maxPusherTime,0);
	section1.pop();
	if(checkReady()){
		MsgSendPulse(controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_FESTO_READY,0);
	}
		std::cout << "SIZE2:" << section2.size() << std::endl;
	return true;
}

bool dataClass::moveFrom2To3(){
	std::cout << "SIZE:" << section2.size() << std::endl;
	 if (section2.empty()){
		 return false;
	 }
	section3.push(section2.front());
	section2.front().timer->stop();
	delete section2.front().timer;
	section3.back().timer = new DynamicTimer(section3.back().id, *handler);
	section3.back().timer->start(maxExitTime,0);
	std::cout << "time after start:" << section3.back().timer->getTime() << std::endl;
	section2.pop();
	return true;
}

bool dataClass::addItem(){
	itemData item;
	item.id = nextItemID++;
	itemCount++;
	item.timer = new DynamicTimer(item.id, *handler);
	section1.push(item);
	return true;
}

bool dataClass::addTransferredItem(itemData item){
	std::cout << "addTransferredItem: id=" << item.id << " nextItem size before=" << nextItem.size() << std::endl;
	item.timer = new DynamicTimer(item.id, *handler);
	nextItem.push(item);
	std::cout << "addTransferredItem: nextItem size after=" << nextItem.size() << std::endl;
	return true;
}

bool dataClass::removeItem(int queue){
	switch (queue){
			case 1:
				if(section1.empty()){
					return false;
				}
				section1.front().timer->stop();
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
				section2.front().timer->stop();
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
				section3.front().timer->stop();
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

int dataClass::findItem(int id){
	std::cout << "findItem called with id: " << id << std::endl;
	std::cout << "s1 front: " << (section1.empty() ? -1 : section1.front().id) << std::endl;
	std::cout << "s2 front: " << (section2.empty() ? -1 : section2.front().id) << std::endl;
	std::cout << "s3 front: " << (section3.empty() ? -1 : section3.front().id) << std::endl;
    if(!section1.empty() && section1.front().id == id){
        return 1;
    }
    if(!section2.empty() && section2.front().id == id){
        return 2;
    }
    if(!section3.empty() && section3.front().id == id){
        return 3;
    }
    return -1;
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

void dataClass::setTimes(){
    std::ifstream file(CALIBRATION_FILE);
    if(!file.is_open()){
        std::cout << "Warning: No calibration file found!" << std::endl;
        return;
    }

    bool foundEntry = false, foundEntrySlow = false, foundHeight = false, foundPusher = false, foundExit = false;
    std::string line;
    while(std::getline(file, line)){
        if(!line.empty() && line.back() == '\r') line.pop_back();
        try {
            if(line.find("timeEntry=")  == 0){ entryTime  = static_cast<uint32_t>(std::stoul(line.substr(10))); foundEntry = true; }
            if(line.find("timeEntrySlow=")  == 0){ entryTimeSlow  = static_cast<uint32_t>(std::stoul(line.substr(14))); foundEntrySlow = true; }
            if(line.find("timeHeight=") == 0){ heightTime = static_cast<uint32_t>(std::stoul(line.substr(11))); foundHeight = true; }
            if(line.find("timePusher=") == 0){ pusherTime = static_cast<uint32_t>(std::stoul(line.substr(11))); foundPusher = true; }
            if(line.find("timeExit=")   == 0){ exitTime   = static_cast<uint32_t>(std::stoul(line.substr(9)));  foundExit = true; }
        } catch(std::exception& e){
            std::cerr << "Fehler beim Parsen: '" << line << "' -> " << e.what() << std::endl;
        }
    }
    std::cout<<"Festo calibrated"<<std::endl;
    file.close();

    if(!foundEntry || !foundEntrySlow || !foundHeight || !foundPusher || !foundExit){
        std::cout << "Times not fully calibrated" << std::endl;
    }


}

void dataClass::raiseError(){
	MsgSendPulse(conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR,0);
    }


void dataClass::updateSequence() {
	if(!isMachine1){
		if(section3.empty()){
			switch(sequenceState){
				case WFP_State: MsgSendPulse(controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_WFP, 0); break;
				case IH_State: MsgSendPulse(controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH, 0); break;
				case IM_State: MsgSendPulse(controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM, 0); break;
				case IF_State: MsgSendPulse(controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF, 0); break;
				case IHM_State: MsgSendPulse(controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM, 0); break;
				default: break;
					}
			}
		else {
			switch(sequenceState){
				case IH_State:
					if (oneRound){
						MsgSendPulse(controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM, 0);
					} else {
						MsgSendPulse(controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_WFP, 0);
					}
					break;
				case IM_State: MsgSendPulse(controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH, 0); break;
				case IF_State: MsgSendPulse(controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM, 0); break;
				case IHM_State: MsgSendPulse(controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF, 0); break;
				default: break;
					}
			MsgSendPulse(conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_LOCAL, 0);
		}
		}
	}
