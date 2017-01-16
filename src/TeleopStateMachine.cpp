/*
 * TeleopStateMachine.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <TeleopStateMachine.h>

DriveController *drive_Controller;
Flywheel *fly_wheel;
Elevator *elevator_;
GearRail *gear_Rail;
Conveyor *conveyor_;
Vision *vision_;

TeleopStateMachine::TeleopStateMachine(Flywheel *flywheelP, Conveyor *conveyorP, GearRail *gearRailP,
		Elevator *elevatorP, DriveController *driveControllerP, Vision *visionP) {

	fly_wheel = flywheelP;

	conveyor_ = conveyorP;

	gear_Rail = gearRailP;

	elevator_ = elevatorP;

	drive_Controller = driveControllerP;

	vision_ = visionP;

}

