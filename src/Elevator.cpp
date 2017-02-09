/*
 * Elevator.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */

#include <Elevator.h>

//	State machine constants for elevating, lowering, and stopping the cogs
const int stop_state = 0;
const int elevate_state = 1;
const int reverse_state = 2;
//	CANTalon reference numbers
const int CAN_TALON_ELEVATOR_TOP = 99;
const int CAN_TALON_ELEVATOR_BOTTOM = 30;
//	Our max speeds
const double ELEVATE_SPEED = .5;
const double REVERSE_SPEED = .5;

//	init
Elevator::Elevator() {

	//	Initialize the CANTalons
	canTalonElevatorTop = new CANTalon(CAN_TALON_ELEVATOR_TOP);
	canTalonElevatorBottom = new CANTalon(CAN_TALON_ELEVATOR_BOTTOM);

}

//	Lift up the cog
void Elevator::Elevate(double speed) {

	//	Set the elevator to go up to speed
	canTalonElevatorTop->Set(speed);
	canTalonElevatorBottom->Set(speed);

}

//	Stop!
void Elevator::Stop() {

	//	No, no. Stop moving!
	canTalonElevatorTop->Set(0);
	canTalonElevatorBottom->Set(0);

}

//	It's going down, the elevatoooorrrrrrr! We got a cog, we'll put it down....
void Elevator::Reverse(double speed) {

	//	Pants on the ground, pants on the ground
	canTalonElevatorTop->Set(-speed);
	canTalonElevatorBottom->Set(-speed);

}

//	State machine for the elevator
void Elevator::ElevatorStateMachine() {

	switch (elevator_state) {

	//	Do we want it to stop? If so, go to page Stop()
	case stop_state:

		Stop();

		break;

	//	Your journey continues on Elevate()
	case elevate_state:

		Elevate(ELEVATE_SPEED);

		break;

	//	If dead, see Reverse()
	case reverse_state:

		Reverse(REVERSE_SPEED);

		break;

	}

}
