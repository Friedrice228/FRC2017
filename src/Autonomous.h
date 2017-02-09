
#include <WPILib.h>
#include <DriveController.h>
#include <fstream>
#include <iostream>

#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_

class Autonomous {
public:

	DriveController *drive_controller;

	Autonomous();

	void GearPlacementUsualAuton();
	void GearPlacementDriveAuton();
	void GearPlacementLeft();
	void GearPlacementRight();
	void ShootAutonRed();
	void ShootAutonBlue();
	void ShootAndLoadAutonBlue();
	void ShootAndLoadAutonRed();
	void DriveForward();



};

#endif
