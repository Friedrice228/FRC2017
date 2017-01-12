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

	frc::SendableChooser<std::string> autonChooser;
	frc::SendableChooser<std::string> allianceChooser;

	const std::string gearPlacementUsualAuton = "Gear Placement Usual";
	const std::string gearPlacementAlternateAuton = "Gear Placement Alternate";
	const std::string shootAuton = "Shoot";
	const std::string shootAndLoadAuton = "Shoot and Load";

	const std::string redAlliance = "Red Alliance";
	const std::string blueAlliance = "Blue Alliance";

	std::string autoSelected;

	const int HDrive = 0;
	const int Split = 1;
	const int wait = 2;
	int driveMode = HDrive; //0 = HDRIVE 1 = split

	int lastState = 0;
	int strafeState = 0;
	int yawState = 0;

	void RobotInit() {

		joyOp = new Joystick(0);
		joyThrottle = new Joystick(1);
		joyWheel = new Joystick(2);

		driveController = new DriveController();

		autonChooser.AddDefault(gearPlacementUsualAuton, gearPlacementUsualAuton);
		autonChooser.AddObject(gearPlacementAlternateAuton, gearPlacementAlternateAuton);
		autonChooser.AddObject(shootAuton, shootAuton);
		autonChooser.AddObject(shootAndLoadAuton, shootAndLoadAuton);

		frc::SmartDashboard::PutData("Auto Modes", &autonChooser);

		allianceChooser.AddDefault(blueAlliance, blueAlliance);
		allianceChooser.AddObject(redAlliance,redAlliance);

		frc::SmartDashboard::PutData("Alliance", &allianceChooser);

	}

	void AutonomousInit() override {

		autoSelected = autonChooser.GetSelected();

	}

	void AutonomousPeriodic() {

		if (autoSelected == gearPlacementAlternateAuton) {
			//System.out.println("Auto 1");
			std::cout << "Auto 1" << std::endl;
		}
		else {
			std::cout << "Auto 2" << std::endl;
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

			std::cout << "HERE" << std::endl;

			if (arcadeDrive) {

				driveController->StopAll();

				driveMode = Split; //Split = 1;
			}

			break;

		case 1: //SPLIT

			std::cout << "DONE" << std::endl;

			driveController->SplitArcade(joyThrottle, joyWheel);

			if (hDrive) {

				driveController->StopAll();

				driveMode = HDrive;

			}

			break;

		case 2: //WAIT

			if (!strafe && !yaw) {

				driveMode = lastState;

				strafeState = 0; // reset to the init state

				yawState = 0;

				driveController->StopAll();
			}

			break;

		}

		//CLOSED LOOP DRIVER SWITCHOUT
		if (strafe) {

			switch (strafeState) {

			case 0: //INIT STATE

				lastState = driveMode;
				driveMode = wait;

				strafeState = 1;

				break;

			case 1: //DRIVE STATE

				driveController->ClosedLoopStrafe();

				break;

			}

		} else if (yaw) {

			switch (yawState) {

			case 0: //INIT STATE

				lastState = driveMode;
				driveMode = wait;
				yawState = 1;

				break;

			case 1: //DRIVE STATE

				driveController->ClosedLoopYaw();

				break;

			}

		}

		//END DRIVECODE

	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
