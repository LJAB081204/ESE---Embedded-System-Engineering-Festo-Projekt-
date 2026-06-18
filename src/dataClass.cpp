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
	}

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
	std::cout << "SIZE:" << section1.size() << std::endl;
	if (section1.empty()){
		return false;
	}
	if(heightTime - section1.front().timer->getTime()>1500){
		std::cout<< "Error in moving"<< std::endl;
		delete section1.front().timer;
		err = ITEM_APPEARED;
		errorLoc = 1;
		raiseError();
	}
	else{
		delete section1.front().timer;
		section1.front().timer = new DynamicTimer(section1.front().id, *handler);
		section1.front().timer->start(pusherTime); //1000 test wert
		section2.push(section1.front());
		section1.pop();
		if(checkReady()){
			MsgSendPulse(controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_FESTO_READY,0);
		}
		std::cout << "SIZE2:" << section2.size() << std::endl;
	}
	return true;
}

bool dataClass::moveFrom2To3(){
	std::cout << "SIZE:" << section2.size() << std::endl;
	 if (section2.empty()){
		 return false;
	 }
	section2.front().timer = new DynamicTimer(section2.front().id, *handler);
	section2.front().timer->start(exitTime); // Testwert
	section3.push(section2.front());
	section2.pop();
	return true;
}

bool dataClass::addItem(){
	itemData item;
	item.id = nextItemID++;
	itemCount++;
	item.timer = new DynamicTimer(item.id, *handler);
	//item.timer->start(1000); //1000 testwert
	section1.push(item);
	return true;
}

bool dataClass::addTransferredItem(itemData item){
	item.timer = new DynamicTimer(item.id, *handler);
	//item.timer->start(1000);
	nextItem.push(item);
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

void dataClass::setTimes(){
    std::ifstream file(CALIBRATION_FILE);
    if(!file.is_open()){
        std::cout << "Keine Kalibrierungsdatei gefunden, Zeiten nicht verfuegbar" << std::endl;
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
    file.close();

    if(!foundEntry || !foundEntrySlow || !foundHeight || !foundPusher || !foundExit){
        std::cout << "Zeiten noch nicht vollstaendig kalibriert" << std::endl;
    }


}

void dataClass::raiseError(){
	MsgSendPulse(conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR,0);
    }

