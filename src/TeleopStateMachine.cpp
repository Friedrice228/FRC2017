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
Climber *climber_;

TeleopStateMachine::TeleopStateMachine(Flywheel *flywheelP, Conveyor *conveyorP,
		GearRail *gearRailP, Elevator *elevatorP,
		DriveController *driveControllerP, Vision *visionP, Climber *climberP) {

	fly_wheel = flywheelP;

	conveyor_ = conveyorP;

	gear_rail = gearRailP;

	elevator_ = elevatorP;

	drive_Controller = driveControllerP;

	vision_ = visionP;

	climber_ = climberP;

}

void TeleopStateMachine::StateMachine(bool is_gear, bool is_fire, bool is_climb,
		bool is_ret, bool is_popcorn) {

	if (is_popcorn) {

		conveyor_->conveyor_state = conveyor_->popcorn_state_h;

	} else {

		conveyor_->conveyor_state = conveyor_->stop_state_h;

	}

	switch (state) {

	case init_state:

		conveyor_->conveyor_state = conveyor_->stop_state_h;

		gear_rail->gear_rail_state = gear_rail->close_state_h;

		elevator_->elevator_state = elevator_->stop_state_h;

		state = wait_for_button_state;

		break;

	case wait_for_button_state: // can only do one thing at a time - sean wanted this

		gear_rail->gear_rail_state = gear_rail->close_state_h;

		elevator_->elevator_state = elevator_->stop_state_h;

		if (is_gear) {

			state = open_gear_rails_state;

		} else if (is_fire) {

			state = init_shooting_state;

		} else if (is_climb) {

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

		fly_wheel->flywheel_state = fly_wheel->spin_state_h; //TODO: if flywheel doesn't spin up, then check this

		if (!is_fire) {

			state = wait_for_button_state;

		}

		if (fly_wheel->IsAtSpeed()) {

			state = fire_state;
		}

		break;

	case fire_state:

		if (is_popcorn) {

			conveyor_->conveyor_state = conveyor_->popcorn_state_h;

		} else {

			conveyor_->conveyor_state = conveyor_->load_state_h;

		}

		elevator_->elevator_state = elevator_->elevate_state_h;

		if (!is_fire) {

			state = wait_for_button_state;

		}

		break;

	case init_climbing:

		elevator_->elevator_state = elevator_->stop_state_h;

		gear_rail->gear_rail_state = gear_rail->close_state_h;

		fly_wheel->flywheel_state = fly_wheel->stop_state_h;

		state = climbing_state;

		break;

	case climbing_state: //hold to climb, release to stop

		climber_->climber_state = climber_->climbing_state_h;

		if (!is_climb) {

			state = wait_for_button_state;
		}

		if (climber_->CheckCurrent() >= climber_->MAX_CURRENT) {

			state = finish_climbing;

		}

		break;

	case finish_climbing:

		climber_->climber_state = climber_->stop_state_h;

		if (is_ret) {

			state = wait_for_button_state;

		}

		break;

	}

}

