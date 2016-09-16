#include "WPILib.h"
//#include "math.h"

class Robot: public IterativeRobot
{
	CANTalon *lDrive;
	CANTalon *rDrive;
	//RobotDrive *myRobot; // robot drive system
	Joystick *driveStick; // only joydriveStick
	Joystick *manipulatorStick;
	CANTalon *lifter; //bat
	CANTalon *rake; //bird
	//Talon rake; //fish
	//Talon motor; //kitty
	//LiveWindow *lw;
	//int autoLoopCounter;

	//bool btnVal = SmartDashboard::GetBoolean("DB/Button 1");

public:
	Robot() {
		Wait(1);
		lDrive = new CANTalon(3);
		rDrive = new CANTalon(1);
		//myRobot = new RobotDrive(lDrive,rDrive);	// these must be initialized in the same order
		driveStick = new Joystick(0);// as they are declared above.
		manipulatorStick = new Joystick(1);
		lifter = new CANTalon(2);
		rake = new CANTalon(4);
		//rake(3),
		//motor(1),
		//lw = new LiveWindow(NULL);
		//autoLoopCounter()
		//myRobot->SetExpiration(2);
	}

private:


	double leftX = 0;
	double leftY = 0;
	double rightX = 0;
	double rightY = 0;
	double mLeftY = 0;
	double mRightY = 0;
	double leftTrigger = 0;
	double rightTrigger = 0;
	double threshold = 0.1;
	double cruise = 0;
	double rCruise = 0;
	void RobotInit() override
	{
		CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
		//lw = LiveWindow::GetInstance();
		//SmartDashboard::PutString("DB/String 0", "Btn 0 sets container on totes");
	}

	void AutonomousInit()
	{
		//autoLoopCounter = 0;

		rake->Set(0.5);
		Wait(2);
		rake->Set(0);
		lDrive->Set(-0.25);
		rDrive->Set(0.25);
		//myRobot.Drive(0.25, 0.0);
		Wait(1);
		lDrive->Set(0);
		rDrive->Set(0);
		//myRobot.Drive(0.0, 0.0);
		rake->Set(-0.5);
		Wait(3);
		//rake->Set(-0.3); // FOR CONTAINERS
		rake->Set(0); // FOR NO CONTAINERS
		lDrive->Set(0.25);
		rDrive->Set(-0.25);
		//myRobot.Drive(-1.0, 0.0);
		Wait(4); // Testing Field
		//Wait(6); //Full Field
		lDrive->Set(0);
		rDrive->Set(0);
		//myRobot.Drive(0.0, 0.0);
		//rake->Set(0.25);
		//Wait(1);
		//rake->Set(0);
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

		leftX = driveStick->GetRawAxis(0);
		if(fabs(leftX) < threshold)
			leftX = 0;
		leftY = driveStick->GetRawAxis(1);
		if(fabs(leftY) < threshold)
			leftY = 0;
		mLeftY = manipulatorStick->GetRawAxis(1);
		if(fabs(mLeftY) < threshold)
			mLeftY = 0;
		rightX = driveStick->GetRawAxis(4);
		if(fabs(rightX) < threshold)
			rightX = 0;
		rightY = driveStick->GetRawAxis(5);
		if(fabs(rightY) < threshold)
			rightY = 0;
		mRightY = manipulatorStick->GetRawAxis(5);
		if(fabs(mRightY) < threshold)
			mRightY = 0;
		leftTrigger = (driveStick->GetRawAxis(2));
		if(fabs(leftTrigger) < (threshold))
			leftTrigger = 0;
		rightTrigger = (driveStick->GetRawAxis(3));
		if(fabs(rightTrigger) < (threshold))
			rightTrigger = 0;

		leftX = pow(leftX, 3.0);
		leftY = pow(leftY, 3.0);
		rightX = pow(rightX, 3.0);
		rightY = pow(rightY, 3.0);
		mRightY = pow(mRightY, 3.0);

		if(driveStick->GetRawButton(6)) {
			rake->Set(rightY/1.5);
			//rake->Set(0);
			lifter->Set(0);
		} else if(manipulatorStick->GetRawButton(6)) {
			rake->Set(mRightY);
			/*if(driveStick->GetRawButton(2)) {
				rake->Set(1);
			} else {
				rake->Set(0);
			}*/
			lifter->Set((rightY)/1.5);
		} else {
			rake->Set(0);
			lifter->Set((rightY)/1.5);
		}

		//myRobot->ArcadeDrive((-leftY/1.1), ((-leftX/1.1) - (rightTrigger * leftY) + (leftTrigger * leftY))); // drive with arcade style (use correct driveStick)
		lDrive->Set(-((leftY - leftX) * (1-leftTrigger)));
		rDrive->Set((leftY + leftX) * (1-rightTrigger));
		//lDrive->Set(-(leftY - (leftX + leftTrigger*leftY)));
		//rDrive->Set(leftY + (leftX - rightTrigger*leftY));
			//lifter2.Set((rightY + cruise)/1.1);

		/*if(driveStick->GetRawButton(4)) {
			rake->Set(0.4);
		} else if(driveStick->GetRawButton(1)) {
			rake->Set(-0.4);
		} else {
			rake->Set(0);
		}*/

		if(driveStick->GetRawButton(5)) {
			cruise = rightY;
		}
		if(driveStick->GetRawButton(10)) {
			cruise = 0;
		}
		if(driveStick->GetRawButton(9)) {
			rCruise = rightY;
		}
		/*if(manipulatorStick->GetRawButton(3)) {
			CameraServer::GetInstance()->StartAutomaticCapture("cam1");
		}
		if(manipulatorStick->GetRawButton(2)) {
			CameraServer::GetInstance()->StartAutomaticCapture("cam0");
		}*/
	}

	void TestPeriodic()
	{
		//lw->Run();
	}

	void DisabledPeriodic()
	{

	}

};

START_ROBOT_CLASS(Robot);