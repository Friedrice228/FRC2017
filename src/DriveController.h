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
#include <AHRS.h>
#include <thread>
#include <chrono>

#ifndef SRC_DRIVECONTROLLER_H_
#define SRC_DRIVECONTROLLER_H_

class DriveController {
public:

	RobotDrive *robotDrive;
	CANTalon *canTalonFrontLeft, *canTalonFrontRight, *canTalonBackRight,
			 *canTalonBackLeft, *canTalonKicker;
	AHRS *ahrs;
	DoubleSolenoid *kickerPiston;

	DriveController();
	void HDrive(Joystick *JoyThrottle, Joystick *JoyWheel, bool is_kick);
	void StopAll();
	void StartThreads(Joystick *JoyThrottle, Joystick *JoyWheel, bool *is_kick);
	void EndThreads();
	void KickerUp();
	void KickerDown();
	static void HDriveWrapper(Joystick *JoyThrottle, Joystick *JoyWheel, bool *is_kick, DriveController *driveController);
};

#endif /* SRC_DRIVECONTROLLER_H_ */
