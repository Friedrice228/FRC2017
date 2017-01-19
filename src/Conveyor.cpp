#include <Conveyor.h>

const int popcorn_state = 0;
const int load_state = 1;
const int stop_state = 2;

Conveyor::Conveyor() {

	canTalonFront = new CANTalon(20);
	canTalonBack = new CANTalon(29);

}

void Conveyor::Popcorn(double speed) {

	canTalonFront->Set(-speed);
	canTalonBack->Set(-speed);

}

void Conveyor::Stop() {

	canTalonFront->Set(0);
	canTalonBack->Set(0);

}

void Conveyor::Load(double speed) {

	canTalonFront->Set(speed);
	canTalonBack->Set(speed);

}

void Conveyor::ConStateMachine() {

	switch (conveyor_state) {

	case popcorn_state:

		Popcorn(.5);

		break;

	case load_state:

		Load(.5);

		break;

	case stop_state:

		Stop();

		break;
	}

}

