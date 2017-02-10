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
	void HDrive(Joystick *JoyThrottle, Joystick *JoyWheel);
	void StopAll();
	void StartTeleopThreads(Joystick *JoyThrottle, Joystick *JoyWheel, bool *is_heading);
	void StartAutonThreads();
	void EndThreads();
	void Drive(double ref_kick, double ref_right, double ref_left, double ref_yaw, double k_p_right, double k_p_left, double k_p_kick, double k_p_yaw, double k_d_yaw, double target_vel);
	void DrivePID();
	void HeadingPID(Joystick *joyWheel);
	void KickerUp();
	void KickerDown();
	void ZeroEncs();
	void ZeroI();
	static void DrivePIDWrapper(DriveController *driveController);
	void SetRef(double refs[][5]);
	double GetRef();
	static void HDriveWrapper(Joystick *JoyThrottle, Joystick *JoyWheel, bool *is_heading, DriveController *driveController);
	void DisableThreads();
};

#endif /* SRC_DRIVECONTROLLER_H_ */
