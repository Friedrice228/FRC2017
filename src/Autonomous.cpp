/*
 * Autonomous.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: DriversStation
 */

#include <Autonomous.h>

const int NUM_POINTS = 1500;
const int NUM_INDEX = 12;

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

	if(refs[index][7] == 1.0){ // piston
		gear_rail_au->gear_rail_state = gear_rail_au->open_state_h;
	}else{
		gear_rail_au->gear_rail_state = gear_rail_au->close_state_h;
	}

	if(refs[index][8] == 1.0){ //Flywheel
		fly_wheel_au->flywheel_state = fly_wheel_au->spin_state_h;
	}else{
		fly_wheel_au->flywheel_state = fly_wheel_au->stop_state_h;
	}

	if(refs[index][9] == 1.0){ //Conveyor
		conveyor_au->conveyor_state = conveyor_au->load_state_h;
	}else{
		conveyor_au->conveyor_state = conveyor_au->stop_state_h;
	}

	if(refs[index][10] == 1.0){ //Elevator
		elevator_au->elevator_state = elevator_au->elevate_state_h;
	}else{
		elevator_au->elevator_state = elevator_au->stop_state_h;
	}

}


void Autonomous::FillProfile(std::string profileName) { //fill array

	int r = 0;
	std::fstream file(profileName, std::ios::in);
	while (r < NUM_POINTS) {
		std::string data;
		std::getline(file, data);
		std::stringstream iss(data);
		if (!file.good()) {
			std::cout << "FAIL" << std::endl;
		}
		int i = 0;
		while (i < NUM_INDEX) {
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

