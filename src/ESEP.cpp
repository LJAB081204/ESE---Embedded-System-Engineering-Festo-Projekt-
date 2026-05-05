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





int main() {
    try {

		ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.

        HAL hal;
        hal.getActors()->greenLampOn();

    	Demultiplexer demux("demo");
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

