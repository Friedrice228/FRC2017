#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <WPILib.h>
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <DriveController.h>
#include <Flywheel.h>
#include <Elevator.h>
#include <GearRail.h>
#include <Conveyor.h>
#include <Climber.h>
#include <TeleopStateMachine.h>
#include <Autonomous.h>
#include <LEDLightStrip.h>

class Robot: public frc::IterativeRobot {
public:

	DriveController *drive_controller;
	Joystick *joyOp, *joyThrottle, *joyWheel;
	Flywheel *fly_wheel;
	Elevator *elevator_;
	GearRail *gear_rail;
	Conveyor *conveyor_;
	Vision *vision_;
	TeleopStateMachine *teleop_state_machine;
	Autonomous *autonomous_;
	Climber *climber_;
	LEDLightStrip *light_;
	Compressor *compressor;
	PowerDistributionPanel *pdp;



// defining values for button actions
	const int JOY_OP = 1;
	const int JOY_THROTTLE = 0;
	const int JOY_WHEEL = 2;

	const int GEAR_LIGHT_BUTTON = 8;
	const int BALL_LIGHT_BUTTON = 9;
	const int GEAR_AND_BALL_LIGHT_BUTTON = 10;
	const int CLIMB_BUTTON = 12;
	const int GEAR_BUTTON = 7;
	const int GEAR_CLOSE_BUTTON = 8;
	const int FIRE_BUTTON = 4;
	const int POPCORN_BUTTON = 2;
	const int FIRE_BUTTON_2 = 1;
	const int STOP_SHOOT_BUTTON = 3;
	const int RETURN_BUTTON = 6; //NOT USED



	const int HEADING_CONTROL_BUTTON = 5;
	const int VISION_TRACK_BUTTON = 6;
	const int FC_BUTTON = 1;
	const int REG_BUTTON = 2;




	frc::SendableChooser<std::string> autonChooser;
	frc::SendableChooser<std::string> allianceChooser;

	const std::string gearPlacementAndShoot = "Gear Placement and Shoot"; //move forward place a gear and then shoot the ten pre loaded balls
	const std::string gearPlacementUsualAuton = "Gear Placement Usual"; //gear to middle position
	const std::string gearPlacementUsualVision = "Gear Placement Usual Vision";
	const std::string gearPlacementRight = "Gear Placement Right"; //gear to side position
	const std::string gearPlacementLeft = "Gear Placement Left";
	const std::string shootAuton = "Shoot"; //shoot with pre-loaded balls
	const std::string shootAndLoadAuton = "Shoot and Load"; //get balls from hopper, then shoot
	const std::string driveForward = "Drive Forward";
	const std::string fancyDriveForward = "Fancy Drive Forward";


	const std::string redAlliance = "Red Alliance";
	const std::string blueAlliance = "Blue Alliance";

	std::string autoSelected;
	std::string allianceSelected;

	const std::string version = "1.0";

	const int HDrive = 0;
	const int Split = 1;
	const int Vis = 2;
	int driveMode = HDrive; //0 = HDRIVE 1 = split
	const int FC = 0;
	const int Reg = 1;
	int Type = Reg;

	bool is_heading;bool is_vision;bool is_fc;

	double total = 0;

