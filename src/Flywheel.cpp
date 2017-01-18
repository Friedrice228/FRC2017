/*
 * Flywheel.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <Flywheel.h>

const int stop_state = 0;
const int spin_state = 1;
const double GOAL_RPM = 0;
const double MAX_FLYWHEEL_ERROR = 0;

Flywheel::Flywheel() {

	canTalonFlywheelRight = new CANTalon(23);
	canTalonFlywheelLeft = new CANTalon(24);

}

void Flywheel::spin(double ref){


}

void Flywheel::stop(){

	canTalonFlywheelRight->Set(0);
	canTalonFlywheelLeft->Set(0);

}
// current speed target speed variable
bool Flywheel::is_at_speed(){

	double flywheel_value = -((double)canTalonFlywheelRight->GetEncVel()/(double)4096) * 600;

	if(abs(flywheel_value - GOAL_RPM) < MAX_FLYWHEEL_ERROR) {

		return true;

	} else {

		return false;
	}


}

void Flywheel::flywheel_state_machine(){

	switch(flywheel_state) {

	case stop_state:

		stop();

		break;

	case spin_state:

		spin(GOAL_RPM);

		break;


	}
}

void Flywheel::start_thread(){

}
