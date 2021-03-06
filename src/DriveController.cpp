/*
 * DriveController.cpp
 *
 *  Created on: Jan 9, 2017
 *      Author: Apes of Wrath
 */

#include <DriveController.h>
#include <WPILib.h>

#define PI 3.1415926

using namespace std::chrono;

const double WHEEL_DIAMETER = 4.0; //inches
const double TICKS_PER_ROT = 4096;

const double MAX_Y_RPM = 480;
double DYN_MAX_Y_RPM = 480;
const double MAX_X_RPM = 230; // Max RPM ACTUAL: 330
const double MAX_YAW_RATE = (15.48 / 508) * MAX_Y_RPM; //max angular velocity divided by the max rpm multiplied by set max rpm

const int DC_SLEEP_TIME = 1;

const int CAN_TALON_FRONT_LEFT = 22;
const int CAN_TALON_BACK_LEFT = 18;
const int CAN_TALON_BACK_RIGHT = 36;
const int CAN_TALON_FRONT_RIGHT = 30;
const int CAN_TALON_KICKER = 20;

double l_last_error = 0;
double r_last_error = 0;
double yaw_last_error = 0;
double kick_last_error = 0;

double l_last_error_vel = 0;
double r_last_error_vel = 0;
double kick_last_error_vel = 0;

//CHANGEABLESTART

const double K_P_YAW_T = 6.0;

const double K_P_YAW_AU = 10.0;
const double K_D_YAW_AU = 0.0;

const double K_P_YAW_H_VEL = 13.0;

const double K_P_YAW_HEADING_POS = 9;

const double K_P_LEFT_VEL = 0.00550;//0.0077;
const double K_D_LEFT_VEL = 0.0;// -0.025;
const double K_F_LEFT_VEL = 1.0 / 508.0;
double P_LEFT_VEL = 0;
double D_LEFT_VEL = 0;
double d_left_vel = 0; //dynamic value

const double K_P_RIGHT_VEL = 0.00550;//0.0077;
const double K_D_RIGHT_VEL = 0.0; //-0.025;
const double K_F_RIGHT_VEL = 1.0 / 508.0;
double P_RIGHT_VEL = 0;
double D_RIGHT_VEL = 0;
double d_right_vel = 0; //dynamic value

const double K_P_KICK_VEL = .005;
const double K_D_KICK_VEL = 0.0;
const double K_F_KICK_VEL = 1.0 / 230.0; //1/330
double P_KICK_VEL = 0;
double D_KICK_VEL = 0;
double d_kick_vel = 0; // dynamic value

const double CONVERSION_DIVISION = 4096;
const double CONVERSION_MULTIPLICATION = 600;

const double K_P_RIGHT_DIS = 0.225;
const double K_P_LEFT_DIS = 0.225;
const double K_P_KICKER_DIS = 0.175; //TODO: check this value

const double K_I_RIGHT_DIS = 0.000;
const double K_I_LEFT_DIS = 0.000;
const double K_I_KICKER_DIS = 0.0;

const double K_D_RIGHT_DIS = 0.0;
const double K_D_LEFT_DIS = 0.0;
const double K_D_KICKER_DIS = 0.0;

//CHANGEABLE END

const double MAX_FPS = ((MAX_Y_RPM * 4.0 * PI) / 12.0) / 60.0; //conversion to fps
const double Kv = 1 / MAX_FPS; //scale from -1 to 1

const double MAX_KICK_FPS = ((MAX_X_RPM * 4.0 * PI) / 12.0) / 60.0;
const int Kv_KICK = 1 / MAX_KICK_FPS;

double P_RIGHT_DIS = 0;
double I_RIGHT_DIS = 0;
double D_RIGHT_DIS = 0;

double P_LEFT_DIS = 0;
double I_LEFT_DIS = 0;
double D_LEFT_DIS = 0;

double P_KICK_DIS = 0;
double I_KICK_DIS = 0;
double D_KICK_DIS = 0;

double d_right = 0;
double i_right = 0;

double d_yaw_dis = 0;

double d_left = 0;
double i_left = 0;

double d_kick = 0;
double i_kick = 0;

double l_error_vel_au = 0;
double l_error_dis_au = 0;

double r_error_vel_au = 0;
double r_error_dis_au = 0;

double k_error_dis_au = 0;

double l_error_vel_t = 0;
double l_error_dis_t = 0;

double r_error_vel_t = 0;
double r_error_dis_t = 0;

