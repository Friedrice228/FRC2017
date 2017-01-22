/*
 * Elevator.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */

#include <Elevator.h>

const int stop_state = 0;
const int elevate_state = 1;
const int reverse_state = 2;
const int CAN_TALON_ELEVATOR_TOP = 99;
const int CAN_TALON_ELEVATOR_BOTTOM = 30;
const double ELEVATE_SPEED = .5;
const double REVERSE_SPEED = .5;

Elevator::Elevator() {

	canTalonElevatorTop = new CANTalon(CAN_TALON_ELEVATOR_TOP);
	canTalonElevatorBottom = new CANTalon(CAN_TALON_ELEVATOR_BOTTOM);

}

void Elevator::Elevate(double speed) {

	canTalonElevatorTop->Set(speed);
	canTalonElevatorBottom->Set(speed);

}

void Elevator::Stop() {

	canTalonElevatorTop->Set(0);
	canTalonElevatorBottom->Set(0);

}

void Elevator::Reverse(double speed) {

	canTalonElevatorTop->Set(-speed);
	canTalonElevatorBottom->Set(-speed);

}

void Elevator::ElevatorStateMachine() {

	switch (elevator_state) {

	case stop_state:

		Stop();

		break;

	case elevate_state:

		Elevate(ELEVATE_SPEED);

		break;

	case reverse_state:

		Reverse(REVERSE_SPEED);

		break;

	}

}
