/*
 * GearRail.h
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <WPILib.h>

#ifndef GEARRAIL_H_
#define GEARRAIL_H_

class GearRail {
public:

	//	State machine options
	const int close_state_h = 0;
	const int open_state_h = 1;
	//	Default state closed
	int gear_rail_state = close_state_h;

	//	Solenoids
	DoubleSolenoid *gearRailPistonRight, *gearRailPistonLeft;

	//	Functions
	GearRail();

	void Open();
	void Close();
	void GearRailStateMachine();

};

#endif
