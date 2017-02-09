/*
 * GearRail.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */

#include "GearRail.h"
#include <WPILib.h> //TODO: needs to be included in both for reasons i do not know

//	State machine options
const int close_state = 0;
const int open_state = 1;

//	init
GearRail::GearRail() {

	//	Pistons that will open or close for our gear
	gearRailPistonLeft = new DoubleSolenoid(4, 2, 3);   //(4, 4, 5);
	gearRailPistonRight = new DoubleSolenoid(4, 0, 1); //(4, 3, 2);

}

//	Open up our arms for the gear
void GearRail::Open() {

	//	Open wide...CHEESE
	gearRailPistonLeft->Set(DoubleSolenoid::Value::kReverse);
	gearRailPistonRight->Set(DoubleSolenoid::Value::kReverse);

}

//	Embrace our inner gear
void GearRail::Close() {

	//	Awww, don't close up like that. It isn't healthy :/
	gearRailPistonLeft->Set(DoubleSolenoid::Value::kForward);
	gearRailPistonRight->Set(DoubleSolenoid::Value::kForward);

}

//	State machine to open or close our feelings to others
void GearRail::GearRailStateMachine() {

	switch (gear_rail_state) {

	//	Be shy to the gears
	case close_state:

		Close();

		break;

	//	Let others inside our robot
	case open_state:

		Open();

		break;

	}
}
