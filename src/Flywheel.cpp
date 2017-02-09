/*
 * Flywheel.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <Flywheel.h>

//	State machine options
const int stop_state = 0;
const int spin_state = 1;

//	Speed we want
const int GOAL_RPM = 2500;

//	Maximum error we want with the flywheel
const int MAX_FLYWHEEL_ERROR = 300;
//	CANTalon reference numbers
const int CAN_TALON_FLYWHEEL_FRONT_RIGHT = 33;
const int CAN_TALON_FLYWHEEL_BACK_RIGHT = 24;
const int CAN_TALON_FLYWHEEL_FRONT_LEFT = 29;
const int CAN_TALON_FLYWHEEL_BACK_LEFT = 19;
//	How long we have to wait for the Flywheel to be cool
const int FLYWHEEL_WAIT_TIME = 10;

//	PID stuff
const double RIGHT_F_GAIN = 0.025;
const double RIGHT_P_GAIN = .01;
const double LEFT_F_GAIN = 0;
const double LEFT_P_GAIN = 0;
const double CONVERSION_DIVISION = 4096;
const double CONVERSION_MULTIPLICATION = 600;

//	Active?
bool active_;

//	This
double flywheel_time = 0.01;

//	Timer to keep track of time
Timer *timerFly = new Timer();

//	init
Flywheel::Flywheel() {

	//	CANTALON init for front right motor. Then, set up stuff
	canTalonFlywheelFrontRight = new CANTalon(CAN_TALON_FLYWHEEL_FRONT_RIGHT);
	canTalonFlywheelFrontRight->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	canTalonFlywheelFrontRight->SetF(RIGHT_F_GAIN);
	canTalonFlywheelFrontRight->SetP(RIGHT_P_GAIN);
	canTalonFlywheelFrontRight->ConfigNominalOutputVoltage(+2.0f, -0.0f);
	canTalonFlywheelFrontRight->ConfigPeakOutputVoltage(+12.0f, +2.0f);
	canTalonFlywheelFrontRight->SetSensorDirection(true);
	canTalonFlywheelFrontRight->SelectProfileSlot(0);

	//	Set all other motors as slaves that will follow the output of the master
	canTalonFlywheelFrontLeft = new CANTalon(CAN_TALON_FLYWHEEL_FRONT_LEFT);
	canTalonFlywheelFrontLeft->SetControlMode(CANSpeedController::kFollower);
	canTalonFlywheelFrontLeft->Set(CAN_TALON_FLYWHEEL_FRONT_RIGHT);

	canTalonFlywheelBackLeft = new CANTalon(CAN_TALON_FLYWHEEL_BACK_LEFT);
	canTalonFlywheelBackLeft->SetControlMode(CANSpeedController::kFollower);
	canTalonFlywheelBackLeft->Set(CAN_TALON_FLYWHEEL_FRONT_RIGHT);

	canTalonFlywheelBackRight = new CANTalon(CAN_TALON_FLYWHEEL_BACK_RIGHT);
	canTalonFlywheelBackRight->SetControlMode(CANSpeedController::kFollower);
	canTalonFlywheelBackRight->Set(CAN_TALON_FLYWHEEL_FRONT_RIGHT);

	active_ = false;
}

//	Make flywheel spin
void Flywheel::Spin(int ref) {

	//	Set it to go (Go Johnny go...)
	canTalonFlywheelFrontRight->SetControlMode(CANSpeedController::kSpeed);
	canTalonFlywheelFrontRight->Set(ref);


}

//	Stop the flywheel
void Flywheel::Stop() {

	//	STOP lights are just a suggestion
	canTalonFlywheelFrontRight->Set(0);

}

// Check if current speed is at target speed variable
bool Flywheel::IsAtSpeed() {

	//	Get current speed
	double flywheel_value = -((double) canTalonFlywheelFrontRight->GetEncVel()
			/ (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION;

	//	Make sure we're within the max error of our goal RPM as defined above
	if (abs(flywheel_value - GOAL_RPM) < MAX_FLYWHEEL_ERROR) {

		//	If we're satisfied, the flywheel is at speed
		return true;

	} else {

		//	If we aren't satisfied, eat a Snickers and try again
		return false;
	}

}

double Flywheel::FlywheelValue() {

	double flywheel_value = -((double) canTalonFlywheelFrontRight->GetEncVel()
			/ (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION;

	return flywheel_value;

}

void Flywheel::FlywheelStateMachine() {

	switch (flywheel_state) {

	case stop_state:

		active_ = false;

		Stop();

		break;

	case spin_state:

		active_ = true;

		break;

	}
}

double Flywheel::GetSpeed() {

	return -((double) canTalonFlywheelFrontRight->GetEncVel()
			/ (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION;
}

void Flywheel::SpinWrapper(Flywheel *fw, int ref, bool *active) {

	timerFly->Start();

	while (true) {

		while (*active) {

			std::this_thread::sleep_for(std::chrono::milliseconds(FLYWHEEL_WAIT_TIME));

			if (timerFly->HasPeriodPassed(flywheel_time)){

				fw->Spin(ref);

			}

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(FLYWHEEL_WAIT_TIME));

	}

}

void Flywheel::StartThread() {

	Flywheel *fw = new Flywheel();

	std::thread SpinThread(&Flywheel::SpinWrapper, fw, GOAL_RPM, &active_);
	SpinThread.detach();

}
