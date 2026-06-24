/*
 * currentAck.cpp
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#include <new>
#include "ErrorBase.h"
#include "ErrorContext.h"
#include "waitingForStart.h"
#include "../../PulseMessages.h"
#include "currentAck.h"
#include "errorPusherTooLong.h"
#include <sys/neutrino.h>
#include <iostream>


	currentAck::currentAck(){

	}

	void currentAck::entry(ErrorContext& ctx){
		ctx.hal.getActors()->redLampBlinkOff();
		ctx.hal.getActors()->redLampOn();
		ctx.data.errorState = CURRENT_ACK_STATE;
		switch (ctx.data.err){
		case ITEM_SPECIAL: std::cout << "Spezialitem beim Höhensensor entfernen."<< std::endl;
			break;
		case BOTH_SLIDES_FULL: std::cout << "Rutsche auf Band zwei leeren." << std::endl;
			break;
		case ITEM_APPEARED: std::cout << "Werkstück entfernen. Location :"  << ctx.data.errorLoc << std::endl;
			break;
		default:
			break;
		}
	}

	void currentAck::exit(ErrorContext& ctx){
		ctx.hal.getActors()->redLampOff();
	}

	void currentAck::eventItemLeftSlide(ErrorContext& ctx){
		if(ctx.data.err == BOTH_SLIDES_FULL){
			ctx.data.err = ERROR_CLEARED;
			raiseItemAtPusher(ctx);
			exit(ctx);
			new(this) waitingForStart;
			entry(ctx);
		}
		else{
			exit(ctx);
			new(this) waitingForStart;
			entry(ctx);
		}
	}

	void currentAck::eventItemGone(ErrorContext& ctx){
		if(ctx.data.err == ITEM_SPECIAL){
			ctx.data.removeItem(1);
			raiseItemGone(ctx);
			exit(ctx);
			new(this) waitingForStart;
			entry(ctx);
		}
		else if(ctx.data.err == ITEM_APPEARED && ctx.data.errorLoc == 1){
			ctx.data.heightDefault = true;
			if(!ctx.data.section1.empty()){
				uint32_t runnedTime = ctx.data.section1.front().timer->getTime();
				ctx.data.section1.front().timer->stop();
				std::cout<<"runnedTime:"<<runnedTime<<std::endl;
				delete ctx.data.section1.front().timer;
				ctx.data.section1.front().timer = new DynamicTimer(ctx.data.section1.front().id, *ctx.data.handler);
				ctx.data.section1.front().timer->start(ctx.data.entryTime+1000,runnedTime);
			}
			std::cout << ctx.data.heightDefault<< std::endl;
//			raiseItemGone(ctx);
			exit(ctx);
			new(this) waitingForStart;
			entry(ctx);
		}
	}
	void currentAck::eventItemLeftSlideRemote(ErrorContext& ctx){
		if(ctx.data.err == BOTH_SLIDES_FULL){
			raiseItemAtPusher(ctx);
			exit(ctx);
			new(this) waitingForStart;
			entry(ctx);
		}
	}
	void currentAck::errorPusherTimerExpired(ErrorContext& ctx){
		exit(ctx);
		new(this) errorPusherTooLong;
		entry(ctx);
	}

	void currentAck::eventItemLeftPusher(ErrorContext& ctx){
		if(ctx.data.err == ITEM_APPEARED && ctx.data.errorLoc == 2){
			if(!ctx.data.section2.empty()){
				uint32_t runnedTime = ctx.data.section2.front().timer->getTime();
				ctx.data.section2.front().timer->stop();
				std::cout<<"runnedTime:"<<runnedTime<<std::endl;
				delete ctx.data.section2.front().timer;
				ctx.data.section2.front().timer = new DynamicTimer(ctx.data.section2.front().id, *ctx.data.handler);
				ctx.data.section2.front().timer->start(ctx.data.pusherTime+1000,runnedTime);
			}
			exit(ctx);
			new(this) waitingForStart;
			entry(ctx);
		}
	}
	void currentAck::eventItemLeftExit(ErrorContext& ctx){
		if(ctx.data.err == ITEM_APPEARED && ctx.data.errorLoc == 3){
			if(!ctx.data.section3.empty()){
				uint32_t runnedTime = ctx.data.section3.front().timer->getTime();
				ctx.data.section3.front().timer->stop();
				std::cout<<"runnedTime:"<<runnedTime<<std::endl;
				delete ctx.data.section3.front().timer;
				ctx.data.section3.front().timer = new DynamicTimer(ctx.data.section3.front().id, *ctx.data.handler);
				ctx.data.section3.front().timer->start(ctx.data.exitTime+1000,runnedTime);
			}
			exit(ctx);
			new(this) waitingForStart;
			entry(ctx);
		}
	}
	void currentAck::eventConnectionRestored(ErrorContext& ctx){
		if(ctx.data.err == CONNECTION_LOST){
			exit(ctx);
			new(this) waitingForStart;
			entry(ctx);
		}
	}

	void currentAck::raiseItemAtPusher(ErrorContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_PUSHER_LOCAL, 0);
	}

	void currentAck::raiseItemGone(ErrorContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_GONE, 0);
	}
