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
#include <cmath>
#include <Timer.h>

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
	void DrivePID(double ref);
	void HeadingPID(double ref);
	void KickerUp();
	void KickerDown();
	void ZeroEncs();
	static void HDriveWrapper(Joystick *JoyThrottle, Joystick *JoyWheel, bool *is_kick, DriveController *driveController);
};

#endif /* SRC_DRIVECONTROLLER_H_ */
