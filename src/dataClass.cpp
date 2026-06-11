/*
 * dataClass.cpp
 *
 *  Created on: 02.06.2026
 *      Author: lewis
 */

#include "dataClass.h"

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
	section2.push(section1.front());
	section1.pop();
	return true;
}

bool dataClass::moveFrom2To3(){
	 if (section2.empty()){
		 return false;
	 }
	section3.push(section2.front());
	section2.pop();
	return true;
}

bool dataClass::addItem(){
	itemData item;
	item.id = nextItemID++;
	itemCount++;
	section1.push(item);
	return true;
}

bool dataClass::removeItem(int queue){
	switch (queue){
			case 1:
				if(section1.empty()){
					return false;
				}
				section1.pop();
				itemCount--;
				return true;
			case 2:
				if(section2.empty()){
					return false;
				}
				section2.pop();
				itemCount--;
				return true;
			case 3:
				if(section3.empty()){
					return false;
				}
				section3.pop();
				itemCount--;
				return true;
			default:
				return false;
	}
}

