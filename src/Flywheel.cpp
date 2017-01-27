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

const int MAX_FLYWHEEL_ERROR = 100;
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

	canTalonFlywheelFrontRight = new CANTalon(CAN_TALON_FLYWHEEL_RIGHT);

	canTalonFlywheelFrontRight->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	canTalonFlywheelFrontRight->SetF(RIGHT_F_GAIN);
	canTalonFlywheelFrontRight->SetP(RIGHT_P_GAIN);
	canTalonFlywheelFrontRight->ConfigNominalOutputVoltage(+2.0f, -0.0f);
	canTalonFlywheelFrontRight->ConfigPeakOutputVoltage(+12.0f, +2.0f);
	canTalonFlywheelFrontRight->SetSensorDirection(true);
	canTalonFlywheelFrontRight->SelectProfileSlot(0);

	canTalonFlywheelFrontLeft = new CANTalon(CAN_TALON_FLYWHEEL_LEFT);

	canTalonFlywheelFrontLeft->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	canTalonFlywheelFrontLeft->SetF(LEFT_F_GAIN);
	canTalonFlywheelFrontLeft->SetP(LEFT_P_GAIN);
	canTalonFlywheelFrontLeft->ConfigNominalOutputVoltage(+2.0f, -0.0f);
	canTalonFlywheelFrontLeft->ConfigPeakOutputVoltage(+12.0f, +2.0f);
	canTalonFlywheelFrontLeft->SetSensorDirection(true);
	canTalonFlywheelFrontLeft->SelectProfileSlot(0);

	canTalonFlywheelBackLeft = new CANTalon(CAN_TALON_FLYWHEEL_LEFT);

	canTalonFlywheelBackLeft->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	canTalonFlywheelBackLeft->SetF(LEFT_F_GAIN);
	canTalonFlywheelBackLeft->SetP(LEFT_P_GAIN);
	canTalonFlywheelBackLeft->ConfigNominalOutputVoltage(+2.0f, -0.0f);
	canTalonFlywheelBackLeft->ConfigPeakOutputVoltage(+12.0f, +2.0f);
	canTalonFlywheelBackLeft->SetSensorDirection(true);
	canTalonFlywheelBackLeft->SelectProfileSlot(0);

	canTalonFlywheelBackRight = new CANTalon(CAN_TALON_FLYWHEEL_LEFT);

	canTalonFlywheelBackRight->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	canTalonFlywheelBackRight->SetF(LEFT_F_GAIN);
	canTalonFlywheelBackRight->SetP(LEFT_P_GAIN);
	canTalonFlywheelBackRight->ConfigNominalOutputVoltage(+2.0f, -0.0f);
	canTalonFlywheelBackRight->ConfigPeakOutputVoltage(+12.0f, +2.0f);
	canTalonFlywheelBackRight->SetSensorDirection(true);
	canTalonFlywheelBackRight->SelectProfileSlot(0);

	active_ = false;
}

void Flywheel::Spin(int ref) {

	canTalonFlywheelFrontRight->SetControlMode(CANSpeedController::kSpeed);
	canTalonFlywheelFrontRight->Set(ref);

	canTalonFlywheelFrontLeft->SetControlMode(CANSpeedController::kSpeed);
	canTalonFlywheelFrontLeft->Set(ref);

	canTalonFlywheelBackRight->SetControlMode(CANSpeedController::kSpeed);
	canTalonFlywheelBackRight->Set(ref);

	canTalonFlywheelBackLeft->SetControlMode(CANSpeedController::kSpeed);
	canTalonFlywheelBackLeft->Set(ref);

}

void Flywheel::Stop() {

	canTalonFlywheelFrontRight->Set(0);
	canTalonFlywheelFrontLeft->Set(0);
	canTalonFlywheelBackRight->Set(0);
	canTalonFlywheelBackLeft->Set(0);

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

	while (true) {

		while (*active) {

			//	std::cout << "on" << std::endl;

			fw->Spin(ref);

			std::this_thread::sleep_for(
					std::chrono::milliseconds(FLYWHEEL_WAIT_TIME));

		}

		//	std::cout << "off" << std::endl;

		std::this_thread::sleep_for(
				std::chrono::milliseconds(FLYWHEEL_WAIT_TIME));

	}

}

void Flywheel::StartThread() {

	Flywheel *fw = new Flywheel();

	std::thread SpinThread(&Flywheel::SpinWrapper, fw, GOAL_RPM, &active_);
	SpinThread.detach();

}
