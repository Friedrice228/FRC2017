/*
 * TeleopStateMachine.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <TeleopStateMachine.h>
#include <iostream>

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

Timer *shootTimer = new Timer();

const double period = .5;
const double duty_cycle = 1.0;
const double sleep_cycle = 0.0;
const double reverse_cycle = 0.0;

bool elevate_go = false;

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

void TeleopStateMachine::StateMachine(bool is_gear, bool is_close_gear,
bool is_fire, bool is_climb,
bool is_ret, bool is_popcorn, bool is_second_fire, bool is_stop_shoot) {

	if (fly_wheel->IsAtSpeed()) {

		SmartDashboard::PutBoolean("At Speed", true);

	} else {

		SmartDashboard::PutBoolean("At Speed", false);

	}

	SmartDashboard::PutNumber("FlyWheel Speed", fly_wheel->GetSpeed());

	if (is_popcorn) {

		elevator_->elevator_state = elevator_->reverse_state_h;

	} else if (!is_popcorn && state != fire_state) {

		elevator_->elevator_state = elevator_->stop_state_h;

	}

	if (is_gear) {

		gear_rail->gear_rail_state = gear_rail->open_state_h;

		std::cout<<"HE"<<std::endl;

	} else if (is_close_gear) {

		gear_rail->gear_rail_state = gear_rail->close_state_h;
	}

	//START SWITCH
	switch (state) {

	case init_state:

		SmartDashboard::PutString("State", "Initial");

		gear_rail->gear_rail_state = gear_rail->close_state_h;

		fly_wheel->flywheel_state = fly_wheel->stop_state_h;

		conveyor_->conveyor_state = conveyor_->stop_state_h;

		elevator_->elevator_state = elevator_->stop_state_h;

		state = wait_for_button_state;

		break;

	case wait_for_button_state: // can only do one thing at a time - sean wanted this

		SmartDashboard::PutString("State", "Wait For Button");

		fly_wheel->flywheel_state = fly_wheel->stop_state_h;

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

		if (is_stop_shoot) {

			state = wait_for_button_state;

		}

		if (fly_wheel->IsAtSpeed() && is_second_fire) {

			shootTimer->Reset();

			shootTimer->Start();

			state = fire_state;
		}

		break;

	case fire_state:

		SmartDashboard::PutString("State", "Fire");

		if (is_popcorn) {

			elevator_->elevator_state = elevator_->reverse_state_h;

		} else if ((shootTimer->Get() < (duty_cycle * period))) { //will run 40% of the time

			elevator_->elevator_state = elevator_->elevate_state_h;

		}else if ((shootTimer->Get() < ((reverse_cycle * period)) + (duty_cycle * period))){

			elevator_->elevator_state = elevator_->reverse_state_h;

		}else if ((shootTimer->Get() < ((reverse_cycle * period)) + (duty_cycle * period) + (sleep_cycle * period))){

			elevator_->elevator_state = elevator_->stop_state_h;

		}else if ((shootTimer->Get() >= ((reverse_cycle * period)) + (duty_cycle * period) + (sleep_cycle * period)) && (shootTimer->Get() < (period))) { //stop during other time

			elevator_->elevator_state = elevator_->stop_state_h;

		}else {//if ((shootTimer->Get() >= (period))) { // automatically resets the timer

			shootTimer->Reset();

		}

		if (is_stop_shoot) {

			state = wait_for_button_state;

			elevator_->elevator_state = elevator_->stop_state_h;

			shootTimer->Stop();

		} else if (!is_second_fire) {

			elevator_->elevator_state = elevator_->stop_state_h;

			shootTimer->Stop();

			state = init_shooting_state;

		}

		break;

	case init_climbing:

		SmartDashboard::PutString("State", "Initial Climbing");

		elevator_->elevator_state = elevator_->stop_state_h;

		fly_wheel->flywheel_state = fly_wheel->stop_state_h;

		state = climbing_state;

		break;

	case climbing_state:
		//hold to climb, release to stop

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
void TeleopStateMachine::Initialize() {

	state = init_state;

}

