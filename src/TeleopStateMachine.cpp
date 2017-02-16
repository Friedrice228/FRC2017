/*
 * TeleopStateMachine.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <TeleopStateMachine.h>
#include <iostream>

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
bool is_ret, bool is_popcorn, bool second_fire_button) {

	if (fly_wheel->IsAtSpeed()) {

		SmartDashboard::PutBoolean("At Speed", true);

	} else {

		SmartDashboard::PutBoolean("At Speed", false);

	}

	if (is_popcorn) {

		elevator_->elevator_state = elevator_->reverse_state_h;
		//conveyor_->conveyor_state = conveyor_->popcorn_state_h; // not using conveyor anymore

	} else if (!is_popcorn && state != fire_state) {

		elevator_->elevator_state = elevator_->stop_state_h;
		//conveyor_->conveyor_state = conveyor_->stop_state_h;

	}

	if (is_gear) {

		gear_rail->gear_rail_state = gear_rail->open_state_h;

	} else {

		gear_rail->gear_rail_state = gear_rail->close_state_h;
	}


	//START SWITCH
	switch (state) {

	case init_state:

		SmartDashboard::PutString("State", "Initial");

		fly_wheel->flywheel_state = fly_wheel->stop_state_h;

		conveyor_->conveyor_state = conveyor_->stop_state_h;

		elevator_->elevator_state = elevator_->stop_state_h;

		state = wait_for_button_state;

		break;

	case wait_for_button_state: // can only do one thing at a time - sean wanted this

		SmartDashboard::PutString("State", "Wait For Button");

		fly_wheel->flywheel_state = fly_wheel->stop_state_h;

		elevator_->elevator_state = elevator_->stop_state_h;

		climber_->climber_state = climber_->stop_state_h;

		if (is_fire) {

			state = init_shooting_state;

		} else if (is_climb) {

			state = init_climbing;

		}

		break;

	case init_shooting_state:

		SmartDashboard::PutString("State", "Initial Shooting");

		fly_wheel->flywheel_state = fly_wheel->spin_state_h;

		std::cout << fly_wheel->FlywheelValue() << std::endl;

		if (!is_fire) {

			state = wait_for_button_state;

		}

		if (fly_wheel->IsAtSpeed() && second_fire_button) {

			state = fire_state;
		}

		break;

	case fire_state:

		SmartDashboard::PutString("State", "Fire");

		if (is_popcorn) {

			elevator_->elevator_state = elevator_->reverse_state_h;
			//conveyor_->conveyor_state = conveyor_->popcorn_state_h;

		} else {

			//conveyor_->conveyor_state = conveyor_->load_state_h;
			elevator_->elevator_state = elevator_->elevate_state_h;

		}

		if (!is_fire) {

			state = wait_for_button_state;

		}

		break;

	case init_climbing:

		SmartDashboard::PutString("State", "Initial Climbing");

		elevator_->elevator_state = elevator_->stop_state_h;

		fly_wheel->flywheel_state = fly_wheel->stop_state_h;

		state = climbing_state;

		break;

	case climbing_state: //hold to climb, release to stop

		SmartDashboard::PutString("State", "Climbing");

		climber_->climber_state = climber_->climbing_state_h;

		if (!is_climb) {

			state = wait_for_button_state;
		}

		if (climber_->CheckCurrent() >= climber_->MAX_CURRENT) {

			state = finish_climbing;

		}

		break;

	case finish_climbing:

		SmartDashboard::PutString("State", "Finish Climbing");

		climber_->climber_state = climber_->stop_state_h;

		if (is_ret) {

			state = wait_for_button_state;

		}

		break;

	}
	//END SWITCH

}

//sets the state back to the init state
void TeleopStateMachine::Initialize(){

	state = init_state;

}

