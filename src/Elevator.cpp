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

Elevator::Elevator() {

	canTalonElevatorTop = new CANTalon(29);
	canTalonElevatorBottom = new CANTalon(30);

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

		Elevate(.5);

		break;

	case reverse_state:

		Reverse(.5);

		break;

	}

}
