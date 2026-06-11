
#include <sys/neutrino.h>
#include <iostream>
#include "HAL.h"
#include "Demultiplexer.h"
#include "InterruptHandler.h"
#include "Dispatcher.h"
#include "timer.h"
#include "HeightSensor.h"
#include "dataClass.h"
#include "ConnectionController.h"
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

		if (strcmp(argv[1],"-c") == 0){
			local = SYSTEM1;
			remote = SYSTEM2;
		}
		else{
			local = SYSTEM2;
			remote = SYSTEM1;
		}

		cout << "local: " << local << " remote: " << remote << endl;

        HAL hal;
        dataClass data;
        TSCADC tsc;
		HeightSensor hs (tsc,-1);


        ConnectionController conctrl(local,remote);
        Dispatcher dispatcher(conctrl.getConnectionID(), hal, data,hs);
        hs.setCoId(dispatcher.getConnectionID());
        conctrl.start(dispatcher.getConnectionID());


    	Demultiplexer demux(dispatcher.getConnectionID());
    	InterruptHandler handler(demux.getConnectionID());



    	while(true){

    	}



        return 1;
    }


