/*
 * Flywheel.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <Flywheel.h>

Flywheel::Flywheel() {

	canTalonFlywheelRight = new CANTalon(23);
	canTalonFlywheelLeft = new CANTalon(24);
}

void Flywheel::spin(){

}

void Flywheel::stop(){

}

bool Flywheel::is_at_speed(){


	return true;
}

void Flywheel::flywheel_state_machine(){

}

void Flywheel::start_thread(){

}
