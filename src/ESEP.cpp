#include <stdint.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <sys/neutrino.h>
#include <iostream>
using namespace std;
#include "HAL.h"
#include "testing.h"
#include "Demultiplexer.h"
#include "InterruptHandler.h"
#include "Dispatcher.h"

#define SYSTEM1 "system1"
#define SYSTEM2 "system2"



int main(int argc, char **argv) {
    try {

		ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.
		const char* local;
		const char* remote;
	    /*
		if (argc < 2) {
	        printf("Usage %s -s | -c \n", argv[0]);
	        ret = EXIT_FAILURE;
	    }

		if (strcmp(argv[1],"-c") == 0){
			local = SYSTEM1;
			remote = SYSTEM2;
		}
		else{
			remote = SYSTEM2;
			local = SYSTEM1;
		}
		*/
		local = SYSTEM1;
		remote = SYSTEM2;
        HAL hal;
        hal.getActors()->motorStop();
        hal.getActors()->greenLampBlink(500);
        hal.getActors()->yellowLampBlink(500);
        hal.getActors()->redLampBlink(500);

        this_thread::sleep_for(chrono::seconds(10));

        hal.getActors()->greenLampBlinkOff();
        hal.getActors()->yellowLampBlinkOff();
        hal.getActors()->redLampOn();

    	Dispatcher dispatcher(local,remote);
    	Demultiplexer demux(dispatcher.getConnectionID());
    	InterruptHandler handler(demux.getConnectionID());
    	while(true){

    	}


        hal.getActors()->ledButtonReset();


        // Sensors abfragen
        while (!hal.getSensors()->isButtonStartPressed()) {
        }
        std::cout << "Start button pressed!" << std::endl;
        hal.getActors()->motorForwards();
        hal.getActors()->greenLampOn();

        while (!hal.getSensors()->isEStopTriggered()) {
        }
        hal.getActors()->motorStop();
        hal.getActors()->redLampOn();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}

