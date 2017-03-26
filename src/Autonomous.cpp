/*
 * Autonomous.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: DriversStation
 */

#include <Autonomous.h>

const int NUM_POINTS = 1500;
const int NUM_INDEX = 13;
const int PISTON_INDEX = 8;
const int FLYWHEEL_INDEX = 9;
const int CONVEYOR_INDEX = 10;
const int ELEVATOR_INDEX = 11;

double refs[NUM_POINTS][NUM_INDEX];

DriveController *drive_controller;
Elevator *elevator_au;
Conveyor *conveyor_au;
GearRail *gear_rail_au;
Flywheel *fly_wheel_au;

Autonomous::Autonomous(DriveController *drive_controller_pass, Elevator *elevator_pass, Conveyor *conveyor_pass, GearRail *gear_rail_pass, Flywheel *fly_wheel_pass) {

	drive_controller = drive_controller_pass;
	elevator_au = elevator_pass;
	conveyor_au = conveyor_pass;
	gear_rail_au = gear_rail_pass;
	fly_wheel_au = fly_wheel_pass;

}

void Autonomous::RunAuton() { // runs continuously through all autonomous modes

	int index = drive_controller->GetIndex();

	if(refs[index][PISTON_INDEX] == 1){ // piston
		gear_rail_au->gear_rail_state = gear_rail_au->open_state_h;
	}else{
		gear_rail_au->gear_rail_state = gear_rail_au->close_state_h;
	}

	if(refs[index][FLYWHEEL_INDEX] == 1){ //Flywheel
		fly_wheel_au->flywheel_state = fly_wheel_au->spin_state_h;
	}else{
		fly_wheel_au->flywheel_state = fly_wheel_au->stop_state_h;
	}

	if(refs[index][CONVEYOR_INDEX] == 1){ //Conveyor
		conveyor_au->conveyor_state = conveyor_au->load_state_h;
	}else{
		conveyor_au->conveyor_state = conveyor_au->stop_state_h;
	}

	if(refs[index][ELEVATOR_INDEX] == 1){ //Elevator
		elevator_au->elevator_state = elevator_au->elevate_state_h;
	}else{
		elevator_au->elevator_state = elevator_au->stop_state_h;
	}

}


void Autonomous::FillProfile(std::string profileName) { //fill array and run auton

	for (int r = 0; r < NUM_POINTS; r++){ //sets the entire array to 0 so that all the points that arent filled are zeros, easy to check for
		for (int c = 0; c < NUM_INDEX; c++){
			refs[r][c] = 0;
		}
	}

	int r = 0;
	std::fstream file(profileName, std::ios::in);
	while (r < NUM_POINTS) {
		std::string data;
		std::getline(file, data);
		std::stringstream iss(data);
		if (!file.good()) {
			std::cout << "FAIL" << std::endl;
		}
		int c = 0;
		while (c < NUM_INDEX) {
			std::string val;
			std::getline(iss, val, ',');
			std::stringstream convertor(val);
			convertor >> refs[r][c];
			c++;
			if (file.eof()){
			//	drive_controller->SetProfileLength(r); //sets array length to length of csv file
			}
		}
		r++;
	}

	drive_controller->SetRef(refs);
	drive_controller->StartAutonThreads();



}