	void RobotInit() {

		pdp = new PowerDistributionPanel(1);

		joyOp = new Joystick(JOY_OP);
		joyThrottle = new Joystick(JOY_THROTTLE);
		joyWheel = new Joystick(JOY_WHEEL);

		fly_wheel = new Flywheel();

		elevator_ = new Elevator();

		gear_rail = new GearRail();

		conveyor_ = new Conveyor();

		vision_ = new Vision();

		drive_controller = new DriveController(vision_);

		autonomous_ = new Autonomous(drive_controller, elevator_, conveyor_,
				gear_rail, fly_wheel);

		climber_ = new Climber();

		light_ = new LEDLightStrip();

		teleop_state_machine = new TeleopStateMachine(fly_wheel, conveyor_,
				gear_rail, elevator_, drive_controller, vision_, climber_);

		autonChooser.AddDefault(gearPlacementUsualAuton, gearPlacementUsualAuton);
		autonChooser.AddObject(gearPlacementAndShoot, gearPlacementAndShoot);
		autonChooser.AddObject(gearPlacementRight, gearPlacementRight);
		autonChooser.AddObject(gearPlacementLeft, gearPlacementLeft);
		autonChooser.AddObject(shootAuton, shootAuton);
		autonChooser.AddObject(shootAndLoadAuton, shootAndLoadAuton);
		autonChooser.AddObject(gearPlacementUsualVision, gearPlacementUsualVision);
		autonChooser.AddObject(driveForward, driveForward);

		frc::SmartDashboard::PutData("Auto Modes", &autonChooser);

		allianceChooser.AddDefault(blueAlliance, blueAlliance);
		allianceChooser.AddObject(redAlliance, redAlliance);

		frc::SmartDashboard::PutData("Alliance", &allianceChooser);

		is_heading = false;
		is_heading = false;
		is_fc = true;

		compressor = new Compressor(31);
		compressor->SetClosedLoopControl(true);

	} // RobotInit

	void AutonomousInit() override {

		autoSelected = autonChooser.GetSelected();

		allianceSelected = allianceChooser.GetSelected();

		fly_wheel->StartThread(); //starts the speed controller

		drive_controller->ZeroEncs(); //reset everything
		drive_controller->ZeroI();
		drive_controller->ahrs->ZeroYaw();
		drive_controller->ResetIndex();
		drive_controller->ResetVisionState();

		if (autoSelected == gearPlacementUsualAuton) { //choose auton sequence

			if (allianceSelected == redAlliance){

				autonomous_->FillProfile("/home/lvuser/Gear_Profile_Red.csv");

			} else {

				autonomous_->FillProfile("/home/lvuser/Gear_Profile_Blue.csv");

			}

		} else if(autoSelected == gearPlacementUsualVision){

			if (allianceSelected == redAlliance){

			} else{

			}

		}else if (autoSelected == gearPlacementRight) {

			autonomous_->FillProfile("/home/lvuser/Gear_Right_Profile.csv");

		} else if (autoSelected == gearPlacementLeft) {

		} else if (autoSelected == driveForward) {

			autonomous_->FillProfile("/home/lvuser/Drive_Forward_Profile.csv");

		}else if (autoSelected == shootAuton) {


			if (allianceSelected == redAlliance) {

			} else {

			}

		} else if (autoSelected == shootAndLoadAuton) {

			if (allianceSelected == redAlliance) {

			} else {

			}

		} else if (autoSelected == gearPlacementAndShoot) {

			if (allianceSelected == redAlliance) {

			} else {

			}

		}

	}

	void AutonomousPeriodic() {

		conveyor_->ConStateMachine();
		elevator_->ElevatorStateMachine();
		fly_wheel->FlywheelStateMachine();
		gear_rail->GearRailStateMachine();

		autonomous_->RunAuton();

		std::cout << "angle: " << drive_controller->ahrs->GetYaw() << std::endl;

	}

	void TeleopInit() {

		fly_wheel->StartThread(); //starts the speed controller thread

		drive_controller->StartTeleopThreads(joyThrottle, joyWheel, &is_heading,
				&is_vision, &is_fc); //starts the drive code thread
		drive_controller->KickerDown();

		teleop_state_machine->Initialize(); //sets the state back to init

		drive_controller->DisableAutonThreads();

		drive_controller->ahrs->ZeroYaw();

	}

