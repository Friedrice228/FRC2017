/*
 * Elevator.h
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <WPILib.h>
#include <CANTalon.h>

#ifndef SRC_ELEVATOR_H_
#define SRC_ELEVATOR_H_

class Elevator {
public:

	CANTalon *canTalonElevatorTop, *canTalonElevatorBottom;

	Elevator();

	void elevate(double speed);
	void stop();
	void reversed(double speed);
	void elevator_state_machine();

};

#endif
