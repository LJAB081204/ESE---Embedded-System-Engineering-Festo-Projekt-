#include <stdint.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <sys/neutrino.h>
#include <iostream>
using namespace std;
#include "HAL.h"
#include "testing.h"

int main() {
    try {
        HAL hal;

        // Sensors abfragen
        if (hal.getSensors()->isButtonStartPressed()) {
            std::cout << "Start button pressed!" << std::endl;
            hal.getActors()->motorForwards();
            hal.getActors()->greenLampOn();
        }

        if (hal.getSensors()->isEStopTriggered()) {
            hal.getActors()->motorStop();
            hal.getActors()->redLampOn();
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}