	void TeleopPeriodic() {

	//	std::cout<<pdp->GetTotalCurrent()<<std::endl;

//		std::cout<<"R1: "<< drive_controller->canTalonBackRight->GetOutputCurrent();//<<std::endl;
//		std::cout<<" R2: "<< drive_controller->canTalonFrontRight->GetOutputCurrent();//<<std::endl;
//		std::cout<<" L1: "<< drive_controller->canTalonBackLeft->GetOutputCurrent();//<<std::endl;
//		std::cout<< " L2: "<< drive_controller->canTalonFrontLeft->GetOutputCurrent()<<std::endl;

		SmartDashboard::PutNumber("Azimuth", vision_->findAzimuth());

		bool gear_button = joyOp->GetRawButton(GEAR_BUTTON);
		bool gear_close_button = joyOp->GetRawButton(GEAR_CLOSE_BUTTON);
		bool stop_shoot_button = joyOp->GetRawButton(STOP_SHOOT_BUTTON);
		bool fire_button = joyOp->GetRawButton(FIRE_BUTTON);
		bool climb_button = joyOp->GetRawButton(CLIMB_BUTTON);
		bool return_button = joyOp->GetRawButton(RETURN_BUTTON);
		bool gear_light_button = joyOp->GetRawButton(GEAR_LIGHT_BUTTON);
		bool ball_light_button = joyOp->GetRawButton(BALL_LIGHT_BUTTON);
		bool gear_and_ball_light_button = joyOp->GetRawButton(
				GEAR_AND_BALL_LIGHT_BUTTON);
		bool popcorn_button = joyOp->GetRawButton(POPCORN_BUTTON);
		bool second_fire_button = joyOp->GetRawButton(FIRE_BUTTON_2);

		teleop_state_machine->StateMachine(gear_button, gear_close_button,
				fire_button, climb_button, return_button, popcorn_button,
				second_fire_button, stop_shoot_button);

		//light_->LEDStateMachine(gear_light_button, ball_light_button,
		//		gear_and_ball_light_button);
		conveyor_->ConStateMachine();
		elevator_->ElevatorStateMachine();
		fly_wheel->FlywheelStateMachine();
		gear_rail->GearRailStateMachine();
		climber_->ClimberStateMachine();

		//START DRIVE CODE
		const int HDrive = 0;
		const int Heading = 1;
		const int Vis = 2;

		bool headingDrive = joyWheel->GetRawButton(HEADING_CONTROL_BUTTON);
		bool visionTrack = joyWheel->GetRawButton(VISION_TRACK_BUTTON);

		switch (driveMode) {

		case HDrive:

			is_heading = false;

			is_vision = false;

			if (headingDrive) {

				drive_controller->StopAll();

				drive_controller->KickerDown(); //Kicker to stay down the whole time

				drive_controller->SetInitHeading();

				driveMode = Heading;

			} else if (visionTrack) {

				drive_controller->StopAll();

				drive_controller->KickerDown(); //Kicker to stay down the whole time

				drive_controller->SetInitHeading();

				drive_controller->SetAngle();

				driveMode = Vis;

			}

			break;

		case Heading:

			is_heading = true;

			if (!headingDrive) {

				drive_controller->StopAll();

				drive_controller->KickerDown();

				driveMode = HDrive;

			}

			break;

		case Vis:

			is_vision = true;

			std::cout << "VIS" << std::endl;

			if (!visionTrack) {

				drive_controller->StopAll();

				drive_controller->KickerDown();

				driveMode = HDrive;

			}

			break;

		}
		//END DRIVECODE

		const int FC = 0; //Field Centric
		const int Reg = 1;

		bool fcButton = joyThrottle->GetRawButton(FC_BUTTON);
		bool regButton = joyThrottle->GetRawButton(REG_BUTTON);

		switch (Type) {

		case FC:

			SmartDashboard::PutString("Drive: ", "Field Centric");

			is_fc = true;

			if (regButton) {

				Type = Reg;

			}

			break;

		case Reg:

			SmartDashboard::PutString("Drive: ", "Regular");

			is_fc = false;

			if (fcButton) {

				Type = FC;

			}

			break;

		}

		if (joyThrottle->GetRawButton(3)) {

			drive_controller->ahrs->ZeroYaw();

		}

	} // TeleopPeriodic

	void DisabledInit() {

		drive_controller->DisableTeleopThreads();

		drive_controller->DisableAutonThreads();

		fly_wheel->DisableThreads();

	}

	void TestPeriodic() {

		std::cout << vision_->findAzimuth() << std::endl;

	}

private:

};

START_ROBOT_CLASS(Robot)