double kick_error_vel = 0;

double timetokeep = 0.01;

const int NUM_POINTS = 1500;
const int NUM_INDEX = 12;

double last_drive_ref[NUM_INDEX] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0 };
double drive_ref[NUM_INDEX] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0 };
double full_refs[NUM_POINTS][NUM_INDEX];

double acceptable_yaw_error = .22;

double error_heading = 0;
double total_heading = 0;

double init_heading = 0;
double visionAngle = 0;
double visionDistance = 0;

double store_right_enc = 0;
double store_left_enc = 0;

const int stamp_state = 0;
const int vision_aim_state = 1;
const int drive_state = 2;

bool vision_done_before = false;

int vision_track_state = stamp_state;

Timer *timerTeleop = new Timer();
Timer *timerAuton = new Timer();

std::thread HDriveThread, DrivePIDThread;

int index1 = 0;

int *ptr_index = &index1; //setting pointer to index

Vision *vision_dc = new Vision();

DriveController::DriveController(Vision *vis) {

	vision_dc = vis;

	canTalonFrontLeft = new CANTalon(CAN_TALON_FRONT_LEFT);

	canTalonBackLeft = new CANTalon(CAN_TALON_BACK_LEFT);
	canTalonBackLeft->SetControlMode(CANSpeedController::kFollower);
	canTalonBackLeft->Set(CAN_TALON_FRONT_LEFT);

	canTalonFrontRight = new CANTalon(CAN_TALON_FRONT_RIGHT);

	canTalonBackRight = new CANTalon(CAN_TALON_BACK_RIGHT);
	canTalonBackRight->SetControlMode(CANSpeedController::kFollower);
	canTalonBackRight->Set(CAN_TALON_FRONT_RIGHT);

	canTalonKicker = new CANTalon(CAN_TALON_KICKER);

	ahrs = new AHRS(SPI::Port::kMXP);

	kickerPiston = new DoubleSolenoid(4, 5, 6);

}

void DriveController::HDrive(Joystick *JoyThrottle, Joystick *JoyWheel,
bool *is_fc) { //finds targets for teleop

	double forward = -1.0 * JoyThrottle->GetY();
	double strafe = JoyThrottle->GetX();
	double current_yaw = fmod((-1.0 * ahrs->GetYaw() * (PI / 180.0)),
			(2.0 * PI));

	if ((bool) *is_fc) {

		if (current_yaw < -PI) {
			current_yaw += (2.0 * PI);
		} else if (current_yaw > PI) {
			current_yaw -= (2.0 * PI);
		}

		double psi = std::atan2(forward, strafe);

		double magnitude = sqrt((forward * forward) + (strafe * strafe));

		forward = magnitude * (std::sin(psi - current_yaw));
		strafe = magnitude * (std::cos(psi - current_yaw));

		if ((psi - current_yaw) > (-1.0 * PI) && (psi - current_yaw) <= (0)) {
			if (forward > 0) {
				forward = -1.0 * forward;
			}
		}
		if (((psi - current_yaw) > (PI / 2) && (psi - current_yaw) <= (PI))
				|| ((psi - current_yaw) > (-1.0 * PI)
						&& (psi - current_yaw) <= (-1.0 * PI / 2))) {
			if (strafe > 0) {
				strafe = -1.0 * strafe;
			}
		}
	} else {

		forward = 1.0 * forward; //not needed

	}

	double target_l, target_r, target_kick, target_yaw_rate;

	double axis_ratio = 0.0; //ratio between x and y axes

	if (strafe != 0) { //if x is 0, it is undefined (division)
		axis_ratio = std::abs(forward / strafe); //dont use regular abs, that returns an int
		DYN_MAX_Y_RPM = MAX_X_RPM * (double) axis_ratio;
		DYN_MAX_Y_RPM = DYN_MAX_Y_RPM > MAX_Y_RPM ? MAX_Y_RPM : DYN_MAX_Y_RPM; //if DYN_max_Y is bigger than MAX_Y then set to MAX_Y, otherwise keep DYN_MAX_Y
	} else {
		DYN_MAX_Y_RPM = MAX_Y_RPM; //deals with special case tht x = 0 (ratio cant divide by zero)
	}

	target_l = 1.0 * (forward < 0 ? -1 : 1) * (forward * forward)
			* DYN_MAX_Y_RPM; //if joy value is less than 0 set to -1, otherwise to 1

	target_r = target_l;

	target_kick = 1.0 * (strafe < 0 ? -1 : 1) * (strafe * strafe) * MAX_X_RPM; //if joy value is less than 0 set to -1, otherwise to 1

	double joy_wheel_val = JoyWheel->GetX();

	if (std::abs(joy_wheel_val) < .07){
		joy_wheel_val = 0.0;
	}

	target_yaw_rate = -1.0 * (joy_wheel_val) * MAX_YAW_RATE; //Left will be positive

	if (abs(target_kick) < 35) {
		target_kick = 0;
	}

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

	Drive(target_kick, target_r, target_l, target_yaw_rate, K_P_RIGHT_VEL,
			K_P_LEFT_VEL, K_P_KICK_VEL, K_P_YAW_T, 0.0, K_D_LEFT_VEL, K_D_RIGHT_VEL, K_D_KICK_VEL, 0.0, 0.0, 0.0);

}

