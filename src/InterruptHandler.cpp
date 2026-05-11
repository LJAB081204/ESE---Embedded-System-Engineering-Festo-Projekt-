/*
 * InterruptHandler.cpp
 *
 *  Created on: 03.05.2026
 *      Author: tayla
 */
#include "InterruptHandler.h"
#include "HAL.h"
#include "PulseMessages.h"
#include <sys/mman.h>
#include <hw/inout.h>
#include <iostream>
using namespace std;

	InterruptHandler::InterruptHandler(int coid){
		gpio_bank_0 = mmap_device_io(GPIO_REGISTER_LENGHT, (uint64_t) GPIO_PORT0);
		connectionID = coid;
		idButtonReset = -1;
		idButtonStart = -1;
		idButtonStop = -1;
		idEstop = -1;
		idItemAtEntry = -1;
		idItemAtExit = -1;
		idPusherOpen = -1;
		idSlideFull = -1;
		iditemAtPusher = -1;
		iditemMetal = -1;
		initInterrupts(connectionID);
	}
	InterruptHandler::~InterruptHandler(){
		InterruptDetach(idButtonReset);
		InterruptDetach(idButtonStart);
		InterruptDetach(idButtonStop);
		InterruptDetach(idEstop);
		InterruptDetach(idItemAtEntry);
		InterruptDetach(idItemAtExit);
		InterruptDetach(idPusherOpen);
		InterruptDetach(idSlideFull);
		InterruptDetach(iditemAtPusher);
		InterruptDetach(iditemMetal);
		munmap_device_io(gpio_bank_0,GPIO_REGISTER_LENGHT);

	}

	void InterruptHandler::initInterrupts(int coid){
		ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.

		out32(gpio_bank_0 + GPIO_FALLINGDETECT, ESTOP | BUTTON_STOP | ITEM_AT_ENTRY | ITEM_AT_EXIT
		      | ITEM_IN_PUSHER | SLIDE_FULL | BUTTON_RESET | BUTTON_START | ITEM_METAL | PUSHER_OPEN);
		out32(gpio_bank_0 + GPIO_RISINGDETECT, ESTOP | BUTTON_STOP | ITEM_AT_ENTRY | ITEM_AT_EXIT
		      | ITEM_IN_PUSHER | SLIDE_FULL | BUTTON_RESET | BUTTON_START | ITEM_METAL | PUSHER_OPEN);

		//Debounce Buttons
		out32(gpio_bank_0 + GPIO_DEBOUNCENABLE, BUTTON_START | BUTTON_STOP | BUTTON_RESET | ESTOP);
		out32(gpio_bank_0 + GPIO_DEBOUNCINGTIME, 0xFF);

		//Interrupt E-Stop (low when true, Bit 27)
		out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),ESTOP);
		//out32((gpio_bank_0 + FALLING_DETECT),ESTOP);
		SIGEV_PULSE_INIT(&eventEstop, connectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_E_STOP, 0);
		idEstop = InterruptAttachEvent((GPIO_0_IRQ_BASE+27),&eventEstop,_NTO_INTR_FLAGS_TRK_MSK);
		eventEstop.sigev_value.sival_int = idEstop;


		//Interrupt Button Reset (high when true, Bit 26)
		out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),BUTTON_RESET);
		//out32((gpio_bank_0 + RISING_DETECT),BUTTON_RESET);
		SIGEV_PULSE_INIT(&eventReset, connectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_RESET, 0);
		idButtonReset = InterruptAttachEvent((GPIO_0_IRQ_BASE+26),&eventReset,_NTO_INTR_FLAGS_TRK_MSK);
		eventReset.sigev_value.sival_int = idButtonReset;
		cout << "ID:" << idButtonReset << endl;

		//Interrupt Button Stop (low when true, Bit 23)
		out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),BUTTON_STOP);
		//out32((gpio_bank_0 + FALLING_DETECT),BUTTON_STOP);
		SIGEV_PULSE_INIT(&eventStop, connectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_STOP, 0);
		idButtonStop =  InterruptAttachEvent((GPIO_0_IRQ_BASE+23),&eventStop,_NTO_INTR_FLAGS_TRK_MSK);
		eventStop.sigev_value.sival_int = idButtonStop;
		cout << "ID:" << idButtonStop << endl;


		//Interrupt Button Start (high when true, Bit 22)
		out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0),BUTTON_START);
		//out32((gpio_bank_0 + RISING_DETECT),BUTTON_START);
		SIGEV_PULSE_INIT(&eventStart, connectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_START, 0);
		idButtonStart =  InterruptAttachEvent((GPIO_0_IRQ_BASE+22),&eventStart,_NTO_INTR_FLAGS_TRK_MSK);
		eventStart.sigev_value.sival_int = idButtonStart;
		cout << "ID:" << idButtonStart << endl;


		//Interrupt Item at entry (low when true, Bit 2)
		out32(gpio_bank_0 + GPIO_IRQSTATUS_SET_0, ITEM_AT_ENTRY);
		//out32(gpio_bank_0 + FALLING_DETECT, ITEM_AT_ENTRY);
		SIGEV_PULSE_INIT(&eventItemAtEntry, connectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_ENTRY, 0);
		idItemAtEntry = InterruptAttachEvent((GPIO_0_IRQ_BASE+2), &eventItemAtEntry, _NTO_INTR_FLAGS_TRK_MSK);
		eventItemAtEntry.sigev_value.sival_int = idItemAtEntry;
		cout << "ID:" << idItemAtEntry << endl;



		//Interrupt Item at exit (low when true, Bit 20)
		out32(gpio_bank_0 + GPIO_IRQSTATUS_SET_0, ITEM_AT_EXIT);
		//out32(gpio_bank_0 + FALLING_DETECT, ITEM_AT_EXIT);
		SIGEV_PULSE_INIT(&eventItemAtExit, connectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_EXIT, 0);
		idItemAtExit = InterruptAttachEvent((GPIO_0_IRQ_BASE+20), &eventItemAtExit, _NTO_INTR_FLAGS_TRK_MSK);
		eventItemAtExit.sigev_value.sival_int = idItemAtExit;


		// Interrupt Item in Pusher (low when true, Bit 5)
		out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), ITEM_IN_PUSHER);
		SIGEV_PULSE_INIT(&eventItemAtPusher, connectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_IN_PUSHER, 0);
		iditemAtPusher = InterruptAttachEvent((GPIO_0_IRQ_BASE+5), &eventItemAtPusher, _NTO_INTR_FLAGS_TRK_MSK);
		eventItemAtPusher.sigev_value.sival_int = iditemAtPusher;

		// Interrupt Item Metal (high when true, Bit 7)
		out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), ITEM_METAL);
		SIGEV_PULSE_INIT(&eventItemMetal, connectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_METAL, 0);
		iditemMetal = InterruptAttachEvent((GPIO_0_IRQ_BASE+7), &eventItemMetal, _NTO_INTR_FLAGS_TRK_MSK);
		eventItemMetal.sigev_value.sival_int = iditemMetal;

		// Interrupt Pusher Open (high when true, Bit 14)
		out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), PUSHER_OPEN);
		SIGEV_PULSE_INIT(&eventPusherOpen, connectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_PUSHER_OPEN, 0);
		idPusherOpen = InterruptAttachEvent((GPIO_0_IRQ_BASE+14), &eventPusherOpen, _NTO_INTR_FLAGS_TRK_MSK);
		eventPusherOpen.sigev_value.sival_int = idPusherOpen;

		// Interrupt Slide Full (low when true, Bit 15)
		out32((gpio_bank_0 + GPIO_IRQSTATUS_SET_0), SLIDE_FULL);
		SIGEV_PULSE_INIT(&eventSlideFull, connectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_SLIDE_FULL, 0);
		idSlideFull = InterruptAttachEvent((GPIO_0_IRQ_BASE+15), &eventSlideFull, _NTO_INTR_FLAGS_TRK_MSK);
		eventSlideFull.sigev_value.sival_int = idSlideFull;
	}






