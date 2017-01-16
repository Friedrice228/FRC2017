/*
 * TeleopStateMachine.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <TeleopStateMachine.h>

const std::string maxs_korean_name = "Ku Sung";

const int init_state = 0;
const int wait_for_button_state = 1;
const int open_gear_rails_state = 2;
const int init_shooting_state = 3;
const int fire_state = 4;
const int init_climbing = 5;
const int climbing_state = 6;
const int finish_climbing = 7;
int state = init_state;


DriveController *drive_Controller;
Flywheel *fly_wheel;
Elevator *elevator_;
GearRail *gear_rail;
Conveyor *conveyor_;
Vision *vision_;

TeleopStateMachine::TeleopStateMachine(Flywheel *flywheelP, Conveyor *conveyorP, GearRail *gearRailP,
		Elevator *elevatorP, DriveController *driveControllerP, Vision *visionP) {

	fly_wheel = flywheelP;

	conveyor_ = conveyorP;

	gear_rail = gearRailP;

	elevator_ = elevatorP;

	drive_Controller = driveControllerP;

	vision_ = visionP;

}

void TeleopStateMachine::StateMachine(bool is_gear, bool is_fire, bool is_climb){

	switch(state) {

	case init_state:

		conveyor_->conveyor_state = conveyor_->popcorn_state_h;

		gear_rail->gear_rail_state = gear_rail->close_state_h;

		elevator_->elevator_state = elevator_->stop_state_h;

		state = wait_for_button_state;

		break;

	case wait_for_button_state:

		conveyor_->conveyor_state = conveyor_->popcorn_state_h;

		gear_rail->gear_rail_state = gear_rail->close_state_h;

		elevator_->elevator_state = elevator_->stop_state_h;

		if (is_gear) {

			state = open_gear_rails_state;

		}else if (is_fire) {

			state = init_shooting_state;

		}else if (is_climb) {

			state = init_climbing;

		}

		break;

	case open_gear_rails_state:

		gear_rail->gear_rail_state = gear_rail->open_state_h;

		if (!is_gear) {

			state = wait_for_button_state;

		}

		break;

	case init_shooting_state:



		break;

	case fire_state:



		break;

	case init_climbing:



		break;

	case climbing_state:



		break;

	case finish_climbing:



		break;


	}

}

