/*
 * GearRail.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */

#include "GearRail.h"
#include <WPILib.h> //TODO: needs to be included in both for reasons i do not know

const int close_state = 0;
const int open_state = 1;

GearRail::GearRail() {

	gearRailPistonLeft = new DoubleSolenoid(4, 4, 5);
	gearRailPistonRight = new DoubleSolenoid(4, 3, 2);



}

void GearRail::open() {

	gearRailPistonLeft->Set(DoubleSolenoid::Value::kReverse);
	gearRailPistonRight->Set(DoubleSolenoid::Value::kReverse);

}

void GearRail::close() {

	gearRailPistonLeft->Set(DoubleSolenoid::Value::kForward);
	gearRailPistonRight->Set(DoubleSolenoid::Value::kForward);

}

void GearRail::gear_rail_state_machine() {

	switch (gear_rail_state) {

	case close_state:

		void close();

		break;

	case open_state:

		void open();

		break;

	}
}
