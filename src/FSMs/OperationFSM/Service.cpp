#include <new>
#include "OperationBase.h"
#include "OperationContext.h"
#include "Service.h"
#include "Idle.h"
#include "ServiceMode/Calibration.h"
#include <iostream>

Service::Service(){
	}

	void Service::entry(OperationContext& ctx){
		ctx.hal.getActors()->greenLampBlink(1000);
		ctx.hal.getActors()->yellowLampOff();
		ctx.hal.getActors()->ledButtonStart();
		ctx.hal.getActors()->ledButtonReset();
		ctx.hs.stopCalibration(); //testen!!!
		std::cout << "Service" << std::endl;
	}

	void Service::exit(OperationContext& ctx){}

	void Service::eventButtonStartPressedLong(OperationContext& ctx){
		exit(ctx);
		new (this) Idle;
		entry(ctx);
	}

	void Service::eventButtonStartPressedLongRemote(OperationContext& ctx){
		exit(ctx);
		new (this) Idle;
		entry(ctx);
	}

	void Service::eventButtonResetPressed(OperationContext& ctx){
		exit(ctx);
		ctx.hs.startCalibration();
		new (this) Calibration;
		entry(ctx);
	}

	void Service::eventButtonResetPressedRemote(OperationContext& ctx){
		exit(ctx);
		ctx.hs.startCalibration();
		new (this) Calibration;
		entry(ctx);
	}

	void Service::eventItemAtEntry(OperationContext& ctx){
		ctx.hal.getActors()->motorForwards();
	}

	void Service::eventItemArrived(OperationContext& ctx){
		ctx.hal.getActors()->motorSlow();
	}

	void Service::eventItemGone(OperationContext& ctx){
		ctx.hal.getActors()->motorForwards();
	}

	void Service::eventItemAtPusher(OperationContext& ctx){
			ctx.hal.getActors()->letItemPass();
	}

	void Service::eventItemAtExit(OperationContext& ctx){
			ctx.hal.getActors()->motorStop();
	}

