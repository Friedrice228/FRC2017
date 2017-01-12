/*
 * DriveController.h
 *
 *  Created on: Jan 9, 2017
 *      Author: Apes of Wrath
 */
#include <WPILib.h>
#include <CANTalon.h>
#include <RobotDrive.h>
#include <Joystick.h>

#ifndef SRC_DRIVECONTROLLER_H_
#define SRC_DRIVECONTROLLER_H_

class DriveController {
public:

	RobotDrive *robotDrive;
	CANTalon *canTalonFrontLeft, *canTalonFrontRight, *canTalonBackRight,
			 *canTalonBackLeft, *canTalonKicker;

	DriveController();
	void SplitArcade(Joystick *JoystickThrottle, Joystick *JoyWheel);
	void HDrive(Joystick *JoyThrottle, Joystick *JoyWheel);
	void ClosedLoopStrafe();
	void ClosedLoopYaw();
	void StopAll();

};

#endif /* SRC_DRIVECONTROLLER_H_ */