/**
 * Param: Feet forward, + = forward
 */
void DriveController::DrivePID() { //finds targets for Auton

	double refLeft = drive_ref[0];
	double refRight = drive_ref[1];
	double refKick = drive_ref[2];
	double targetYawRate = drive_ref[3];
	double tarVelLeft = drive_ref[4];
	double tarVelRight = drive_ref[5];
	double tarVelKick = drive_ref[6];

	if (std::abs(tarVelKick) < .05) {
		tarVelKick = 0.0;
	}

	//conversion to feet
	double r_dis = -(((double) canTalonFrontRight->GetEncPosition()
			/ TICKS_PER_ROT) * (WHEEL_DIAMETER * PI) / 12);
	double l_dis = (((double) canTalonFrontLeft->GetEncPosition()
			/ TICKS_PER_ROT) * (WHEEL_DIAMETER * PI) / 12);

	double k_dis = (((double) canTalonKicker->GetEncPosition() / TICKS_PER_ROT)
			* (WHEEL_DIAMETER * PI) / 12);

	l_error_dis_au = refLeft - l_dis;
	r_error_dis_au = refRight - r_dis;
	k_error_dis_au = refKick - k_dis;

	i_right += (r_error_dis_au);
	d_right = (r_error_dis_au - r_last_error);

	i_left += (l_error_dis_au);
	d_left = (l_error_dis_au - l_last_error);

	i_kick += (k_error_dis_au);
	d_kick = (k_error_dis_au - kick_last_error);

	P_RIGHT_DIS = K_P_RIGHT_DIS * r_error_dis_au;
	I_RIGHT_DIS = K_I_RIGHT_DIS * i_right;
	D_RIGHT_DIS = K_D_RIGHT_DIS * d_right;

	P_LEFT_DIS = K_P_LEFT_DIS * l_error_dis_au;
	I_LEFT_DIS = K_I_LEFT_DIS * i_left;
	D_LEFT_DIS = K_D_LEFT_DIS * d_left;

	P_KICK_DIS = K_P_KICKER_DIS * k_error_dis_au;
	I_KICK_DIS = K_I_KICKER_DIS * i_kick;
	D_KICK_DIS = K_D_KICKER_DIS * d_kick;

	double total_right = P_RIGHT_DIS + I_RIGHT_DIS + D_RIGHT_DIS;
	double total_left = P_LEFT_DIS + I_LEFT_DIS + D_LEFT_DIS;
	double total_kick = P_KICK_DIS + I_KICK_DIS + D_KICK_DIS;

	double target_rpm_right = total_right * MAX_Y_RPM;
	double target_rpm_left = total_left * MAX_Y_RPM;
	double target_rpm_kick = total_kick * MAX_X_RPM;

	if (target_rpm_left > MAX_Y_RPM) {
		target_rpm_left = MAX_Y_RPM;
	} else if (target_rpm_left < -MAX_Y_RPM) {
		target_rpm_left = -MAX_Y_RPM;
	}

	if (target_rpm_right > MAX_Y_RPM) {
		target_rpm_right = MAX_Y_RPM;
	} else if (target_rpm_right < -MAX_Y_RPM) {
		target_rpm_right = -MAX_Y_RPM;
	}

	Drive(target_rpm_kick, target_rpm_right, target_rpm_left, targetYawRate,
			K_P_RIGHT_VEL, K_P_LEFT_VEL, K_P_KICK_VEL, K_P_YAW_AU, K_D_YAW_AU,
			K_D_LEFT_VEL, K_D_RIGHT_VEL, K_D_KICK_VEL, tarVelLeft, tarVelRight, tarVelKick);

	l_last_error = l_error_dis_au;
	r_last_error = r_error_dis_au;
	kick_last_error = k_error_dis_au;

}

