#include <iostream>
#include <memory>
#include <string>

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <DriveController.h>

class Robot: public frc::IterativeRobot {
public:

	DriveController *driveController;
	Joystick *joyOp, *joyThrottle, *joyWheel;

	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	frc::SendableChooser<std::string> chooser;
	std::string autoSelected;

	const int HDrive = 0;
	const int Split = 1;
	const int wait = 2;
	int driveMode = HDrive; //0 = HDRIVE 1 = split
	int lastState = 0;

	void RobotInit() {

		joyOp = new Joystick(0);
		joyThrottle = new Joystick(1);
		joyWheel = new Joystick(2);

		driveController = new DriveController();

		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);

	}

	void AutonomousInit() override {
		autoSelected = chooser.GetSelected();
		// std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

	}

	void AutonomousPeriodic() {

		if (autoSelected == autoNameCustom) {

		} else {

		}
	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {

		//START DRIVE CODE
		bool hDrive = joyThrottle->GetRawButton(1);
		bool arcadeDrive = joyThrottle->GetRawButton(2);
		bool strafe = joyThrottle->GetRawButton(3);
		bool yaw = joyThrottle->GetRawButton(4);

		switch (driveMode) {

		case 0: //HDRIVE

			driveController->HDrive(joyThrottle, joyWheel);

			if (arcadeDrive) {
				driveMode = Split;
			}

			break;

		case 1: //SPLIT

			driveController->SplitArcade(joyThrottle, joyWheel);

			if (hDrive) {
				driveMode = hDrive;
			}

			break;

		case 2: //WAIT

			if (!strafe && !yaw) {

				driveMode = lastState;

			}

			break;

		}

		if (strafe) {

			lastState = driveMode;
			driveMode = wait;

			driveController->ClosedLoopStrafe();

		} else if (yaw) {

			lastState = driveMode;
			driveMode = wait;

			driveController->ClosedLoopYaw();
		}

		//END DRIVECODE

	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
