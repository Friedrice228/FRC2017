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

Flywheel::Flywheel() {

	canTalonFlywheelRight = new CANTalon(34);
		canTalonFlywheelRight->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
		canTalonFlywheelRight->SetF(0.025);
		canTalonFlywheelRight->SetP(.01);
		canTalonFlywheelRight->ConfigNominalOutputVoltage(+2.0f, -0.0f);
		canTalonFlywheelRight->ConfigPeakOutputVoltage(+12.0f, +2.0f);
		canTalonFlywheelRight->SetSensorDirection(true);
		canTalonFlywheelRight->SelectProfileSlot(0);

	canTalonFlywheelLeft = new CANTalon(33);
		canTalonFlywheelLeft->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
		canTalonFlywheelLeft->SetF(0);
		canTalonFlywheelLeft->SetP(0);
		canTalonFlywheelLeft->ConfigNominalOutputVoltage(+2.0f, -0.0f);
		canTalonFlywheelLeft->ConfigPeakOutputVoltage(+12.0f, +2.0f);
		canTalonFlywheelLeft->SetSensorDirection(true);
		canTalonFlywheelLeft->SelectProfileSlot(0);
}

void Flywheel::Spin(int ref){

	canTalonFlywheelRight->SetControlMode(CANSpeedController::kSpeed);
	canTalonFlywheelRight->Set(ref);

	canTalonFlywheelLeft->SetControlMode(CANSpeedController::kSpeed);
	canTalonFlywheelLeft->Set(ref);

}

void Flywheel::Stop(){

	canTalonFlywheelRight->Set(0);
	canTalonFlywheelLeft->Set(0);

}
// current speed target speed variable
bool Flywheel::IsAtSpeed(){

	double flywheel_value = -((double)canTalonFlywheelRight->GetEncVel()/(double)4096) * 600;

	if(abs(flywheel_value - GOAL_RPM) < MAX_FLYWHEEL_ERROR) {

		return true;

	} else {

		return false;
	}


}

void Flywheel::FlywheelStateMachine(){

	switch(flywheel_state) {

	case stop_state:

		Stop();

		break;

	case spin_state:

		Spin(GOAL_RPM);

		break;


	}
}

double Flywheel::GetSpeed() {

	return -((double)canTalonFlywheelRight->GetEncVel()/(double)4096) * 600;
}

void Flywheel::StartThread(){

}