/*
 * Param: radian value, + is right
 */
void DriveController::HeadingPID(Joystick *joyWheel) { //angling

	double target_heading = init_heading
			+ (-1.0 * joyWheel->GetX() * (90.0 * PI / 180.0)); //scaling, conversion to radians,left should be positive

	double current_heading = -1.0 * ahrs->GetYaw() * ( PI / 180.0); //degrees to radians, left should be positive

	double error_heading_h = target_heading - current_heading;

	double total_heading_h = K_P_YAW_HEADING_POS * error_heading_h;

	if (total_heading > MAX_YAW_RATE) {
		total_heading = MAX_YAW_RATE;
	} else if (total_heading < -MAX_YAW_RATE) {
		total_heading = -MAX_YAW_RATE;
	}

	Drive(0.0, 0.0, 0.0, total_heading_h, K_P_RIGHT_VEL, K_P_LEFT_VEL,
			K_P_KICK_VEL, K_P_YAW_H_VEL, 0.0, K_D_RIGHT_VEL, K_D_LEFT_VEL, K_D_KICK_VEL, 0.0, 0.0, 0.0);

}

void DriveController::VisionP() { //auto-aiming

	double angle = visionAngle;

	double normalized_angle = 0;

	if (angle > 180) {
		normalized_angle = 360 - angle;
	} else {
		normalized_angle = -angle; //negative = right
	}

	normalized_angle = normalized_angle * (double)(PI / 180.0);

	double current_heading = -1.0 * ahrs->GetYaw() * (double)(PI / 180.0);

	error_heading = (init_heading + normalized_angle) - current_heading;

	total_heading = K_P_YAW_HEADING_POS * error_heading;

	if (total_heading > MAX_YAW_RATE) {
		total_heading = MAX_YAW_RATE;
	} else if (total_heading < -MAX_YAW_RATE) {
		total_heading = -MAX_YAW_RATE;
	}

	//call to velocit controller
	Drive(0.0, 0.0, 0.0, total_heading, K_P_RIGHT_VEL, K_P_LEFT_VEL,
			K_P_KICK_VEL, K_P_YAW_H_VEL, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0);

}

void DriveController::AutoVisionTrack(){ //aims the robot and then drive forward in auton, do not use n teleop

	switch(vision_track_state){

	case stamp_state:
		SetInitHeading();
		SetAngle();
		vision_track_state = vision_aim_state;
		break;

	case vision_aim_state:
		VisionP();
		if (!vision_done_before && (total_heading < 0.4)){ //TODO: MAGYK NUBER
			vision_track_state = stamp_state;
			vision_done_before = true;
		}
		if (vision_done_before && (std::abs(error_heading) < 2.5)){ //TODO: MAGYK NUBER
			ZeroEncs();
			SetDist();
			vision_track_state = drive_state;
		}
		break;

	case drive_state:
		drive_ref[0] = visionDistance;
		drive_ref[1] = visionDistance;
		drive_ref[2] = 0;
		drive_ref[3] = 0;
		DrivePID();
		break;

	}

}

