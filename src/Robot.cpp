#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "WPILib.h"
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <DriveController.h>
#include <Flywheel.h>
#include <Elevator.h>
#include <GearRail.h>
#include <Conveyor.h>
#include <TeleopStateMachine.h>
#include <Autonomous.h>

class Robot: public frc::IterativeRobot {
public:

	DriveController *drive_Controller;
	Joystick *joyOp, *joyThrottle, *joyWheel;
	Flywheel *fly_wheel;
	Elevator *elevator_;
	GearRail *gear_Rail;
	Conveyor *conveyor_;
	Vision *vision_;
	TeleopStateMachine *teleop_State_Machine;
	Autonomous *autonomous_;

	frc::SendableChooser<std::string> autonChooser;
	frc::SendableChooser<std::string> allianceChooser;

	const std::string gearPlacementUsualAuton = "Gear Placement Usual";
	const std::string gearPlacementAlternateAuton = "Gear Placement Alternate";
	const std::string shootAuton = "Shoot";
	const std::string shootAndLoadAuton = "Shoot and Load";

	const std::string redAlliance = "Red Alliance";
	const std::string blueAlliance = "Blue Alliance";

	std::string autoSelected;
	std::string allianceSelected;

	const int HDrive = 0;
	const int Split = 1;
	int driveMode = HDrive; //0 = HDRIVE 1 = split

	bool is_kick;

	void RobotInit() {

		joyOp = new Joystick(0);
		joyThrottle = new Joystick(1);
		joyWheel = new Joystick(2);

		drive_Controller = new DriveController();

		fly_wheel = new Flywheel();

		elevator_ = new Elevator();

		gear_Rail = new GearRail();

		conveyor_ = new Conveyor();

		vision_ = new Vision();

		autonomous_ = new Autonomous();

		teleop_State_Machine = new TeleopStateMachine(fly_wheel, conveyor_, gear_Rail,
				elevator_, drive_Controller, vision_);

		autonChooser.AddDefault(gearPlacementUsualAuton,
				gearPlacementUsualAuton);
		autonChooser.AddObject(gearPlacementAlternateAuton,
				gearPlacementAlternateAuton);
		autonChooser.AddObject(shootAuton, shootAuton);
		autonChooser.AddObject(shootAndLoadAuton, shootAndLoadAuton);

		frc::SmartDashboard::PutData("Auto Modes", &autonChooser);

		allianceChooser.AddDefault(blueAlliance, blueAlliance);
		allianceChooser.AddObject(redAlliance, redAlliance);

		frc::SmartDashboard::PutData("Alliance", &allianceChooser);

		is_kick = true;

	}

	void AutonomousInit() override {

		autoSelected = autonChooser.GetSelected();

		allianceSelected = allianceChooser.GetSelected();

	}

	void AutonomousPeriodic() {

		if (autoSelected == gearPlacementUsualAuton) {
			std::cout << "Auto 1" << std::endl;
		} else if (autoSelected == gearPlacementAlternateAuton) {
			std::cout << "Auto 2" << std::endl;
		} else if (autoSelected == shootAuton) {
			std::cout << "Auto 3" << std::endl;
		} else if (autoSelected == shootAndLoadAuton) {
			std::cout << "Auto 4" << std::endl;

		}

		if (allianceSelected == redAlliance) {

			std::cout << "Red" << std::endl;

		} else {

			std::cout << "Blue" << std::endl;

		}

	}

	void TeleopInit() {

		drive_Controller->StartThreads(joyThrottle, joyWheel, &is_kick);
		drive_Controller->KickerDown();

	}

	void TeleopPeriodic() {

		//START DRIVE CODE
		const int HDrive = 0;
		const int Split = 1;

		bool hDrive = joyThrottle->GetRawButton(1);
		bool arcadeDrive = joyThrottle->GetRawButton(2);

		switch (driveMode) {

		case HDrive: //HDRIVE

			is_kick = true;

			if (arcadeDrive) {

				drive_Controller->StopAll();

				drive_Controller->KickerUp();

				driveMode = Split; //Split = 1;
			}

			break;

		case Split: //SPLIT

			is_kick = false;

			if (hDrive) {

				drive_Controller->StopAll();

				drive_Controller->KickerDown();

				driveMode = HDrive;

			}

			break;

		}

		//END DRIVECODE


	}

	void TestPeriodic() {
		CANTalon *canTalon = new CANTalon(21);

		canTalon->Set(1);

		std::cout << canTalon->GetEncPosition() << std::endl;

	}

private:

};

START_ROBOT_CLASS(Robot)
