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

	gearRailPistonLeft = new DoubleSolenoid(4,2, 3);   //(4, 4, 5);
	gearRailPistonRight = new DoubleSolenoid(4, 0, 1); //(4, 3, 2);



}

void GearRail::Open() {

	gearRailPistonLeft->Set(DoubleSolenoid::Value::kReverse);
	gearRailPistonRight->Set(DoubleSolenoid::Value::kReverse);

}

void GearRail::Close() {

	gearRailPistonLeft->Set(DoubleSolenoid::Value::kForward);
	gearRailPistonRight->Set(DoubleSolenoid::Value::kForward);

}

void GearRail::GearRailStateMachine() {

	switch (gear_rail_state) {

	case close_state:

		Close();

		break;

	case open_state:

		Open();

		break;

	}
}
