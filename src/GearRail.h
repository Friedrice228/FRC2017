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

	DoubleSolenoid *gearRailPistonRight, *gearRailPistonLeft;

	GearRail();

	void open();
	void close();
	void gear_rail_state_machine();

};

#endif
