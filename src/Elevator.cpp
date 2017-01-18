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

void Elevator::elevate(double speed) {

	canTalonElevatorTop->Set(speed);
	canTalonElevatorBottom->Set(speed);

}

void Elevator::stop() {

	canTalonElevatorTop->Set(0);
	canTalonElevatorBottom->Set(0);

}

void Elevator::reverse(double speed) {

	canTalonElevatorTop->Set(-speed);
	canTalonElevatorBottom->Set(-speed);

}

void Elevator::elevator_state_machine() {

	switch (elevator_state) {

	case stop_state:

		stop();

		break;

	case elevate_state:

		elevate(.5);

		break;

	case reverse_state:

		reverse(.5);

		break;

	}

}
