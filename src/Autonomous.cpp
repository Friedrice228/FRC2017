/*
 * Autonomous.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: DriversStation
 */

#include <Autonomous.h>

Autonomous::Autonomous() {

	drive_controller = new DriveController();

}

void Autonomous::DriveForward() {

	double refs[300][5];  //l,r,k

	int r = 0;
	std::fstream file("/home/lvuser/MP.csv", std::ios::in);
	while (r < 300) {
		std::string data;
		std::getline(file, data);
		std::stringstream iss(data);
		if (!file.good()) {
			std::cout << "FAIL" << std::endl;
		}
		int i = 0;
		while (i < 5) {
			std::string val;
			std::getline(iss, val, ',');
			std::stringstream convertor(val);
			convertor >> refs[r][i];
			i++;
		}
		r++;
	}

	drive_controller->SetRef(refs);
	drive_controller->StartAutonThreads();

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

