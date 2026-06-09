/*
 * Operation.cpp
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */
#include <new>
#include "MainContext.h"
#include "Operation.h"
#include "EStop.h"
#include "Error.h"
#include "../EstopFSM/EstopBase.h"
#include "../EstopFSM/EstopContext.h"


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
void Operation::eventButtonResetPressedRemote(MainContext& ctx) {
}
void Operation::eventButtonEstopPressed(MainContext& ctx) {
    exit(ctx);
    new(this) EStop(ctx.hal, EStopSource::LOCAL);
    entry(ctx);
}
void Operation::eventButtonEstopPressedRemote(MainContext& ctx) {
    exit(ctx);
    new(this) EStop(ctx.hal, EStopSource::REMOTE);
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
    operationFSM.eventItemAtPusher();
}
void Operation::eventItemLeftPusher(MainContext& ctx) {
    operationFSM.eventItemLeftPusher();
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
void Operation::eventItemGone(MainContext& ctx){
	operationFSM.eventItemGone();
}

void Operation::eventItemArrived(MainContext& ctx){
	operationFSM.eventItemArrived();
}

void Operation::eventFestoReady(MainContext& ctx){
	operationFSM.eventFestoReady();
}

void Operation::eventFestoNotReady(MainContext& ctx){
	operationFSM.eventFestoNotReady();
}

void Operation::eventHeightHigh(MainContext& ctx){
	operationFSM.eventHeightHigh();
}
void Operation::eventHeightFlat(MainContext& ctx){
	operationFSM.eventHeightFlat();
}
void Operation::eventHeightHole(MainContext& ctx){
	operationFSM.eventHeightHole();
}
void Operation::eventHeightWrong(MainContext& ctx){
	operationFSM.eventHeightWrong();
}
void Operation::eventItemWrong(MainContext& ctx){
	operationFSM.eventItemWrong();
}
void Operation::eventItemCorrect(MainContext& ctx){
	operationFSM.eventItemCorrect();
}
void Operation::eventUpdate(MainContext& ctx){
	operationFSM.eventUpdate();
}

void Operation::eventTimerExpired(MainContext& ctx){
	operationFSM.eventTimerExpired();
}



