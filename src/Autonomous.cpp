/*
 * Autonomous.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: DriversStation
 */

#include <Autonomous.h>

Autonomous::Autonomous() {

	//	Create new DriveController object, which contains drive code
	drive_controller = new DriveController();

}

//	Go forward, using the motion profile specified in /home/lvuser/MP.csv
void Autonomous::DriveForward() {

	//	Temporary 3D array that copies the motion profile from the motion profile file (ew necessary grammatical redundancy)
	double refs[300][5];  //l,r,k

	//	Counter to keep track of lines
	int r = 0;
	//	Take the motion profile file
	std::fstream file("/home/lvuser/MP.csv", std::ios::in);
	//	Look at each line
	while (r < 300) {

		//	Temporarily store data
		std::string data;
		std::getline(file, data);
		std::stringstream iss(data);
		//	Make sure the file exists
		if (!file.good()) {

			//	Printer says computer broken
			std::cout << "FAIL" << std::endl;
		}
		//	Copy each command in motion profile into our array
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

	//	GO GO GO
	drive_controller->SetRef(refs);
	drive_controller->StartAutonThreads();

}

//	Place gears under our standard autonomous routine
void Autonomous::GearPlacementUsualAuton() {

}

//	Place gears on the left
void Autonomous::GearPlacementLeft() {

}

//	Place gears on the right
void Autonomous::GearPlacementRight() {

}

//	Not sure tbh I just made the last 3 up
void Autonomous::GearPlacementDriveAuton() {

}

//	Now I'm totally lost but hey this comment exists MAX REPLACE IT WITH A COHERENT COMMENT
void Autonomous::ShootAutonRed() {

}

//	Foo
void Autonomous::ShootAutonBlue() {

}

//	Bar
void Autonomous::ShootAndLoadAutonBlue() {

}

//	Baz
void Autonomous::ShootAndLoadAutonRed() {

}

