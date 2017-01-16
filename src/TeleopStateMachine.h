/*
 * TeleopStateMachine.h
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <WPILib.h>
#include <DriveController.h>
#include <Flywheel.h>
#include <Elevator.h>
#include <GearRail.h>
#include <Conveyor.h>
#include <Vision.h>

#ifndef TELEOPSTATEMACHINE_H_
#define TELEOPSTATEMACHINE_H_

class TeleopStateMachine {
public:


	TeleopStateMachine(Flywheel *flywheelP, Conveyor *conveyorP, GearRail *gearRailP,
			Elevator *elevatorP, DriveController *driveControllerP, Vision *visionP);
	void StateMachine(bool is_gear, bool is_fire, bool is_climb);





};

#endif
