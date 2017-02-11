/*
 * Autonomous.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: DriversStation
 */

#include <Autonomous.h>

DriveController *drive_controller;


Autonomous::Autonomous(DriveController *drive_controller_pass) {

	drive_controller = drive_controller_pass;

}

void Autonomous::DriveForward() {

	int index = drive_controller->GetIndex();
	std::cout << index <<std::endl;

}

void Autonomous::GearPlacementUsualAuton() {

}

void Autonomous::GearPlacementLeft() {

}

void Autonomous::GearPlacementRight() {

}

void Autonomous::GearPlacementDriveAuton() {

}

void Autonomous::ShootAutonRed() {

}

void Autonomous::ShootAutonBlue() {

}

void Autonomous::ShootAndLoadAutonBlue() {

}

void Autonomous::ShootAndLoadAutonRed() {

}



