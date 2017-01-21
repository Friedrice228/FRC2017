/*
 * Flywheel.h
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <WPILib.h>
#include <CANTalon.h>

#ifndef SRC_FLYWHEEL_H_
#define SRC_FLYWHEEL_H_

class Flywheel {
public:

	const int stop_state_h = 0;
	const int spin_state_h = 1;
	int flywheel_state = stop_state_h;

	CANTalon *canTalonFlywheelLeft, *canTalonFlywheelRight;

	Flywheel();

	void Spin(int ref);
	void Stop();
	bool IsAtSpeed();
	void FlywheelStateMachine();
	void StartThread();
	static void SpinWrapper(Flywheel *fw, int ref, bool *active);
	double GetSpeed();

};

#endif
