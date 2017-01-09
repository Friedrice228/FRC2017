/*
 * DriveController.cpp
 *
 *  Created on: Jan 9, 2017
 *      Author: Apes of Wrath
 */

#include <DriveController.h>

DriveController::DriveController() {

	canTalonFrontLeft = new CANTalon(0);
	canTalonBackLeft = new CANTalon(1);
	canTalonBackRight = new CANTalon(2);
	canTalonFrontRight = new CANTalon(3);
	canTalonKicker = new CANTalon(4);

	robotDrive = new RobotDrive(canTalonFrontLeft, canTalonBackLeft,
			canTalonFrontRight, canTalonBackRight);
	robotDrive->SetInvertedMotor(RobotDrive::MotorType::kFrontLeftMotor, true);
	robotDrive->SetInvertedMotor(RobotDrive::MotorType::kRearLeftMotor, true);
	robotDrive->SetInvertedMotor(RobotDrive::MotorType::kFrontRightMotor, true);
	robotDrive->SetInvertedMotor(RobotDrive::MotorType::kRearRightMotor, true);
}

void DriveController::SplitArcade(Joystick *JoyThrottle, Joystick *JoyWheel) {

	robotDrive->ArcadeDrive(JoyThrottle->GetY(), JoyWheel->GetX(), false);

}

void DriveController::HDrive(Joystick *JoyThrottle, Joystick *JoyWheel) {

	double leftVel;
	double rightVel;
	double hVel;

	leftVel = JoyThrottle->GetY();
	rightVel = -JoyThrottle->GetY();

	hVel = JoyThrottle->GetY();

	leftVel = leftVel + JoyWheel->GetY(); //if the wheel values are reversed, switch out +/- on operations
	rightVel = rightVel - JoyWheel->GetY();

	if (abs(rightVel) > 1) {
		rightVel = 1;
	} else if (abs(rightVel) < -1) {
		rightVel = -1;
	}

	if (abs(leftVel) > 1) {
		leftVel = 1;
	} else if (abs(leftVel) < -1) {
		leftVel = -1;
	}

	canTalonFrontLeft->Set(leftVel);
	canTalonBackLeft->Set(leftVel);

	canTalonFrontRight->Set(rightVel);
	canTalonBackRight->Set(rightVel);

	canTalonKicker->Set(hVel);

}

void DriveController::ClosedLoopStrafe() {

}

void DriveController::ClosedLoopYaw() {

}

