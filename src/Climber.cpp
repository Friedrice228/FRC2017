/*
 * Climber.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: DriversStation
 */

#include <Climber.h>

const int stop_state = 0;
const int climbing_state = 1;

Climber::Climber() { // not climbing back down - sean

	canTalonClimberRight = new CANTalon(46); //intake
	canTalonClimberLeft = new CANTalon(41); //flywheel

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

		std::cout << canTalonClimberRight->GetOutputCurrent() << std::endl;
		Climbing(.5);

		break;

	}
}




