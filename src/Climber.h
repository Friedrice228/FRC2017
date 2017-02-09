/*
 * Climber.h
 *
 *  Created on: Jan 17, 2017
 *      Author: DriversStation
 */
#include <WPILib.h>
#include <CANTalon.h>

#ifndef SRC_CLIMBER_H_
#define SRC_CLIMBER_H_

class Climber {
public:

	//	Climber CANTalons
	CANTalon *canTalonClimberLeft, *canTalonClimberRight;

	//	FUNCTIONS (not methods)
	Climber();
	void Climbing(double speed);
	void Stop();
	void ClimberStateMachine();
	double CheckCurrent();

	//	The max output current we want from the CANTalons
	const int MAX_CURRENT = 15.0;

	//	State machine constants just in case
	const int stop_state_h = 0;
	const int climbing_state_h = 1;

	//	Current state
	int climber_state = stop_state_h;

};

#endif