void DriveController::Drive(double ref_kick, double ref_right, double ref_left,
		double ref_yaw, double k_p_right, double k_p_left, double k_p_kick,
		double k_p_yaw, double k_d_yaw, double k_d_right, double k_d_left, double k_d_kick, double target_vel_left,
		double target_vel_right, double target_vel_kick) { //setting talons

	double yaw_rate_current = -1.0 * (double) ahrs->GetRawGyroZ()
			* (double) ((PI) / 180); //Left should be positive

	double target_yaw_rate = ref_yaw;

	ref_left = ref_left - (target_yaw_rate * (MAX_Y_RPM / MAX_YAW_RATE)); //left should be positive
	ref_right = ref_right + (target_yaw_rate * (MAX_Y_RPM / MAX_YAW_RATE));

	double yaw_error = target_yaw_rate - yaw_rate_current;

	if (std::abs(yaw_error) < .25) {
		yaw_error = 0;
	}

	d_yaw_dis = yaw_error - yaw_last_error;

	double yaw_output = ((k_p_yaw * yaw_error) + (k_d_yaw * d_yaw_dis));

	ref_right += yaw_output; //left should be positive
	ref_left -= yaw_output;

	if (abs(ref_kick) < 25) {
		ref_kick = 0;
	}

	if (ref_left > MAX_Y_RPM) {
		ref_left = MAX_Y_RPM;
	} else if (ref_left < -MAX_Y_RPM) {
		ref_left = -MAX_Y_RPM;
	}

	if (ref_right > MAX_Y_RPM) {
		ref_right = MAX_Y_RPM;
	} else if (ref_right < -MAX_Y_RPM) {
		ref_right = -MAX_Y_RPM;
	}

	double feed_forward_r = K_F_RIGHT_VEL * ref_right;
	double feed_forward_l = K_F_LEFT_VEL * ref_left;
	double feed_forward_k = K_F_KICK_VEL * ref_kick;

	//conversion to RPM from native unit
	double l_current = ((double) canTalonFrontLeft->GetEncVel()
			/ (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION;
	double r_current = -((double) canTalonFrontRight->GetEncVel()
			/ (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION;
	double kick_current = ((double) canTalonKicker->GetEncVel()
			/ (double) CONVERSION_DIVISION) * CONVERSION_MULTIPLICATION; //going right is positive

	l_error_vel_t = ref_left - l_current;
	r_error_vel_t = ref_right - r_current;
	kick_error_vel = ref_kick - kick_current;

	d_left_vel = (l_error_vel_t - l_last_error_vel);
	d_right_vel = (r_error_vel_t - r_last_error_vel);
	d_kick_vel = (kick_error_vel - kick_last_error_vel);

	P_LEFT_VEL = k_p_left * l_error_vel_t;
	P_RIGHT_VEL = k_p_right * r_error_vel_t;
	P_KICK_VEL = k_p_kick * kick_error_vel;

	D_LEFT_VEL = k_d_left * d_left_vel;
	D_RIGHT_VEL = k_d_right * d_right_vel;
	D_KICK_VEL = k_d_kick * d_kick_vel;

	double total_right = D_RIGHT_VEL + P_RIGHT_VEL + feed_forward_r + (Kv * target_vel_right);
	double total_left = D_LEFT_VEL + P_LEFT_VEL + feed_forward_l + (Kv * target_vel_left);
	double total_kick = D_KICK_VEL + P_KICK_VEL + feed_forward_k
			+ (Kv_KICK * target_vel_kick);

	canTalonFrontLeft->Set(-total_left); //back cantalons follow front, don't need to set them individually
	canTalonFrontRight->Set(total_right);
	canTalonKicker->Set(-total_kick);

//	std::cout << "P: " << P_LEFT_VEL;
//	std::cout << " D: " << D_LEFT_VEL;
//	std::cout << " Ref: " << ref_kick;
//	std::cout << " Left: " << kick_current;
//	std::cout << " Error: " << kick_error_vel << std::endl;

//	std::cout << "YAW RATE: " << yaw_rate_current;
//	std::cout << " ERROR: " << yaw_error << std::endl;

	yaw_last_error = yaw_error;
	l_last_error_vel = l_error_vel_t;
	r_last_error_vel = r_error_vel_t;
	kick_last_error_vel = kick_error_vel;

}

void DriveController::ResetVisionState(){

	vision_track_state = stamp_state;

	vision_done_before = false;

}

void DriveController::StopAll() {

	canTalonFrontLeft->Set(0);

	canTalonFrontRight->Set(0);

	canTalonKicker->Set(0);
}

void DriveController::KickerUp() {

	kickerPiston->Set(DoubleSolenoid::Value::kReverse);

}

void DriveController::KickerDown() {

	kickerPiston->Set(DoubleSolenoid::Value::kForward);
}

void DriveController::ZeroEncs() {

	canTalonFrontRight->SetEncPosition(0);
	canTalonFrontLeft->SetEncPosition(0);
	canTalonBackRight->SetEncPosition(0);
	canTalonBackLeft->SetEncPosition(0);
	canTalonKicker->SetEncPosition(0);

}

void DriveController::ZeroI() {

	i_right = 0;
	i_left = 0;

}

void DriveController::SetInitHeading() {

	init_heading = -1.0 * ahrs->GetYaw() * (double)(PI / 180);

}

void DriveController::SetAngle() {

	visionAngle = vision_dc->findAzimuth();

}

void DriveController::SetDist(){

	visionDistance = vision_dc->findDistance() / 12.0; //convert to feet

}

bool DriveController::CheckIfNull() { //true if all values are 0

	for (int i = 0; i < NUM_INDEX; i++) { //checks if the entire array is zero (excluding the first point, make sure you dont check that one)
		if (drive_ref[i] != 0) {
			return false;
		}
	}
	return true;

}

void DriveController::StoreEncValues() {

	store_right_enc = -canTalonFrontRight->GetEncPosition();
	store_left_enc = canTalonFrontLeft->GetEncPosition();

}

void DriveController::SetEncValues() {

	canTalonFrontRight->SetEncPosition(store_right_enc);
	canTalonBackRight->SetEncPosition(store_right_enc);

	canTalonFrontLeft->SetEncPosition(store_left_enc);
	canTalonBackLeft->SetEncPosition(store_left_enc);


}

void DriveController::HDriveWrapper(Joystick *JoyThrottle, Joystick *JoyWheel,
bool *is_heading, bool *is_vision, bool *is_fc,
		DriveController *driveController) {

	timerTeleop->Start();

	while (true) {
		while (frc::RobotState::IsEnabled() && !frc::RobotState::IsAutonomous()
				&& !(bool) *is_heading && !(bool) *is_vision) {

			std::this_thread::sleep_for(
					std::chrono::milliseconds(DC_SLEEP_TIME));

			if (timerTeleop->HasPeriodPassed(timetokeep)) {

				driveController->HDrive(JoyThrottle, JoyWheel, is_fc);

			}
		}
		while (frc::RobotState::IsEnabled() && !frc::RobotState::IsAutonomous()
				&& (bool) *is_heading && !(bool) *is_vision) {

			std::this_thread::sleep_for(
					std::chrono::milliseconds(DC_SLEEP_TIME));

			if (timerTeleop->HasPeriodPassed(timetokeep)) {

				driveController->HeadingPID(JoyWheel);

			}
		}
		while (frc::RobotState::IsEnabled() && !frc::RobotState::IsAutonomous()
				&& (bool) *is_vision && !(bool) *is_heading) {

			std::this_thread::sleep_for(
					std::chrono::milliseconds(DC_SLEEP_TIME));

			if (timerTeleop->HasPeriodPassed(timetokeep)) {

				driveController->VisionP();

			}

		}
	}
}

void DriveController::DrivePIDWrapper(DriveController *driveController) {

	timerAuton->Start();

	while (frc::RobotState::IsAutonomous() && frc::RobotState::IsEnabled()) {

		std::this_thread::sleep_for(std::chrono::milliseconds(DC_SLEEP_TIME));

		if (timerAuton->HasPeriodPassed(timetokeep)) {

			for (int i = 0; i < sizeof(drive_ref); i++) { //looks through each row and then fills drive_ref with the column here, refills each interval with next set of refs

				drive_ref[i] = full_refs[index1][i]; //from SetRefs()

			}

		//	if (drive_ref[11] == 1) { //vision on

		//		driveController->AutoVisionTrack();

			//} else { //vision off

				driveController->DrivePID();

			//}

			index1++;
		}

		if (index1 >= NUM_POINTS) { //stop at the end of the motion profile, this number is set after the creation of the array
									//so not all of the array will be accessed, only the part before the non-zero points
			driveController->StopAll();
			break;
		}
	}

}

void DriveController::StartTeleopThreads(Joystick *JoyThrottle,
		Joystick *JoyWheel,
		bool *is_heading, bool *is_vision, bool *is_fc) {

	DriveController *dc = this;

	HDriveThread = std::thread(&DriveController::HDriveWrapper, JoyThrottle,
			JoyWheel, is_heading, is_vision, is_fc, dc);
	HDriveThread.detach();

}

void DriveController::StartAutonThreads() {

	DriveController *dc = this;

	DrivePIDThread = std::thread(&DriveController::DrivePIDWrapper, dc);
	DrivePIDThread.detach();

}

void DriveController::DisableTeleopThreads() {

	HDriveThread.~thread();

}

void DriveController::DisableAutonThreads() {

	DrivePIDThread.~thread();

}

void DriveController::SetRef(double ref[][12]) { //each point has 11 indexes

	for (int r = 0; r < (sizeof(full_refs) / sizeof(full_refs[0])); r++) {

		for (int c = 0; c < (sizeof(full_refs[0]) / sizeof(full_refs[0][0]));
				c++) {

			full_refs[r][c] = ref[r][c];

		}

	}

}

int DriveController::GetIndex() {

	return *ptr_index;

}
void DriveController::ResetIndex() {

	index1 = 0;

}

