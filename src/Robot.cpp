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
#include <Climber.h>
#include <TeleopStateMachine.h>
#include <Autonomous.h>

class Robot: public frc::IterativeRobot {
public:

	DriveController *drive_Controller;
	Joystick *joyOp, *joyThrottle, *joyWheel;
	Flywheel *fly_wheel;
	Elevator *elevator_;
	GearRail *gear_rail;
	Conveyor *conveyor_;
	Vision *vision_;
	TeleopStateMachine *teleop_state_machine;
	Autonomous *autonomous_;
	Climber *climber_;

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

		joyOp = new Joystick(JOY_OP);
		joyThrottle = new Joystick(JOY_THROTTLE);
		joyWheel = new Joystick(JOY_WHEEL);

		drive_Controller = new DriveController();

		fly_wheel = new Flywheel();

		elevator_ = new Elevator();

		gear_rail = new GearRail();

		conveyor_ = new Conveyor();

		vision_ = new Vision();

		autonomous_ = new Autonomous();

		climber_ = new Climber();

		teleop_state_machine = new TeleopStateMachine(fly_wheel, conveyor_,
				gear_rail, elevator_, drive_Controller, vision_, climber_);

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

			if (allianceSelected == redAlliance) {

				std::cout << "Red" << std::endl;

			} else {

				std::cout << "Blue" << std::endl;

			}

		} else if (autoSelected == shootAndLoadAuton) {

			if (allianceSelected == redAlliance) {

				std::cout << "Red" << std::endl;

			} else {

				std::cout << "Blue" << std::endl;

			}

		}

	}

	void TeleopInit() {

		fly_wheel->StartThread();

	//	drive_Controller->StartThreads(joyThrottle, joyWheel, &is_kick);
	//	drive_Controller->KickerDown();

	}

	void TeleopPeriodic() {

		fly_wheel->FlywheelStateMachine();

		if (joyThrottle->GetRawButton(RETURN_BUTTON)) {

			fly_wheel->flywheel_state = fly_wheel->spin_state_h;

		}

		else {

			fly_wheel->flywheel_state = fly_wheel->stop_state_h;
		}

#if 0
		bool gear_button = joyOp->GetRawButton(GEAR_BUTTON);
		bool fire_button = joyOp->GetRawButton(FIRE_BUTTON);
		bool climb_button = joyOp->GetRawButton(CLIMB_BUTTON);
		bool return_button = joyOp->GetRawButton(RETURN_BUTTON);

		teleop_state_machine->StateMachine(gear_button, fire_button,
				climb_button, return_button);
		conveyor_->ConStateMachine();
		elevator_->ElevatorStateMachine();
		fly_wheel->FlywheelStateMachine();
		gear_rail->GearRailStateMachine();

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

#endif	//END DRIVECODE

	}

	void TestPeriodic() {
		CANTalon *canTalon = new CANTalon(CAN_TALON_BACK_RIGHT);

		std::cout << canTalon->GetEncPosition() << std::endl;

	}

private:

};

START_ROBOT_CLASS(Robot)
