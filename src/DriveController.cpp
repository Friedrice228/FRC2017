/*
 * DriveController.cpp
 *
 *  Created on: Jan 9, 2017
 *      Author: Apes of Wrath
 */

#include <DriveController.h>

#define PI 3.1415926

using namespace std::chrono;

const double MAX_Y_RPM = 450;
const double MAX_X_RPM = 100; //Also a guess
const double MAX_YAW_RATE = (17.8/502) * MAX_Y_RPM;

const int DC_SLEEP_TIME = 10;

const int CAN_TALON_FRONT_LEFT = 27;
const int CAN_TALON_BACK_LEFT = 14;
const int CAN_TALON_BACK_RIGHT = 13;
const int CAN_TALON_FRONT_RIGHT = 11;//17.8
const int CAN_TALON_KICKER = 12;

double l_error, r_error, kick_error;
double l_last_error = 0;
double r_last_error = 0;
double kick_last_error = 0;

const double K_P_YAW = -50; // need to invert the yaw direction

const double K_P_LEFT = 0.0005; // 0.0035 has oscillation on ground big
const double K_F_LEFT = 1.0/502.0; // 0.1 too low
double P_LEFT = 0;

const double K_P_RIGHT = 0.0005; //0.004 too big wheels off the ground (250 RPM) / 0.001 too small / 0.003 too small still (300 RPM) / 0.0035 small (300) / 0.003 big
const double K_F_RIGHT = 1.0/502.0;
double P_RIGHT = 0;


const double K_P_KICK = .0;
const double K_F_KICK = .0;

const double CONVERSION_DIVISION = 4096;
const double CONVERSION_MULTIPLICATION = 600;
double P_KICK = 0;

DriveController::DriveController() {

	canTalonFrontLeft = new CANTalon(CAN_TALON_FRONT_LEFT);
	canTalonBackLeft = new CANTalon(CAN_TALON_BACK_LEFT);
	canTalonBackRight = new CANTalon(CAN_TALON_BACK_RIGHT);
	canTalonFrontRight = new CANTalon(CAN_TALON_FRONT_RIGHT);
	canTalonKicker = new CANTalon(CAN_TALON_KICKER);

	ahrs = new AHRS(SPI::Port::kMXP);

	kickerPiston = new DoubleSolenoid(4, 5, 6);

}

void DriveController::HDrive(Joystick *JoyThrottle, Joystick *JoyWheel,
bool is_kick) {

	double target_l, target_r, target_kick, target_yaw_rate;
	double yaw_rate_current = (double) ahrs->GetRawGyroZ() * (double) ((PI) / 180); //Right is positive angular velocity

	target_l = -1.0 * JoyThrottle->GetY() * MAX_Y_RPM;
	target_r = target_l; //-

	target_kick = JoyThrottle->GetX() * MAX_X_RPM * (bool) is_kick;

	target_yaw_rate = JoyWheel->GetX() * MAX_YAW_RATE;

	target_l = target_l + (target_yaw_rate * (MAX_Y_RPM / MAX_YAW_RATE)); //-
	target_r = target_r - (target_yaw_rate * (MAX_Y_RPM / MAX_YAW_RATE));

	double yaw_error = yaw_rate_current - target_yaw_rate;

	double yaw_output = K_P_YAW * yaw_error;

	target_l += yaw_output;
	target_r -= yaw_output;

	if (target_l > MAX_Y_RPM) {

		target_l = MAX_Y_RPM;

	} else if (target_l < -MAX_Y_RPM) {

		target_l = -MAX_Y_RPM;
	}
	if (target_r > MAX_Y_RPM) {

		target_r = MAX_Y_RPM;

	} else if (target_r < -MAX_Y_RPM) {

		target_r = -MAX_Y_RPM;

	}

	double feed_forward_r = K_F_RIGHT * target_r;
	double feed_forward_l = K_F_LEFT * target_l;

	 //conversion to RPM from native unit
	double l_current = ((double) canTalonFrontLeft->GetEncVel() //-
			/ (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION;
	double r_current = -((double) canTalonFrontRight->GetEncVel()
			/ (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION;
	double kick_current = -((double) canTalonKicker->GetEncVel()
			/ (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION; //going right is positive

	l_error = target_l - l_current;
	r_error = target_r - r_current;
	kick_error = target_kick - kick_current;

	P_LEFT = K_P_LEFT * l_error;
	P_RIGHT = K_P_RIGHT * r_error;
	P_KICK = K_P_KICK * kick_error;

	double total_right = P_RIGHT + feed_forward_r;
	double total_left = P_LEFT + feed_forward_l;
	double total_kick = P_KICK + K_F_KICK;

	canTalonFrontLeft->Set(-total_left);
	canTalonBackLeft->Set(-total_left);
	canTalonFrontRight->Set(total_right);
	canTalonBackRight->Set(total_right);
	canTalonKicker->Set(total_kick);

//	std::cout << "OUTPUT: " << canTalonFrontLeft->Get();
//	std::cout << " CURRRNT: " << l_current;
//	std::cout << " ERRK: " << l_error;
	std::cout << " TARGET R : " << target_r << std::endl;
	std::cout << " TARGET L : " << target_l << std::endl;

	std::cout << "Vel: " <<  yaw_rate_current;// << std::endl;

//	std::cout << "Left " << ((double) canTalonFrontLeft->GetEncVel() / (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION;// << std::endl;
//	std::cout << "Right " << ((double) canTalonFrontRight->GetEncVel() / (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION << std::endl;


}

void DriveController::StopAll() {

	canTalonFrontLeft->Set(0);
	canTalonBackLeft->Set(0);

	canTalonFrontRight->Set(0);
	canTalonBackRight->Set(0);

	canTalonKicker->Set(0);
}

void DriveController::KickerUp() {

	kickerPiston->Set(DoubleSolenoid::Value::kReverse);

}

void DriveController::KickerDown() {

	kickerPiston->Set(DoubleSolenoid::Value::kForward);
}

void DriveController::HDriveWrapper(Joystick *JoyThrottle, Joystick *JoyWheel,
bool *is_kick, DriveController *driveController) {

	while (true) {

		std::this_thread::sleep_for(std::chrono::milliseconds(DC_SLEEP_TIME));

		driveController->HDrive(JoyThrottle, JoyWheel, *is_kick);

	}
}

void DriveController::StartThreads(Joystick *JoyThrottle, Joystick *JoyWheel,
bool *is_kick) {

	DriveController *dc = this;

	std::thread HDriveThread(&DriveController::HDriveWrapper, JoyThrottle,
			JoyWheel, is_kick, dc);
	HDriveThread.detach();

}
