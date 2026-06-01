/*
 * Operation.cpp
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#include "MainContext.h"
#include "Operation.h"
#include "EStop.h"
#include "Error.h"

void Operation::entry(MainContext& ctx) {}
void Operation::exit(MainContext& ctx)  {}

void Operation::eventButtonStartPressed(MainContext& ctx) {
    operationFSM.eventButtonStartPressed();
}
void Operation::eventButtonStartPressedLong(MainContext& ctx) {
    operationFSM.eventButtonStartPressedLong();
}
void Operation::eventButtonStopPressed(MainContext& ctx) {
    operationFSM.eventButtonStopPressed();
}
void Operation::eventButtonResetPressed(MainContext& ctx) {
    operationFSM.eventButtonResetPressed();
}
void Operation::eventButtonEstopPressed(MainContext& ctx) {
    exit(ctx);
    new(this) EStop;
    entry(ctx);
}
void Operation::eventItemAtEntry(MainContext& ctx) {
    operationFSM.eventItemAtEntry();
}

void Operation::eventItemLeftEntry(MainContext& ctx) {
    operationFSM.eventItemLeftEntry();
}
void Operation::eventItemAtExit(MainContext& ctx) {
    operationFSM.eventItemAtExit();
}
void Operation::eventItemLeftExit(MainContext& ctx) {
    operationFSM.eventItemLeftExit();
}

void Operation::eventItemAtPusher(MainContext& ctx) {
    operationFSM.eventItemAtExit();
}
void Operation::eventItemLeftPusher(MainContext& ctx) {
    operationFSM.eventItemLeftExit();
}

void Operation::eventItemInSlide(MainContext& ctx) {
    operationFSM.eventItemInSlide();
}
void Operation::eventItemLeftSlide(MainContext& ctx) {
    operationFSM.eventItemLeftSlide();
}

void Operation::eventItemMetal(MainContext& ctx){
	operationFSM.eventItemMetal();
}
void Operation::eventError(MainContext& ctx) {
    exit(ctx);
    //new(this) Error;
    entry(ctx);
}
