#include "WPILib.h"
#include "math.h"

class Robot: public IterativeRobot
{

	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	Joystick manipulatorStick;
	CANTalon lifter; //bat
	CANTalon lifter2; //bird
	Talon rake; //fish
	Talon motor; //kitty
	Compressor *airUpper = new Compressor(0);
	LiveWindow *lw;
	int autoLoopCounter;
	double leftX = 0;
	double leftY = 0;
	double rightX = 0;
	double rightY = 0;
	double leftTrigger = 0;
	double rightTrigger = 0;
	double threshold = 0.15;
	double cruise = 0;
	bool btnVal = SmartDashboard::GetBoolean("DB/Button 0");

public:
	Robot() :
		myRobot(3, 1),	// these must be initialized in the same order
		lifter(4),
		lifter2(2),
		rake(5),
		motor(6),
		stick(0),// as they are declared above.
		manipulatorStick(1),
		lw(NULL),
		autoLoopCounter(0)
	{
		myRobot.SetExpiration(0.5);
	}

private:
	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		SmartDashboard::PutString("DB/String 0", "Btn 0 sets container on totes");
	}

	void AutonomousInit()
	{
		autoLoopCounter = 0;

		rake.Set(-0.125);
		Wait(2);
		rake.Set(0);
		myRobot.Drive(0.25, 0.0);
		Wait(1);
		myRobot.Drive(0.0, 0.0);
		rake.Set(0.125);
		Wait(1);
		rake.Set(0);
		if(btnVal == true) {
			myRobot.Drive(-0.5, 0.0);
			Wait(1);
			myRobot.Drive(0.0, 0.0);
			rake.Set(-0.125);
			Wait(1);
		}
		rake.Set(0);
		myRobot.Drive(-1.0, 0.0);
		if(btnVal == false) {
			Wait(0.5);
		}
		Wait(2);
		myRobot.Drive(0.0, 0.0);
		rake.Set(0.25);
		Wait(1);
		rake.Set(0);
	}

	void AutonomousPeriodic()
	{
		/*if(autoLoopCounter < 100) //Check if we've completed 100 loops (approximately 2 seconds)
		{
			myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
			autoLoopCounter++;
			} else {
			myRobot.Drive(0.0, 0.0); 	// stop robot
		}*/
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{

		leftX = stick.GetRawAxis(0);
		if(fabs(leftX) < threshold)
			leftX = 0;
		leftY = stick.GetRawAxis(1);
		if(fabs(leftY) < threshold)
			leftY = 0;
		rightX = stick.GetRawAxis(4);
		if(fabs(rightX) < threshold)
			rightX = 0;
		rightY = stick.GetRawAxis(5);
		if(fabs(rightY) < threshold)
			rightY = 0;
		leftTrigger = (stick.GetRawAxis(5)-0.5);
		if(fabs(leftTrigger) < (threshold-0.5))
			leftTrigger = 0;
		rightTrigger = (stick.GetRawAxis(5)-0.5);
		if(fabs(rightTrigger) < (threshold-0.5))
			rightTrigger = 0;

		leftX = pow(leftX, 3.0);
		leftY = pow(leftY, 3.0);
		rightX = pow(rightX, 3.0);
		rightY = pow(rightY, 3.0);
		leftTrigger = pow(leftTrigger, 3.0);
		rightTrigger = pow(rightTrigger, 3.0);

		airUpper->SetClosedLoopControl(true);
		myRobot.ArcadeDrive((-leftY/1.1), ((-leftX/1.1)) - (rightTrigger * leftY) + (leftTrigger * leftY)); // drive with arcade style (use correct stick)
		lifter.Set((rightY + cruise)/1.1);
		lifter2.Set((rightY + cruise)/1.1);
		if(stick.GetRawButton(4)) {
			rake.Set(0.2);
		} else if(stick.GetRawButton(1)) {
			rake.Set(-0.2);
		} else {
			rake.Set(0);
		}
		if(stick.GetRawButton(5)) {
			cruise = rightY;
		}
		if(stick.GetRawButton(6)) {
			cruise = 0;
		}
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
