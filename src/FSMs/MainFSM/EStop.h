/*
 * EStop.h
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_MAINFSM_ESTOP_H_
#define SRC_FSMS_MAINFSM_ESTOP_H_

#include "MainBase.h"

class EStop : public MainBase {
public:
    void entry(MainContext& ctx) override;
    void exit(MainContext& ctx) override;
    void eventButtonResetPressed(MainContext& ctx) override;
};

#endif
