#include <Conveyor.h>

//	Make popcorn, put kernels in, or we're out of kernels
const int popcorn_state = 0;
const int load_state = 1;
const int stop_state = 2;
//	CANTalon reference numbers
const int CAN_TALON_FRONT = 77;
const int CAN_TALON_BACK = 78;
//	Speed that we pop popcorn at and load the popper at
const double POPCORN_SPEED = .5;
const double LOAD_SPEED = .5;

//	Init
Conveyor::Conveyor() {

	//	Init CANTalons
	canTalonFront = new CANTalon(CAN_TALON_FRONT);
	canTalonBack = new CANTalon(CAN_TALON_BACK);

}

//	Make popcorn NOTE: no butter yet
void Conveyor::Popcorn(double speed) {

	//	Make popcorn
	canTalonFront->Set(-speed);
	canTalonBack->Set(-speed);

}

//	Stop the popcorn maker because we are out of corn
void Conveyor::Stop() {

	//	Set motors to zero
	canTalonFront->Set(0);
	canTalonBack->Set(0);

}

//	Load up more corn
void Conveyor::Load(double speed) {

	//	Get the CANTalons up to speed (heh)
	canTalonFront->Set(speed);
	canTalonBack->Set(speed);

}

//	STATE MACHINE WOOHOO
void Conveyor::ConStateMachine() {

	switch (conveyor_state) {

	//	If we want popcorn, we make popcorn
	case popcorn_state:

		//	LET THEM EAT CAKE
		Popcorn(POPCORN_SPEED);

		break;

	//	If we need more corn, we load up more corn
	case load_state:

		//	Would you like some butter? (NOTE still no butter)
		Load(LOAD_SPEED);

		break;

	//	STOP...we're out! :/
	case stop_state:

		//	Stop signs are just a recommendation
		Stop();

		break;
	}
}

