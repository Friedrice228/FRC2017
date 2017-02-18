/*
 * Flywheel.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <Flywheel.h>

const int stop_state = 0;
const int spin_state = 1;

const int GOAL_RPM = 3200;

const int MAX_FLYWHEEL_ERROR = 400;
const int CAN_TALON_FLYWHEEL_FRONT_RIGHT = 33;
const int CAN_TALON_FLYWHEEL_BACK_RIGHT = 24;
const int CAN_TALON_FLYWHEEL_FRONT_LEFT = 29;
const int CAN_TALON_FLYWHEEL_BACK_LEFT = 19;
const int FLYWHEEL_WAIT_TIME = 10;

const double RIGHT_F_GAIN = .036; //0.025;
const double RIGHT_P_GAIN = 0.075;//.01;
const double LEFT_F_GAIN = 0;
const double LEFT_P_GAIN = 0;
const double CONVERSION_DIVISION = 4096;
const double CONVERSION_MULTIPLICATION = 600;

bool active_;

double flywheel_time = 0.01;

Timer *timerFly = new Timer();

std::thread SpinThread;

Flywheel::Flywheel() {

	canTalonFlywheelFrontRight = new CANTalon(CAN_TALON_FLYWHEEL_FRONT_RIGHT);
	canTalonFlywheelFrontRight->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	canTalonFlywheelFrontRight->SetF(RIGHT_F_GAIN);
	canTalonFlywheelFrontRight->SetP(RIGHT_P_GAIN);
	canTalonFlywheelFrontRight->ConfigNominalOutputVoltage(+2.0f, -0.0f);
	canTalonFlywheelFrontRight->ConfigPeakOutputVoltage(+12.0f, +2.0f);
	canTalonFlywheelFrontRight->SetSensorDirection(true);
	canTalonFlywheelFrontRight->SelectProfileSlot(0);
	canTalonFlywheelFrontRight->SetSensorDirection(true);

	//Set all other motors as slaves that will follow the output of the master
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

void Flywheel::Spin(int ref) {

	canTalonFlywheelFrontRight->SetControlMode(CANSpeedController::kSpeed);
	canTalonFlywheelFrontRight->Set(ref);


}

void Flywheel::Stop() {

	canTalonFlywheelFrontRight->SetControlMode(CANSpeedController::kVoltage);
	canTalonFlywheelFrontRight->Set(0);

}
// current speed target speed variable
bool Flywheel::IsAtSpeed() {

	double flywheel_value = -((double) canTalonFlywheelFrontRight->GetEncVel()
			/ (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION;

	if (abs(flywheel_value - GOAL_RPM) < MAX_FLYWHEEL_ERROR) {

		return true;

	} else {

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

	SpinThread = std::thread(&Flywheel::SpinWrapper, fw, GOAL_RPM, &active_);
	SpinThread.detach();

}
void Flywheel::DisableThreads(){

	SpinThread.~thread();

}
