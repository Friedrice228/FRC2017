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

	CANTalon *canTalonClimberLeft, *canTalonClimberRight;

	Climber();
	void Climbing(double speed);
	void Stop();
	void ClimberStateMachine();
	double CheckCurrent();

	const int MAX_CURRENT = 25.0;

	const int stop_state_h = 0;
	const int climbing_state_h = 1;

	int climber_state = stop_state_h;

};

#endif
