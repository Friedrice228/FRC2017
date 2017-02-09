/*
 * Climber.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: DriversStation
 */

#include <Climber.h>

//	State machine constants: 0 means not climbing, 1 means climbing
const int stop_state = 0;
const int climbing_state = 1;
//	Reference numbers for the climbing CANTalons
const int CAN_TALON_CLIMBER_RIGHT = 32;
const int CAN_TALON_CLIMBER_LEFT = 99; //doesntexist
//	The fastest we can climb without scaring the monkeys
const double CLIMBING_SPEED = .5;

//	Init CANTalons
Climber::Climber() { // not climbing back down -sean We don't want it to go back down? -Colton

	canTalonClimberRight = new CANTalon(CAN_TALON_CLIMBER_RIGHT); //intake
	canTalonClimberLeft = new CANTalon(CAN_TALON_CLIMBER_LEFT); //flywheel

}

//	Monkey see, monkey do. Robot see monkey do, robot see.
//	AKA Robot climb
void Climber::Climbing(double speed) {

	//	Set CANTalons to make robot climb
	canTalonClimberRight->Set(speed);
	canTalonClimberLeft->Set(-speed);
}

//	Make robot stop climbing because monkeys are throwing bananas at us
//	You can say, they are full of wrath ;)
void Climber::Stop() {

	//	Set CANTalons to zero
	canTalonClimberRight->Set(0);
	canTalonClimberLeft->Set(0);

}

//	Return output current for the CANTalons
double Climber::CheckCurrent(){

	return (double)(canTalonClimberRight->GetOutputCurrent());

}

//	Secretary of State Machines is Max because he wrote this to make robot go and climb like a good robot that listens to its masters because it is a good robot.
void Climber::ClimberStateMachine(){

	switch(climber_state){

	//	If we want robot to stop, we go to stop() function
	case stop_state:

		Stop();

		break;

	//	If we want robot to climb, we go to climb() function
	case climbing_state:

	//	std::cout << canTalonClimberRight->GetOutputCurrent() << std::endl;
		Climbing(CLIMBING_SPEED);

		break;

	}
}




