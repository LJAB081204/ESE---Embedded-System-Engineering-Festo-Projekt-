
#include <sys/neutrino.h>
#include <iostream>
#include "HAL.h"
#include "Demultiplexer.h"
#include "InterruptHandler.h"
#include "Dispatcher.h"
//#include "timer.h"


#define SYSTEM1 "system1"
#define SYSTEM2 "system2"


int main(int argc, char **argv) {
		ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.
		const char* local;
		const char* remote;

		if (argc < 2) {
	        printf("Usage %s -s | -c \n", argv[0]);
	        return 0;
	    }

		if (strcmp(argv[1],"-c") == 0){
			local = SYSTEM1;
			remote = SYSTEM2;
		}
		else{
			local = SYSTEM2;
			remote = SYSTEM1;
		}


        HAL hal;
//        hal.getActors()->greenLampOff();
//        hal.getActors()->yellowLampOff();
//        hal.getActors()->redLampOff();
//        hal.getActors()->motorStop();
        /*
        hal.getActors()->greenLampBlink(500);
        hal.getActors()->yellowLampBlink(500);
        hal.getActors()->redLampBlink(500);

        this_thread::sleep_for(chrono::seconds(10));

        hal.getActors()->greenLampBlinkOff();
        hal.getActors()->yellowLampBlinkOff();
        hal.getActors()->redLampBlinkOff();
		*/

    	Dispatcher dispatcher(local,remote,hal);
    	Demultiplexer demux(dispatcher.getConnectionID());
    	InterruptHandler handler(demux.getConnectionID());
    	/*
    	Timer t1(1, dispatcher.getConnectionID());
    	t1.startOnce(500);

    	Timer t2(2, dispatcher.getConnectionID());
    	t2.startPeriodic(1000);
		*/


    	while(true){

    	}



        return 1;
    }


