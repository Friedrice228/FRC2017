/*
 * Flywheel.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: DriversStation
 */
#include <Flywheel.h>

const int stop_state = 0;
const int spin_state = 1;
const int GOAL_RPM = 2500;
const int MAX_FLYWHEEL_ERROR = 0;
const int CAN_TALON_FLYWHEEL_RIGHT = 27;
const int CAN_TALON_FLYWHEEL_LEFT = 33;
const int FLYWHEEL_WAIT_TIME = 10;

const double RIGHT_F_GAIN = 0.025;
const double RIGHT_P_GAIN = .01;
const double LEFT_F_GAIN = 0;
const double LEFT_P_GAIN = 0;
const double CONVERSION_DIVISION = 4096;
const double CONVERSION_MULTIPLICATION = 600;

bool active_;

Flywheel::Flywheel() {

	canTalonFlywheelRight = new CANTalon(CAN_TALON_FLYWHEEL_RIGHT);
	canTalonFlywheelRight->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	canTalonFlywheelRight->SetF(RIGHT_F_GAIN);
	canTalonFlywheelRight->SetP(RIGHT_P_GAIN);
	canTalonFlywheelRight->ConfigNominalOutputVoltage(+2.0f, -0.0f);
	canTalonFlywheelRight->ConfigPeakOutputVoltage(+12.0f, +2.0f);
	canTalonFlywheelRight->SetSensorDirection(true);
	canTalonFlywheelRight->SelectProfileSlot(0);

	canTalonFlywheelLeft = new CANTalon(CAN_TALON_FLYWHEEL_LEFT);
	canTalonFlywheelLeft->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	canTalonFlywheelLeft->SetF(LEFT_F_GAIN);
	canTalonFlywheelLeft->SetP(LEFT_P_GAIN);
	canTalonFlywheelLeft->ConfigNominalOutputVoltage(+2.0f, -0.0f);
	canTalonFlywheelLeft->ConfigPeakOutputVoltage(+12.0f, +2.0f);
	canTalonFlywheelLeft->SetSensorDirection(true);
	canTalonFlywheelLeft->SelectProfileSlot(0);

	active_ = false;
}

void Flywheel::Spin(int ref) {

	canTalonFlywheelRight->SetControlMode(CANSpeedController::kSpeed);
	canTalonFlywheelRight->Set(ref);

	canTalonFlywheelLeft->SetControlMode(CANSpeedController::kSpeed);
	canTalonFlywheelLeft->Set(ref);

}

void Flywheel::Stop() {

	canTalonFlywheelRight->Set(0);
	canTalonFlywheelLeft->Set(0);

}
// current speed target speed variable
bool Flywheel::IsAtSpeed() {

	double flywheel_value = -((double) canTalonFlywheelRight->GetEncVel()
			/ (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION;

	if (abs(flywheel_value - GOAL_RPM) < MAX_FLYWHEEL_ERROR) {

		return true;

	} else {

		return false;
	}

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

	return -((double) canTalonFlywheelRight->GetEncVel() / (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION;
}

void Flywheel::SpinWrapper(Flywheel *fw, int ref, bool *active) {

	while (true) {

		//std::cout << "off" << *active << std::endl;

		while (*active) {

			std::cout << "on" << std::endl;

			fw->Spin(ref);

			std::this_thread::sleep_for(std::chrono::milliseconds(FLYWHEEL_WAIT_TIME));

		}

		std::cout << "off" << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(FLYWHEEL_WAIT_TIME));

	}

}

void Flywheel::StartThread() {

	Flywheel *fw = new Flywheel();

	std::thread SpinThread(&Flywheel::SpinWrapper, fw, GOAL_RPM, &active_);
	SpinThread.detach();

}
