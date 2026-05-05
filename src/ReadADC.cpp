#include <stdio.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/procmgr.h>
#include <iostream>
#include <thread>
#include <chrono>

// #include "simqnxgpioapi.h"
// #include "simqnxirqapi.h"

#include <sys/mman.h>
#include <hw/inout.h>

#include "ADC_Sources/ADC.h"
#include "ADC_Sources/TSCADC.h"




/* Code configuration */
#define DEMO true         // true for demo time, false to spin forever.
#define DEMO_DURATION 120 // Duration of demo time in minutes.



/* GPIO port addresses (spruh73l.pdf S.177 ff.) */
#define GPIO_PORT0 0x44E07000
#define GPIO_PORT1 0x4804C000
#define GPIO_PORT2 0x481AC000
#define ADC_BASE 0x44E0D000

/* GPIO port registers length */
#define GPIO_REGISTER_LENGHT 0x1000
#define ADC_LENGTH 0x2000

/* GPIO register offsets (spruh73l.pdf S.4877) */
#define GPIO_DATAIN 0x138
#define GPIO_SETDATAOUT 0x194

/* TSC_ADC register offsets (spruh73l.pdf S.1747) */
#define ADC_IRQ_ENABLE_SET 0x2c
#define ADC_IRQ_ENABLE_CLR 0x30
#define ADC_IRQ_STATUS 0x28
#define ADC_CTRL 0x40
#define ADC_DATA 0x100

/* ADC irq pin mask */
#define ADC_IRQ_PIN_MASK 0x2

/* Actuators pin mapping (Aufgabenbeschreibung o. Schaltplan) */
#define MOTOR_RIGHT_PIN 12
#define MOTOR_SLOW_PIN 14
#define MOTOR_STOP_PIN 15

/* Helper macros */
#define BIT_MASK(x) (0x1 << (x))

/* My pulse codes */
#define PULSE_STOP_THREAD _PULSE_CODE_MINAVAIL + 1
#define PULSE_ADC_SAMLING_DONE _PULSE_CODE_MINAVAIL + 2

/* Varialbes */
bool receivingRunning = false;

/* Function declaration */
void receivingRoutine(int channelID, ADC *adc);

using namespace std;

//int main(int argc, char **args)
//{
//
//  /* ### Create channel ### */
//  int chanID = ChannelCreate(0); // Create channel to receive interrupt pulse messages.
//  if (chanID < 0)
//  {
//    perror("Could not create a channel!\n");
//    return EXIT_FAILURE;
//  }
//
//  int conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0); // Connect to channel.
//  if (conID < 0)
//  {
//    perror("Could not connect to channel!");
//    return EXIT_FAILURE;
//  }
//
//  /* ### Start motor slow for demo ### */
//  uintptr_t port1BaseAddr = mmap_device_io(GPIO_REGISTER_LENGHT, GPIO_PORT1);
//  out32((uintptr_t)port1BaseAddr + GPIO_SETDATAOUT, BIT_MASK(MOTOR_RIGHT_PIN));
//  out32((uintptr_t)port1BaseAddr + GPIO_SETDATAOUT, BIT_MASK(MOTOR_SLOW_PIN));
//
//  TSCADC tsc;
//  ADC *adc = new ADC(tsc);
//
//  adc->registerAdcISR(conID, PULSE_ADC_SAMLING_DONE);
//
//  /* ### Start thread for handling interrupt messages. */
//  thread receivingThread = thread(&receivingRoutine, chanID, adc);
//
//  adc->sample();
//
//  if (DEMO)
//  {
//    printf("Demo time for %d minutes...\n", DEMO_DURATION);
//    this_thread::sleep_for(chrono::seconds(DEMO_DURATION)); // main thread sleep.
//    printf("Stopping in...\n");
//    printf("3\n");
//    this_thread::sleep_for(chrono::seconds(1));
//    printf("2\n");
//    this_thread::sleep_for(chrono::seconds(1));
//    printf("1\n");
//    this_thread::sleep_for(chrono::seconds(1));
//    printf("NOW!\n");
//  }
//  else
//  {
//    while (true)
//    {
//      this_thread::sleep_for(chrono::hours(999999));
//    }
//  }
//
//  /* ### Cleaning up ### */
//
//  // Detach interrupts.
//  adc->adcDisable();
//  adc->unregisterAdcISR();
//
//  // Stop receiving thread.
//  MsgSendPulse(conID, -1, PULSE_STOP_THREAD, 0); // using prio of calling thread.
//  receivingThread.join();
//
//  // Close channel
//  int detach_status = ConnectDetach(conID);
//  if (detach_status != EOK)
//  {
//    perror("Detaching channel failed!");
//    exit(EXIT_FAILURE);
//  }
//
//  int destroy_status = ChannelDestroy(chanID);
//  if (destroy_status != EOK)
//  {
//    perror("Destroying channel failed!");
//    exit(EXIT_FAILURE);
//  }
//
//  out32((uintptr_t)port1BaseAddr + GPIO_SETDATAOUT, BIT_MASK(MOTOR_STOP_PIN));
//
//  return EXIT_SUCCESS;
//}

void receivingRoutine(int channelID, ADC *adc)
{

  ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.

  _pulse msg;
  receivingRunning = true;

  printf("Message thread started.\n");

  while (receivingRunning)
  {
    int recvid = MsgReceivePulse(channelID, &msg, sizeof(_pulse), nullptr);

    if (recvid < 0)
    {
      perror("MsgReceivePulse failed!");
      exit(EXIT_FAILURE);
    }

    if (recvid == 0)
    { // pulse received.

      // Stop thread while it blocks.
      if (msg.code == PULSE_STOP_THREAD)
      {
        printf("Thread kill code received!\n");
        receivingRunning = false;
        continue;
      }

      // ADC interrupt value.
      if (msg.code == PULSE_ADC_SAMLING_DONE)
      {
        printf("Value from adc with value %d!\n", msg.value);
        adc->sample();
      }

      // Do not ignore OS pulses!
    }
  }

  printf("Message thread stops...\n");
}
