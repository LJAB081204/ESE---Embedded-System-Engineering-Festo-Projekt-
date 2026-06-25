/*
 * dataClass.h
 *
 *  Created on: 02.06.2026
 *      Author: lewis
 */

#ifndef DATACLASS_H_
#define DATACLASS_H_

#define CALIBRATION_FILE "/etc/heightsensor/calibration.cfg"

#include <queue>
#include "timer.h"
//#include "Dispatcher.h"

enum systemState { IDLE_STATE,RUNNING_STATE,ERROR_STATE,ESTOP_STATE };
enum itemProfile { IH_PROFILE, IM_PROFILE, IF_PROFILE, IHM_PROFILE, IU_PROFILE, IS_PROFILE, I_PROFILE, UNDEFINED_PROFILE};
enum errors { ERROR_CLEARED, ITEM_SPECIAL_0,ITEM_SPECIAL_1, ITEM_GONE, BOTH_SLIDES_FULL, ITEM_APPEARED,CONNECTION_LOST,ENTRY_TOO_FAST};
				//0				1				2			3					4			5
enum WeicheState { LET_ITEM_PASS_STATE,WAIT_STATE};
enum ErrorState  { CURRENT_NACK_STATE,CURRENT_ACK_STATE,PUSHER_TOO_LONG_STATE,GONE_NACK_STATE,WAITING_FOR_START_STATE,WAITING_FOR_START_STATE2};
struct itemData{
		int id = 0;
		bool flat = false;
		bool high = false;
		bool hole = false;
		bool metal = false;
		bool unknown = false;
		bool special = false;
		bool sortOut = false;

		itemProfile item = UNDEFINED_PROFILE;
		DynamicTimer* timer = nullptr;
	};

class dataClass{
	public:

	TimerHandler* handler;
	//Connection
	int conId = -1;
	int controllerConId = -1;

	// Laufband
	int nextItemID = 1;

	std::queue<itemData> section1;   // Einlauf -> Höhensensor
	std::queue<itemData> section2;   // Höhensensor -> Weiche
	std::queue<itemData> section3;   // Weiche -> Bandende

	std::queue<itemData> nextItem;		// in Transport FSM -> bei LeftExit hinzufügen

	//item
	int itemCount = 0;
	int itemCountRemote = 0;

	bool oneRound = false;

	//Festo
	bool readyForItem = true;
	bool readyForItemRemote = true;

	//Weiche/Slide
	bool slideFull = false;
	bool slideFullRemote = false;

	//Error
	errors err = ERROR_CLEARED;
	int errorLoc = 0;
	bool pusherErrorActive = false;
	bool isErrorLocal = false;
	bool returnToError = false;
	bool heightDefault = false;

	// Estop Status
	bool eStopIsPressed = false;
	bool eStopWasPressed = false;

	WeicheState weiche = LET_ITEM_PASS_STATE;
	ErrorState errorState = CURRENT_NACK_STATE;

	dataClass(TimerHandler& handler);
	void reset();
	bool moveFrom1To2();
	bool moveFrom2To3();
	bool addItem();
	bool removeItem(int queue);
	bool addTransferredItem(itemData item);

	int findItem(int id);

	uint32_t convertItemToInt(const itemData& item);
	itemData convertIntToItem(uint32_t itemCode);
	void raiseError();

	bool checkReady();
	bool itemAtExit = false;

	uint32_t entryTime;
	uint32_t entryTimeSlow;
	double slowFactor;
	uint32_t heightTime;
	uint32_t pusherTime;
	uint32_t exitTime;
	uint32_t minTimeEntry;
	uint32_t metalDelayJunction;
	uint32_t meatlDelayPusher;
	int maxEntryTime;
	int maxPusherTime;
	int maxExitTime;
	int minEntryTime;
	int minPusherTime;
	int minExitTime;
	bool readyForEntry = true;
	DynamicTimer* minTimeEntryTimer = nullptr;
	DynamicTimer* timerIS = nullptr;

	bool isMachine1 = false;
	private:

	void setTimes();
	void resetEstop();
};


#endif /* DATACLASS_H_ */
