/*
 * GearRail.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */

#include "GearRail.h"
#include <WPILib.h> //TODO: needs to be included in both for reasons i do not know

GearRail::GearRail() {

	gearRailPistonLeft = new DoubleSolenoid(4, 4 , 5);
	gearRailPistonRight = new DoubleSolenoid(4, 3, 2);


}

void GearRail::open(){

}

void GearRail::close(){

}

void GearRail::gear_rail_state_machine(){

}
