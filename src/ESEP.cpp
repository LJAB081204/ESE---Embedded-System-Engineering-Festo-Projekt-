
#include <sys/neutrino.h>
#include <iostream>
#include "HAL.h"
#include "Demultiplexer.h"
#include "InterruptHandler.h"
#include "Dispatcher.h"
#include "timer.h"
#include "HeightSensor.h"
#include "dataClass.h"
using namespace std;


#define SYSTEM1 "system1"
#define SYSTEM2 "system2"


int main(int argc, char **argv) {
		ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.
		const char* local = SYSTEM1;
		const char* remote = SYSTEM2;

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
        dataClass data;

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


    	Dispatcher dispatcher(local,remote,hal,data);
        TSCADC tsc;
        HeightSensor hs (tsc,dispatcher.getConnectionID());
    	Demultiplexer demux(dispatcher.getConnectionID());
    	InterruptHandler handler(demux.getConnectionID());

    	TimerHandler* th = new TimerHandler(dispatcher.getConnectionID());

    	DynamicTimer dt1(1, *th);
    	SimpleTimer st1(2,dispatcher.getConnectionID());
    	SimpleTimer st2(3,dispatcher.getConnectionID());
    	cout << "blabla" << endl;
    	st1.start(2000);
    	sleep(1);
    	st2.start(2000);
    	sleep(3);
    	cout << "blabla" << endl;



    	while(true){

    	}



        return 1;
    }


