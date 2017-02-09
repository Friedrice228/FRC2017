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

	//	Our state options
	const int popcorn_state_h = 0;
	const int load_state_h = 1;
	const int stop_state_h = 2;
	//	Current state
	int conveyor_state = stop_state_h;

	//	CANTalons
	CANTalon *canTalonBack, *canTalonFront;

	//	Function exclamations
	Conveyor();

	void Popcorn(double speed);
	void Stop();
	void Load(double speed);
	void ConStateMachine();

};

#endif
