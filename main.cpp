#include "robot-config.h"
#include <iostream>
#include <string>
#include <iomanip> // setprecision
#include <sstream>
//https://help.vex.com/article/68-vcs-example-program-vex-c-v5-clawbot-driving-forward-and-backward
//https://www.vexforum.com/index.php/34446-vex-c-how-to-multitask/0
//http://help.vexcodingstudio.com/#pro/namespacevex/classvex_1_1motor/startRotateTo


//Creates a competition object that allows access to Competition methods.
vex::competition    Competition;
int autoMode;



/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/



void pre_auton( void ) {

    // All activities that occur before the competition starts
    // Example: clearing encoders, setting servo positions, ...
    Controller1.Screen.clearScreen();
    // Brain.Screen.clearLine(1,color::black);
    Brain.Screen.clearLine();
    FrontLeft.setRotation(0, rotationUnits::deg);
    FrontRight.setRotation(0, rotationUnits::deg);
    BackLeft.setRotation(0, rotationUnits::deg);
    BackRight.setRotation(0,rotationUnits::deg);
    LiftMotor1.setRotation(0, rotationUnits::deg);
    LiftMotor2.setRotation(0, rotationUnits::deg);
    ClawMotor.setRotation(0,rotationUnits::deg);
    PuncherMotor.setRotation(0,rotationUnits::deg);
    Brain.Screen.drawRectangle(0, 0, 240, 136, vex::color(255,0,0)); // Red 1
    Brain.Screen.printAt(35,75,"RED Near the Flags");
    Brain.Screen.drawRectangle(0,136,240,272,vex::color(255,0,0));
    Brain.Screen.printAt(12,205,"RED Further from Flags");
    Brain.Screen.drawRectangle(240, 0, 480, 136, vex::color(0,0,255));
    Brain.Screen.printAt(260,75,"BLUE Near from Flags");
    Brain.Screen.drawRectangle(240, 136, 480, 272, vex::color(0,0,255));
    Brain.Screen.printAt(246,205,"BLUE Further from Flags");
    Brain.Screen.render();
    int xPos = Brain.Screen.xPosition();

    int yPos = Brain.Screen.yPosition();
    if ((xPos > 0) && (xPos<240) && (yPos>0) && (yPos<136)){
        autoMode = 1;

    }
    if ((xPos > 0) && (xPos<240) && (yPos>136) && (yPos<272)){
        autoMode = 2;

    }
    if ((xPos > 240) && (xPos<280) && (yPos>0) && (yPos<136)){
        autoMode = 3;

    }
    if ((xPos > 240) && (xPos<280) && (yPos>136) && (yPos<272)){
        autoMode = 4;

    }
    Controller1.Screen.setCursor( 1, 1 );
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/


void autonomous() {
    // ..........................................................................
    // Insert autonomous user code here.
    // ..........................................................................
    Drive aDrive;

    int rSpeed = 55;
    //aDrive.moveY(30, rSpeed);
    double LR; //1 denotes starting from the Red Side; -1 denotes starting from the Blue Side
    bool far;
    if (autoMode == 1){
        LR = 1;
        far = false;
    }
    if (autoMode == 2){
        LR = 1;
        far = true;
    }
    if (autoMode == 3){
        LR = -1;
        far = false;
    }
    if (autoMode == 4){
        LR = -1;
        far = true;
    }
    autoMode = 1;
    LR = 1;
    far = false;
    //PuncherMotor.spin(directionType::fwd,100, velocityUnits::pct);
    //vex::task::sleep(1000);
    //PuncherMotor.stop();
    //Step 1. Shift Left/Right
    double shiftDistance = 3.68;
    //aDrive.moveX(-1*LR*shiftDistance,rSpeed);
    //Step 2. Moving forwarding to the first flag and hitting the flag
    double firstFlagDistance = 52.04; //22.1*(0.5+1+0.9)
    if (far){
        firstFlagDistance = 100;//22.1*(0.5+3+0.9)
    }
    aDrive.moveY(firstFlagDistance , rSpeed);
    //Step 3. Moving back from the first flag and prepare to shift to the other flag
    double backFirst = -19.89; //22.1*0.9
    aDrive.moveY(backFirst, rSpeed);
    //Step 4. Moving to the Right/Left to the second flag
    double secondLRMove = 54.2; //22.1*(0.6+1+0.8)
    aDrive.moveX(LR*secondLRMove,rSpeed);
    //Step 5. Moving Forward to the second flag and hitting the flag
    double secondFlagDistance = firstFlagDistance;
    aDrive.moveY(secondFlagDistance,rSpeed);
    //Step 6. Moving Backward to get out
    double backSecond = -19.89;
    aDrive.moveY(backSecond,rSpeed);
    //Step 7 Moving LR back to the main axis
    double thirdLRMove = -secondLRMove;
    aDrive.moveX(LR*thirdLRMove,rSpeed);
    //Step 8. Moving backward to prepare for turning and landing
    double backThird = -55.25;
    aDrive.moveY(backThird,rSpeed);
    aDrive.rotate(LR*90);
    aDrive.moveX(LR*57.46,100);
    aDrive.stopMotor();
}






int drive(){
    Drive m_Drive;
    m_Drive.driveOperation();
    return 1;
}

int lift(){
    Lift m_Lift;
    while (true){
        m_Lift.liftOperation();
    }
    return 1;
}
int claw(){
    Claw m_Claw;
    m_Claw.clawOperation();
    return 1;
}

void usercontrol( ) {
    // User control code here, inside the loop
    //Lift m_Lift;
    Brain.Screen.clearScreen();
    Brain.Screen.render();
    vex::task t_drive(drive,0);
    vex::task t_Lift(lift,1);
    vex::task t_Claw(claw,2);

    /*while (1) {
      // This is the main execution loop for the user control program.
      // Each time through the loop your program should update motor + servo
      // values based on feedback from the joysticks.
      //m_Lift.liftOperation();


      // ........................................................................
      // Insert user code here. This is where you use the joystick values to
      // update your motors, etc.
      // ........................................................................

      if (Controller1.ButtonX.pressing()){
         FrontLeft.setRotation(0,rotationUnits::deg);
         BackLeft.setRotation(0,rotationUnits::deg);
         FrontRight.setRotation(0,rotationUnits::deg);
         BackRight.setRotation(0,rotationUnits::deg);
         LiftMotor1.setRotation(0,rotationUnits::deg);
         LiftMotor2.setRotation(0,rotationUnits::deg);
         ClawMotor.setRotation(0,rotationUnits::deg);
         Puncher.setRotation(0,rotationUnits::deg);
      }
      vex::task::sleep(20); //Sleep the task for a short amount of time to prevent wasted resources.
    }*/
}


int main() {

    //Run the pre-autonomous function.
    pre_auton();

    //Set up callbacks for autonomous and driver control periods.
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );

    //Prevent main from exiting with an infinite loop.
    while(1) {


        vex::task::sleep(100);//Sleep the task for a short amount of time to prevent wasted resources.
    }

}
