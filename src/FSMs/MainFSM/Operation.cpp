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
#include "ConnectionLost.h"
#include "../EstopFSM/EstopBase.h"
#include "../EstopFSM/EstopContext.h"
#include "../ErrorFSM/ErrorContext.h"
#include "../ErrorFSM/ErrorBase.h"
#include <iostream>



void Operation::entry(MainContext& ctx) {}
void Operation::exit(MainContext& ctx)  {
	ctx.hal.getActors()->greenLampOff();
}

void Operation::eventButtonStartPressed(MainContext& ctx) {
    operationFSM.eventButtonStartPressed();
}
void Operation::eventButtonStartPressedRemote(MainContext& ctx) {
    operationFSM.eventButtonStartPressedRemote();
}


void Operation::eventButtonStartPressedLong(MainContext& ctx) {
    operationFSM.eventButtonStartPressedLong();
}
void Operation::eventButtonStartPressedLongRemote(MainContext& ctx) {
    operationFSM.eventButtonStartPressedLongRemote();
}

void Operation::eventButtonStopPressed(MainContext& ctx) {
    operationFSM.eventButtonStopPressed();
}
void Operation::eventButtonStopPressedRemote(MainContext& ctx) {
    operationFSM.eventButtonStopPressedRemote();
}

void Operation::eventButtonResetPressed(MainContext& ctx) {
    operationFSM.eventButtonResetPressed();
}
void Operation::eventButtonResetPressedRemote(MainContext& ctx) {
	operationFSM.eventButtonResetPressedRemote();
}
void Operation::eventButtonEstopPressed(MainContext& ctx) {
	ctx.data.reset();
	ctx.getOperationFSM().saveHistory(HistorySource::ESTOP_HISTORY);
    exit(ctx);
    new(this) EStop(ctx.hal, ctx.hs, ctx.data, EStopSource::LOCAL);
    entry(ctx);
}
void Operation::eventButtonEstopPressedRemote(MainContext& ctx) {
	ctx.data.reset();
	ctx.getOperationFSM().saveHistory(HistorySource::ESTOP_HISTORY);
    exit(ctx);
    new(this) EStop(ctx.hal, ctx.hs, ctx.data, EStopSource::REMOTE);
    entry(ctx);
}
void Operation::eventItemAtEntry(MainContext& ctx) {
    operationFSM.eventItemAtEntry();
}
void Operation::eventItemAtEntryRemote(MainContext& ctx) {
    operationFSM.eventItemAtEntryRemote();
}

void Operation::eventItemLeftEntry(MainContext& ctx) {
    operationFSM.eventItemLeftEntry();
}
void Operation::eventItemLeftEntryRemote(MainContext& ctx) {
    operationFSM.eventItemLeftEntryRemote();
}

void Operation::eventItemAtExit(MainContext& ctx) {
    operationFSM.eventItemAtExit();
}
void Operation::eventItemAtExitRemote(MainContext& ctx) {
    operationFSM.eventItemAtExitRemote();
}

void Operation::eventItemLeftExit(MainContext& ctx) {
    operationFSM.eventItemLeftExit();
}
void Operation::eventItemLeftExitRemote(MainContext& ctx) {
    operationFSM.eventItemLeftExitRemote();
}

void Operation::eventItemAtPusher(MainContext& ctx) {
    operationFSM.eventItemAtPusher();
}

void Operation::eventItemAtPusherRemote(MainContext& ctx) {
    operationFSM.eventItemAtPusherRemote();
}

void Operation::eventItemLeftPusher(MainContext& ctx) {
    operationFSM.eventItemLeftPusher();
}
void Operation::eventItemLeftPusherRemote(MainContext& ctx) {
    operationFSM.eventItemLeftPusherRemote();
}

void Operation::eventItemInSlide(MainContext& ctx) {
    operationFSM.eventItemInSlide();
}
void Operation::eventItemInSlideRemote(MainContext& ctx) {
    operationFSM.eventItemInSlideRemote();
}
void Operation::eventItemLeftSlide(MainContext& ctx) {
    operationFSM.eventItemLeftSlide();
}
void Operation::eventItemLeftSlideRemote(MainContext& ctx) {
    operationFSM.eventItemLeftSlideRemote();
}
void Operation::eventTransportTimerExpired(MainContext& ctx) {
    operationFSM.eventTransportTimerExpired();
}
void Operation::eventItemMetal(MainContext& ctx){
	operationFSM.eventItemMetal();
}
void Operation::eventItemMetalRemote(MainContext& ctx) {
    operationFSM.eventItemMetalRemote();
}
void Operation::eventError(MainContext& ctx) {
	ctx.getOperationFSM().saveHistory(HistorySource::ERROR_HISTORY);
    exit(ctx);
    new(this) Error (ctx.hal,ctx.data);
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


void Operation::eventSlideFull(MainContext& ctx) {
    operationFSM.eventSlideFull();
}
void Operation::eventSlideFullRemote(MainContext& ctx) {
    operationFSM.eventSlideFullRemote();
}
void Operation::eventSlideTimerExpired(MainContext& ctx) {
    operationFSM.eventSlideTimerExpired();
}
void Operation::eventGoBack(MainContext& ctx){
	operationFSM.eventGoBack();
}

void Operation::eventGoBackWFP(MainContext& ctx){
	operationFSM.eventGoBackWFP();
}
void Operation::eventGoBackIH(MainContext& ctx){
	operationFSM.eventGoBackIH();
}
void Operation::eventGoBackIM(MainContext& ctx){
	operationFSM.eventGoBackIM();
}
void Operation::eventGoBackIF(MainContext& ctx){
	operationFSM.eventGoBackIF();
}
void Operation::eventGoBackIHM(MainContext& ctx){
	operationFSM.eventGoBackIHM();
}

void Operation::errorPusherTimerExpired(MainContext& ctx){
	ctx.getOperationFSM().saveHistory(HistorySource::ERROR_HISTORY);
    exit(ctx);
    Error* error = new(this) Error (ctx.hal,ctx.data);
    error->enterPusherTooLong();
    entry(ctx);
}

void Operation::eventConnectionLost(MainContext& ctx){
	ctx.getOperationFSM().saveHistory(HistorySource::ERROR_HISTORY); // Error History?
	exit(ctx);
	new(this) ConnectionLost(ctx.hal, ctx.hs, PreviousFsmState::OPERATION);
	entry(ctx);
}

void Operation::eventSkipUpdate(MainContext& ctx){
	operationFSM.eventSkipUpdate();
}

