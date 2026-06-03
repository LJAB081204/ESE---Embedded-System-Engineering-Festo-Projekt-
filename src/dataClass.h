/*
 * dataClass.h
 *
 *  Created on: 02.06.2026
 *      Author: lewis
 */

#ifndef DATACLASS_H_
#define DATACLASS_H_

#include <queue>

enum systemState { IDLE,RUNNING,ERROR,ESTOP_STATE };
enum itemProfile { IH, IM, IF, IHM, IU, IS, UNDEFINED};
enum errors { ERROR_CLEARED, ITEM_GONE, ITEM_APPEARED, BOTH_SLIDES_FULL, ITEM_SPECIAL, CONNECTION_LOST, HW_CONTROL_FAILED, PUSHER_OPEN_TOO_LONG };

struct itemData{
		int id = 0;
		bool flat = false;
		bool high = false;
		bool hole = false;
		bool metal = false;
		bool unknown = false;
		bool special = false;

		itemProfile item = UNDEFINED;
	};

class dataClass{
	public:

	//Connection
	int conId = -1;

	// Laufband
	int nextItemID = 1;

	std::queue<itemData> section1;   // Einlauf -> Höhensensor
	std::queue<itemData> section2;   // Höhensensor -> Weiche
	std::queue<itemData> section3;   // Weiche -> Bandende

	//item
	int itemCount = 0;
	int itemCountRemote = 0;

	//Festo
	bool readyForItem = true;
	bool readyForItemRemote = true;

	//Weiche/Slide
	bool slideFull = false;
	bool slideFullRemote = false;

	//Error
	errors err = ERROR_CLEARED;
	int errorLoc = 0;

	void reset();
	bool moveFrom1To2();
	bool moveFrom2To3();
	bool addItem();
};


#endif /* DATACLASS_H_ */
