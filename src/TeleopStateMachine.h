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
#include <Climber.h>
#include <SmartDashboard/SmartDashboard.h>

#ifndef TELEOPSTATEMACHINE_H_
#define TELEOPSTATEMACHINE_H_

class TeleopStateMachine {
public:


	TeleopStateMachine(Flywheel *flywheelP, Conveyor *conveyorP, GearRail *gearRailP,
			Elevator *elevatorP, DriveController *driveControllerP, Vision *visionP, Climber *climberP);
	void StateMachine(bool is_gear, bool is_close_gear, bool is_fire, bool is_climb, bool is_ret, bool is_popcorn, bool second_fire_button, bool stop_shoot_button);
	void Initialize();





};

#endif
