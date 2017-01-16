/*
 * Conveyor.h
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <WPILib.h>
#include <CANTalon.h>

#ifndef SRC_CONVEYOR_H_
#define SRC_CONVEYOR_H_

class Conveyor {
public:

	CANTalon *canTalonBack, *canTalonFront;

	Conveyor();

	void popcorn(double speed);
	void stop();
	void load(double speed);
	void con_state_machine();

};

#endif
