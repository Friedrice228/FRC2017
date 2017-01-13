/*
 * DriveController.cpp
 *
 *  Created on: Jan 9, 2017
 *      Author: Apes of Wrath
 */

#include <DriveController.h>

DriveController::DriveController() {

	canTalonFrontLeft = new CANTalon(21);
	canTalonBackLeft = new CANTalon(22);
	canTalonBackRight = new CANTalon(24);
	canTalonFrontRight = new CANTalon(23);
	canTalonKicker = new CANTalon(27);

	robotDrive = new RobotDrive(canTalonFrontLeft, canTalonBackLeft,
			canTalonFrontRight, canTalonBackRight);
	robotDrive->SetInvertedMotor(RobotDrive::MotorType::kFrontLeftMotor, true);
	robotDrive->SetInvertedMotor(RobotDrive::MotorType::kRearLeftMotor, true);
	robotDrive->SetInvertedMotor(RobotDrive::MotorType::kFrontRightMotor, true);
	robotDrive->SetInvertedMotor(RobotDrive::MotorType::kRearRightMotor, true);
}

void DriveController::SplitArcade(Joystick *JoyThrottle, Joystick *JoyWheel) {

	robotDrive->ArcadeDrive(JoyThrottle->GetY() * .8, (((JoyWheel->GetX()))) * .8, false);

}

void DriveController::HDrive(Joystick *JoyThrottle, Joystick *JoyWheel) {

	double leftVel;
	double rightVel;
	double hVel;

	leftVel = JoyThrottle->GetY();
	rightVel = -JoyThrottle->GetY();

	hVel = JoyThrottle->GetX();

	leftVel = leftVel - ((JoyWheel->GetX())) - .1; //if the wheel values are reversed, switch out +/- on operations
	rightVel = rightVel - ((JoyWheel->GetX()))- .1;

	if ((rightVel) > 1) {
		rightVel = 1;
	} else if ((rightVel) < -1) {
		rightVel = -1;
	}

	if ((leftVel) > 1) {
		leftVel = 1;
	} else if ((leftVel) < -1) {
		leftVel = -1;
	}

	std::cout << "L: " << leftVel << std::endl;
	std::cout << "R: " << rightVel << std::endl;

	leftVel = leftVel * .8;
	rightVel = rightVel * .8;

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

void DriveController::StopAll() {

	canTalonFrontLeft->Set(0);
	canTalonBackLeft->Set(0);

	canTalonFrontRight->Set(0);
	canTalonBackRight->Set(0);

	canTalonKicker->Set(0);
}


