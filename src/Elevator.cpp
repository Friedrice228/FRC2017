/*
 * Elevator.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */

#include <Elevator.h>

Elevator::Elevator() {

	canTalonElevatorTop = new CANTalon(11);
	canTalonElevatorBottom = new CANTalon(12);

}

void Elevator::elevate(double speed){

}

void Elevator::stop(){

}

void Elevator::reversed(double speed){

}

void Elevator::elevator_state_machine(){

}
