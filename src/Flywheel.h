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

	void spin(double ref);
	void stop();
	bool is_at_speed();
	void flywheel_state_machine();
	void start_thread();


};

#endif
