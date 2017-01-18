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

	canTalonClimberRight = new CANTalon(19);
	canTalonClimberLeft = new CANTalon(20);

}

void Climber::climbing(double speed) {

	canTalonClimberRight->Set(speed);
	canTalonClimberLeft->Set(-speed);
}

void Climber::stop() {

	canTalonClimberRight->Set(0);
	canTalonClimberLeft->Set(0);

}

void Climber::climber_state_machine(){

	switch(climber_state){

	case stop_state:

		stop();

		break;

	case climbing_state:

		climbing(.5);

		break;

	}
}




