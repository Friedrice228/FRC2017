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
#define PI 3.1415926

const double MAX_Y_RPM = 550;
const double MAX_X_RPM = 2500;
const double MAX_YAW_RATE = 20 * ((PI) / 180);
const double K_P_YAW = 0;
const int DC_SLEEP_TIME = 10;
const int GEAR_BUTTON = 5;
const int FIRE_BUTTON = 6;
const int CLIMB_BUTTON = 7;
const int RETURN_BUTTON = 8;
const int CAN_TALON_FRONT_LEFT = 22;
const int CAN_TALON_BACK_LEFT = 23;
const int CAN_TALON_BACK_RIGHT = 24;
const int CAN_TALON_FRONT_RIGHT = 21;
const int CAN_TALON_KICKER = 57;
const int JOY_OP = 0;
const int JOY_THROTTLE = 1;
const int JOY_WHEEL = 2;
const int CAN_LIGHT =31;

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
