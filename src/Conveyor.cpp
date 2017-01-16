#include <Conveyor.h>

const int popcorn_state = 0;
const int load_state = 1;
const int stop_state = 2;

Conveyor::Conveyor() {

	canTalonFront = new CANTalon(15);
	canTalonBack = new CANTalon(16);

}

void Conveyor::popcorn(double speed) {

	canTalonFront->Set(-speed);
	canTalonBack->Set(-speed);

}

void Conveyor::stop() {

	canTalonFront->Set(0);
	canTalonBack->Set(0);

}

void Conveyor::load(double speed) {

	canTalonFront->Set(speed);
	canTalonBack->Set(speed);

}

void Conveyor::con_state_machine() {

	switch (conveyor_state) {

	case popcorn_state:

		popcorn(.5);

		break;

	case load_state:

		load(.5);

		break;

	case stop_state:

		void stop();

		break;
	}

}

