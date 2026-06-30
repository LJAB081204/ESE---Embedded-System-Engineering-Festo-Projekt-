/*
 * Dispatcher.cpp
 *
 *  Created on: 06.05.2026
 *      Author: lewis
 */
#include "Dispatcher.h"
#include "HeightSensor.h"
#include "PulseMessages.h"
#include "Config.h"
#include "dataClass.h"
#include <chrono>
#include <sys/dispatch.h>


using namespace std;

Dispatcher::Dispatcher(int conCtrlConnectionID, HAL& hal,dataClass& data, HeightSensor& hs,RunningFactory& factory,TimerHandler& timerHandler)
: conCtrlConnectionID(conCtrlConnectionID), hal(hal), data(data), factory(factory),timerHandler(timerHandler), FSM(hal, data, hs, factory,timerHandler) {

    if ((ownChannelID = ChannelCreate(0)) == -1){
        //cout << "ChannelCreate failed" << endl;
    }
    ownConnectionID = ConnectAttach(0, 0, ownChannelID, _NTO_SIDE_CHANNEL, 0);
    if (ownConnectionID == -1){
        //cout << "ConnectAttach failed" << endl;
    }
    waitForMessages = true;
    DispatcherThread = thread(&Dispatcher::run, this);

    data.conId = ownConnectionID;
    data.controllerConId = conCtrlConnectionID;
    timerHandler.setCoid(ownConnectionID);
    itemAtExit = false;
}
	Dispatcher::~Dispatcher(){
		MsgSendPulse(ownConnectionID,-1,PULSE_STOP_RECEIVING,0);
		DispatcherThread.join();
	}

	void Dispatcher::run(){
	    ThreadCtl(_NTO_TCTL_IO, 0);

		while(waitForMessages){
			int rcvid = MsgReceive(ownChannelID, &msg, sizeof(msg), nullptr);

			if (rcvid == 0){
				//Pulse
				switch(msg.code){
				    case PULSE_STOP_RECEIVING:
				        waitForMessages = false;
				        break;

				    case PULSE_BTN_ESTOP_LOCAL:
				        //cout << "ESTOP Pulse reached dispatcher" << endl;
				        if(msg.value.sival_int == 0){
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_ESTOP_REMOTE, 0);
				            data.eStopIsPressed = false;
				            FSM.eventButtonEstopReleased();
				        } else {
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_ESTOP_REMOTE, 1);
				            data.eStopIsPressed = true;
				            data.eStopWasPressed = true;
				            FSM.eventButtonEstopPressed();
				        }
				        break;

				    case PULSE_BTN_ESTOP_REMOTE:
				        //cout << "ESTOP Remote Pulse reached dispatcher" << endl;
				        if(msg.value.sival_int == 0){
				            FSM.eventButtonEstopReleasedRemote();
				        } else {
				            FSM.eventButtonEstopPressedRemote();
				        }
				        break;

				    case PULSE_BTN_START_LOCAL:
				        if(msg.value.sival_int == 1){
				            //cout << "Start pressed!" << endl;
				            buttonPressStartLocal = chrono::steady_clock::now();
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_START_REMOTE, 1);
				        } else {
				            auto ms = chrono::duration_cast<chrono::milliseconds>(
				            	chrono::steady_clock::now() - buttonPressStartLocal).count();
				            if(ms > 1000){
				            	FSM.eventButtonStartPressedLong();
				            }
				            else{
				            	FSM.eventButtonStartPressed();
				            }
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_START_REMOTE, 0);
				        }
				        break;

				    case PULSE_BTN_START_REMOTE:
				        if(msg.value.sival_int == 1){
				            buttonPressStartRemote = chrono::steady_clock::now();
				        } else {
				            auto ms = chrono::duration_cast<chrono::milliseconds>(
				            	chrono::steady_clock::now() - buttonPressStartRemote).count();

				            if(ms > 1000){
				            	FSM.eventButtonStartPressedLongRemote();
				            }
				            else{
				            FSM.eventButtonStartPressedRemote();
				            }
				        }
				        break;

				    case PULSE_BTN_RESET_LOCAL:
				        if(msg.value.sival_int == 1){
				            //cout << "Reset pressed!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_RESET_REMOTE, 1);
				        } else {
				            //cout << "Reset released!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_RESET_REMOTE, 0);
				            FSM.eventButtonResetPressed();
				        }
				        break;

				    case PULSE_BTN_RESET_REMOTE:
				        if(msg.value.sival_int == 1){
				            //cout << "Reset pressed on remote!" << endl;
				        } else {
				            //cout << "Reset released on remote!" << endl;
				            FSM.eventButtonResetPressedRemote();
				        }
				        break;

				    case PULSE_BTN_STOP_LOCAL:
				        if(msg.value.sival_int == 1){
				            //cout << "Stop pressed!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_STOP_REMOTE, 1);
				        } else {
				            //cout << "Stop released!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_BTN_STOP_REMOTE, 0);
				            FSM.eventButtonStopPressed();
				        }
				        break;

				    case PULSE_BTN_STOP_REMOTE:
				        if(msg.value.sival_int == 1){
				            //cout << "Stop pressed on remote!" << endl;
				        } else {
				            //cout << "Stop released on remote!" << endl;
				            FSM.eventButtonStopPressedRemote();
				        }
				        break;

				    case PULSE_ITEM_AT_ENTRY_LOCAL:
				        if(msg.value.sival_int == 1){
				            //cout << "Item at entry!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_ENTRY_REMOTE, 1);
				            FSM.eventItemAtEntry();
				        } else {
				            //cout << "Item left entry!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_ENTRY_REMOTE, 0);
				            FSM.eventItemLeftEntry();
				        }
				        break;

				    case PULSE_ITEM_AT_ENTRY_REMOTE:
				        if(msg.value.sival_int == 1){
				            //cout << "Item at entry on remote!" << endl;
				            FSM.eventItemAtEntryRemote();
				        } else {
				            //cout << "Item left entry on remote!" << endl;
				            FSM.eventItemLeftEntryRemote();
				        }
				        break;

				    case PULSE_ITEM_AT_EXIT_LOCAL:
				        if(msg.value.sival_int == 1){
				            //cout << "Item at exit!" << endl;
				        	MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_EXIT_REMOTE, 1);
				            FSM.eventItemAtExit();
				        } else {
				            //cout << "Item left exit!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_EXIT_REMOTE, 0);
				            FSM.eventItemLeftExit();
				        }
				        break;

				    case PULSE_ITEM_AT_EXIT_REMOTE:
				        if(msg.value.sival_int == 1){
				            //cout << "Item at exit on remote!" << endl;
				            FSM.eventItemAtExitRemote();
				        } else {
				            //cout << "Item left exit on remote!" << endl;
				            FSM.eventItemLeftExitRemote();

				        }
				        break;

				    case PULSE_ITEM_AT_PUSHER_LOCAL:
				        if(msg.value.sival_int == 1){
				            //cout << "Item in pusher!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_PUSHER_REMOTE, 1);
				            FSM.eventItemAtPusher();
				        } else {
				            //cout << "Item left pusher!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_PUSHER_REMOTE, 0);
				            FSM.eventItemLeftPusher();
				        }
				        break;

				    case PULSE_ITEM_AT_PUSHER_REMOTE:
				        if(msg.value.sival_int == 1){
				            //cout << "Item in pusher on remote!" << endl;
				            FSM.eventItemAtPusherRemote();
				        } else {
				            //cout << "Item left pusher on remote!" << endl;
				            FSM.eventItemLeftPusherRemote();

				        }
				        break;

				    case PULSE_ITEM_METAL_LOCAL:
				        if(msg.value.sival_int == 1){
				            //cout << "Item is metal!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_METAL_REMOTE, 1);
				            FSM.eventItemMetal();
				        } else {
				            //cout << "Item is not metal!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_METAL_REMOTE, 0);
				        }
				        break;

				    case PULSE_ITEM_METAL_REMOTE:
				        if(msg.value.sival_int == 1){
				            //cout << "Item is metal on remote!" << endl;
				            FSM.eventItemMetalRemote();
				        } else {
				            //cout << "Item is not metal on remote!" << endl;
				        }
				        break;

				    case PULSE_PUSHER_OPEN_LOCAL:
				        if(msg.value.sival_int == 1){
				            //cout << "Pusher is open!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_PUSHER_OPEN_REMOTE, 1);
				        } else {
				            //cout << "Pusher is closed!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_PUSHER_OPEN_REMOTE, 0);
				        }
				        break;

				    case PULSE_PUSHER_OPEN_REMOTE:
				        if(msg.value.sival_int == 1){
				            //cout << "Pusher is open on remote!" << endl;
				        } else {
				            //cout << "Pusher is closed on remote!" << endl;
				        }
				        break;

				    case PULSE_SLIDE_FULL_LOCAL:
				        if(msg.value.sival_int == 1){
				            //cout << "Slide is full!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_SLIDE_FULL_REMOTE, 1);
				            FSM.eventItemInSlide();
				        } else {
				            //cout << "Slide cleared!" << endl;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_SLIDE_FULL_REMOTE, 0);
				            FSM.eventItemLeftSlide();
				        }
				        break;

				    case PULSE_SLIDE_FULL_REMOTE:
				        if(msg.value.sival_int == 1){
				            //cout << "Slide is full on remote!" << endl;
				            FSM.eventItemInSlideRemote();
				        } else {
				            //cout << "Slide cleared on remote!" << endl;
				            FSM.eventItemLeftSlideRemote();

				        }
				        break;

				    case PULSE_ITEM_GONE:
				    	FSM.eventItemGone();
				    	break;
				    case PULSE_ITEM_ARRIVED:
				    	FSM.eventItemArrived();
				    	break;
				    case PULSE_EVENT_TRANSFER_REQUEST:
				    	if(data.checkReady()){
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_TRANSFER_REPLY, 1);
				    	}
				    	else{
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_TRANSFER_REPLY, 0);
				    	}
				    	break;
				    case PULSE_EVENT_TRANSFER_REPLY:
				    	if(msg.value.sival_int == 1){
				    		FSM.eventFestoReady();
				    	}
				    	else{
				    		FSM.eventFestoNotReady();
				    	}
				    	break;

				    case PULSE_NEW_HEIGHT_LOCAL:
				    	switch(msg.value.sival_int){
							case HEIGHT_CONVEYOR: // Height Conveyer
								FSM.eventItemGone();
								break;
							case HEIGHT_DRILLING:
								FSM.eventHeightHole();
								break;
							case HEIGHT_WORKPIECE_HIGH:
								FSM.eventHeightHigh();
								break;
							case HEIGHT_WORKPIECE_FLAT:
								FSM.eventHeightFlat();
								break;
							case HEIGHT_TOO_HIGH:
								FSM.eventHeightWrong();
								break;
							default:
								break;
				    	break;
				    	}
				    	break;

				    	case PULSE_DYNAMIC_TIMER_EXPIRED:
						switch(msg.value.sival_int){
							case 100:
								FSM.eventTimerExpired();
								break;
							case 101:
								data.readyForEntry = true;
								hal.getActors()->lampQ1Off();
								break;
							case 102:
								FSM.eventTransportTimerExpired();
								break;
						default:
							if(msg.code < 0){
								break;
							}
							data.isErrorLocal = true;
							int itemLoc = data.findItem(msg.value.sival_int);
							data.handler->pauseAll();
							switch(itemLoc){
								case 1:
									data.err = ITEM_GONE;
									data.errorLoc = 1;
									FSM.eventError();
									MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ERROR_REMOTE, 0);
									break;
								case 2:
									data.err = ITEM_GONE;
									data.errorLoc = 2;
									FSM.eventError();
									MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ERROR_REMOTE, 0);
									break;
								case 3:
								{
								    data.err = ITEM_GONE;
								    data.errorLoc = 3;
								    FSM.eventError();  // ← erst Error
								    MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ERROR_REMOTE, 0);
								    break;
								}
								default:
								    break;
								}
								break;
						}
						break;

							default:
						        if (msg.code < 0) {
						            break;
						        }
								data.isErrorLocal = true;
								FSM.eventError();

								break;
					case PULSE_EVENT_ERROR:
							data.isErrorLocal = true;
				            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ERROR_REMOTE, 0);
							FSM.eventError();
							break;
					case PULSE_EVENT_ERROR_REMOTE:
							data.isErrorLocal = false;
							FSM.eventError();
							break;

					case PULSE_EVENT_UPDATE:
							FSM.eventUpdate();
							break;
					case PULSE_EVENT_ITEM_CORRECT:
							FSM.eventItemCorrect();
							break;
					case PULSE_EVENT_ITEM_WRONG:
							FSM.eventItemWrong();
							break;

					case PULSE_ITEM_TRANSFER:
						//FSM.eventItemTransfer(msg.value.sival_int);
						data.addTransferredItem(data.convertIntToItem(msg.value.sival_int));
						break;
					case PULSE_EVENT_SLIDE_TIMER:
						FSM.eventSlideTimerExpired();
						break;
					case PULSE_EVENT_FESTO_READY:
						FSM.eventFestoReady();
						break;


					case PULSE_EVENT_GO_BACK_LOCAL:
						FSM.eventGoBack();
						break;
					case PULSE_EVENT_GO_BACK_WFP:
						data.sequenceState = WFP_State;
						FSM.eventGoBackWFP();
						break;
					case PULSE_EVENT_GO_BACK_IH:
						data.sequenceState = IH_State;
						FSM.eventGoBackIH();
						break;
					case PULSE_EVENT_GO_BACK_IM:
						data.sequenceState = IM_State;
						FSM.eventGoBackIM();
						break;
					case PULSE_EVENT_GO_BACK_IF:
						data.sequenceState = IF_State;
						FSM.eventGoBackIF();
						break;
					case PULSE_EVENT_GO_BACK_IHM:
						data.sequenceState = IHM_State;
						FSM.eventGoBackIHM();
						break;

					case PULSE_EVENT_ERROR_PUSHER_TIMER:
						data.isErrorLocal = true;
			            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ERROR_PUSHER_TIMER_REMOTE, 0);
						FSM.errorPusherTimerExpired();
						break;
					case PULSE_EVENT_ERROR_PUSHER_TIMER_REMOTE:
						data.isErrorLocal = false;
						FSM.errorPusherTimerExpired();
						break;
					case PULSE_EVENT_ERROR_RESOLVED:
					    FSM.eventErrorResolved();
					    break;
					case PULSE_EVENT_ERROR_ENTRY:
						data.isErrorLocal = true;
						data.err = ENTRY_TOO_FAST;
			            MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ERROR_REMOTE, 0);
			            FSM.eventError();
			            break;
					case PULSE_CONNECTION_LOST:
						FSM.eventConnectionLost();
						break;
					case PULSE_CONNECTION_RESTORED:
						FSM.eventConnectionRestored();
						break;

					case PULSE_HANDSHAKE_ESTOP_STATUS:
						if (data.eStopIsPressed){
							MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_RESPONSE_ESTOP_STATUS, 0);
							break;
						} else if (!data.eStopIsPressed && data.eStopWasPressed){
							MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_RESPONSE_ESTOP_STATUS, 1);
							break;
						} else {
							MsgSendPulse(conCtrlConnectionID, SIGEV_PULSE_PRIO_INHERIT, PULSE_RESPONSE_ESTOP_STATUS, 2);
							break;
						}
					case PULSE_EVENT_SKIP_UPDATE:
						FSM.eventSkipUpdate();
						break;


					case PULSE_RESPONSE_ESTOP_STATUS:
						switch (msg.value.sival_int){
							case 0:
								FSM.eventResponseEStopIsPressed();
								break;
							case 1:
								FSM.eventResponseEStopWasPressed();
								break;
							case 2:
								FSM.eventResponseEStopWasNotPressed();
								break;
						}
				}


			}
			else{
				//Software Message
				MsgReply(rcvid,0,nullptr,0);
			}
		}

	}

	int Dispatcher::getConnectionID(){
		return ownConnectionID;
	}






