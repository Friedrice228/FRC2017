/*
 * Climber.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: DriversStation
 */

#include <Climber.h>

const int stop_state = 0;
const int climbing_state = 1;
const int CAN_TALON_CLIMBER_RIGHT = 32;
const int CAN_TALON_CLIMBER_LEFT = 99; //doesntexist
const double CLIMBING_SPEED = .5;

Climber::Climber() { // not climbing back down - sean

	canTalonClimberRight = new CANTalon(CAN_TALON_CLIMBER_RIGHT); //intake
	canTalonClimberLeft = new CANTalon(CAN_TALON_CLIMBER_LEFT); //flywheel

}

void Climber::Climbing(double speed) {

	canTalonClimberRight->Set(speed);
	canTalonClimberLeft->Set(-speed);
}

void Climber::Stop() {

	canTalonClimberRight->Set(0);
	canTalonClimberLeft->Set(0);

}

double Climber::CheckCurrent(){

	return (double)(canTalonClimberRight->GetOutputCurrent());


}

void Climber::ClimberStateMachine(){

	switch(climber_state){

	case stop_state:

		Stop();

		break;

	case climbing_state:

	//	std::cout << canTalonClimberRight->GetOutputCurrent() << std::endl;
		Climbing(CLIMBING_SPEED);

		break;

	}
}




