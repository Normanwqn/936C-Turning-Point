//Declaration of Motors, Brain, Controller, etc
using namespace vex;
vex::brain Brain;
vex::controller Controller1;
vex::motor FrontLeft(vex::PORT21, vex::gearSetting::ratio18_1, false);
vex::motor BackLeft(vex::PORT3, vex::gearSetting::ratio18_1, false);
vex::motor FrontRight(vex::PORT9, vex::gearSetting::ratio18_1, false);
vex::motor BackRight(vex::PORT2, vex::gearSetting::ratio18_1, false);
vex::motor LiftMotor1(vex::PORT1,vex::gearSetting::ratio18_1, true );
vex::motor LiftMotor2(vex::PORT16,vex::gearSetting::ratio18_1, false );
vex::motor ClawMotor(vex::PORT10,vex::gearSetting::ratio18_1, false );
vex::motor PuncherMotor(vex::PORT8,vex::gearSetting::ratio18_1, false );


class Claw{
public:
    Claw(){
        ClawMotor.setRotation(0,rotationUnits::deg);
    }
    void clawOperation(){
        while (true){
            if (Controller1.ButtonX.pressing()){
                ClawMotor.rotateTo(ClawMotor.rotation(rotationUnits::deg)+175,rotationUnits::deg,60,velocityUnits::pct,false);
                vex::task::sleep(1500);
            }
        }
    }
};

class Lift{

public:

    int pos[3];
    //int timeRun = 0;
    int currentLiftIndex = 0;
    bool sleep = false;
    int intialError = -60;
    Lift(){
        LiftMotor1.setRotation(0,rotationUnits::deg);
        LiftMotor2.setRotation(0,rotationUnits::deg);
        LiftMotor1.setMaxTorque(100,percentUnits::pct);
        LiftMotor2.setMaxTorque(100,percentUnits::pct);
        pos[0] = -30; pos[1] = 220; pos[2] = 600;
    }
    void lowerLift(void){
        if (currentLiftIndex != 0){
            currentLiftIndex = currentLiftIndex -1;
        } else {
            //timeRun ++;
        }
    }
    void raiseLift(void){
        if (currentLiftIndex != 2){
            currentLiftIndex = currentLiftIndex+1;
        } else {
            //timeRun ++;
        }
    }
    int liftOperation(){
        bool motorOff = false;
        if (Controller1.ButtonL2.pressing()){
            lowerLift();
            //vex::task::sleep(500);
        }
        if (Controller1.ButtonR2.pressing()){
            raiseLift();

            //vex::task::sleep(500);
        }
        //Controller1.ButtonR2.pressed(lowerLift());
        //Controller1.ButtonL2.pressed(raiseLift(void));

        LiftMotor1.startRotateTo(pos[currentLiftIndex], rotationUnits::deg,70,velocityUnits::pct);
        LiftMotor2.startRotateTo(pos[currentLiftIndex], rotationUnits::deg,70,velocityUnits::pct);
        //vex::task::sleep(500);
        if (Controller1.ButtonB.pressing()){
            motorOff = !motorOff;
        }
        if (motorOff){
            LiftMotor1.stop();
            LiftMotor2.stop();
        }
        Brain.Screen.setCursor(7,1);
        Brain.Screen.print("CurrentLiftIndex =%d",currentLiftIndex);
        Brain.Screen.render();

        return 0;
    }
    int manualLift(){
        if ((Controller1.ButtonR2.pressing()) || (Controller1.ButtonL2.pressing())){
            if (Controller1.ButtonR2.pressing()){
                LiftMotor1.spin(directionType::fwd,100, velocityUnits::pct);
                LiftMotor2.spin(directionType::fwd,100, velocityUnits::pct);
            }
            if (Controller1.ButtonL2.pressing()){
                LiftMotor1.spin(directionType::rev,100, velocityUnits::pct);
                LiftMotor2.spin(directionType::rev,100, velocityUnits::pct);

            }
        }
        else{
            LiftMotor1.stop();
            LiftMotor2.stop();
        }
        return 0;
    }

};

class Drive{
public:
    int inverse = 1;
    double unitRotation = 18.5;
    double piTurn = 642;
    double unevenRatio = 0.995;
    void stopMotor(){
        FrontLeft.stop();
        FrontRight.stop();
        BackLeft.stop();
        BackRight.stop();
    }
    void moveX(double x, int power){
        // double unitRotation = 734;//the magnitude of rotation caused by one unit distance
        double dFL = x * unitRotation; //Change in the rotation in the Front Left Motor
        double dBL = -(x * unitRotation); //Change in the rotation in the Back Left Motor
        double dFR =  x * unitRotation; //Change in the rotation in the Front Right Motor
        double dBR = -(x * unitRotation); //Change in the rotation in the Back Right Motor
        FrontLeft.rotateFor(dFL, rotationUnits::deg, unevenRatio*power,velocityUnits::pct, false);
        BackLeft.rotateFor(dBL, rotationUnits::deg,unevenRatio*power, velocityUnits::pct, false);
        FrontRight.rotateFor(dFR, rotationUnits::deg, power,velocityUnits::pct, false);
        BackRight.rotateFor(dBR,  rotationUnits::deg, power,velocityUnits::pct, false);
        vex::task::sleep(2000);
    }
    void moveY(double y, int power){
        // double unitRotation = 734;//the magnitude of rotation caused by one unit distance
        double dFL = y * unitRotation; //Change in the rotation in the Front Left Motor
        double dBL = (y * unitRotation); //Change in the rotation in the Back Left Motor
        double dFR =  -(y * unitRotation); //Change in the rotation in the Front Right Motor
        double dBR = -(y * unitRotation); //Change in the rotation in the Back Right Motor
        FrontLeft.rotateFor(dFL, rotationUnits::deg,unevenRatio*power,velocityUnits::pct, false);
        BackLeft.rotateFor(dBL, rotationUnits::deg,unevenRatio*power,velocityUnits::pct, false);
        FrontRight.rotateFor(dFR, rotationUnits::deg,power,velocityUnits::pct, false);
        BackRight.rotateFor(dBR,  rotationUnits::deg,power,velocityUnits::pct, false);
        vex::task::sleep(2500);
    }
    void rotate(double angle){
        //double piTurn = 642;
        double turnAngle = (angle)/(180.0)*piTurn;
        FrontLeft.setRotation(0, rotationUnits::deg);
        FrontRight.setRotation(0, rotationUnits::deg);
        BackLeft.setRotation(0, rotationUnits::deg);
        BackRight.setRotation(0, rotationUnits::deg);
        FrontLeft.rotateFor(turnAngle,rotationUnits::deg,100,velocityUnits::pct,false);
        FrontRight.rotateFor(turnAngle,rotationUnits::deg,100,velocityUnits::pct,false);
        BackLeft.rotateFor(turnAngle,rotationUnits::deg,100,velocityUnits::pct,false);
        BackRight.rotateFor(turnAngle,rotationUnits::deg,100,velocityUnits::pct,false);
        vex::task::sleep(2500);
    }
    void rotateBack(){
        inverse = (-1)*inverse;
        double angle = 642;
        FrontLeft.setRotation(0, rotationUnits::deg);
        FrontRight.setRotation(0, rotationUnits::deg);
        BackLeft.setRotation(0, rotationUnits::deg);
        BackRight.setRotation(0, rotationUnits::deg);
        FrontLeft.rotateFor(angle,rotationUnits::deg,100,velocityUnits::pct,false);
        FrontRight.rotateFor(angle,rotationUnits::deg,100,velocityUnits::pct,false);
        BackLeft.rotateFor(angle,rotationUnits::deg,100,velocityUnits::pct,false);
        BackRight.rotateFor(angle,rotationUnits::deg,100,velocityUnits::pct,false);
        //vex::task::sleep(200);
    }
    int driveOperation(){
        while (true){
            //Power Mode
            double power = 0.5+Controller1.Axis2.position(percentUnits::pct)/100;
            //Joystick
            double y = 1.2*(Controller1.Axis3.position(percentUnits::pct));
            double x = 1.2*(Controller1.Axis4.position(percentUnits::pct));
            double r = 0;// (Controller1.Axis1.position(percentUnits::pct));
            //Tank Rotation
            if (Controller1.ButtonA.pressing()){
                r = 35;
            }
            if (Controller1.ButtonY.pressing()){
                r = (-35);
            }
            //Tank Drive
            if (Controller1.ButtonUp.pressing()){
                y = 60;
            }
            if (Controller1.ButtonDown.pressing()){
                y = (-60);
            }
            if (Controller1.ButtonRight.pressing()){
                x = 60;
            }
            if (Controller1.ButtonLeft.pressing()){
                x = (-60);
            }
            double threshold = 5;
            if (abs(x) < threshold) {
                x = 0;
            }
            if (abs(y) < threshold) {
                y = 0;
            }

            /*if (Controller1.ButtonB.pressing()){
                rotateBack();
                vex::task::sleep(1100);
            }*/
            if ( (abs(x) > threshold) || (abs(y) > threshold) || (abs(r)>0)){
                FrontLeft.spin(directionType::fwd, unevenRatio*power*inverse*(y+x+r), velocityUnits::pct);
                FrontRight.spin(directionType::fwd, power*inverse*(-y+x+r), velocityUnits::pct);
                BackRight.spin(directionType::fwd, power*inverse*(-y-x+r), velocityUnits::pct);
                BackLeft.spin(directionType::fwd, unevenRatio*power*inverse*(y-x+r), velocityUnits::pct);
            } else{
                FrontLeft.stop();
                FrontRight.stop();
                BackRight.stop();
                BackLeft.stop();
            }
            Controller1.Screen.print( "x = %f", x);
            Controller1.Screen.setCursor( 2, 1 );
            Controller1.Screen.print( "y = %f", y);
            Controller1.Screen.setCursor( 3, 1 );
            Controller1.Screen.print( "r = %f", r);
            Brain.Screen.clearLine(1,color::black);
            Brain.Screen.setCursor(1,1);
            Brain.Screen.print("FL = %f", FrontLeft.rotation(rotationUnits::deg));
            Brain.Screen.clearLine(2,color::black);
            Brain.Screen.setCursor(2,1);
            Brain.Screen.print("FR = %f", FrontRight.rotation(rotationUnits::deg));
            Brain.Screen.clearLine(3,color::black);
            Brain.Screen.setCursor(3,1);
            Brain.Screen.print("BL = %f", BackLeft.rotation(rotationUnits::deg));
            Brain.Screen.clearLine(4,color::black);
            Brain.Screen.setCursor(4,1);
            Brain.Screen.print("BR = %f", BackRight.rotation(rotationUnits::deg));
            Brain.Screen.setCursor(5,1);
            Brain.Screen.print("Lift1 = %f", LiftMotor1.rotation(rotationUnits::deg));
            Brain.Screen.render();
            Brain.Screen.setCursor(6,1);
            Brain.Screen.print("Lift2 = %f", LiftMotor2.rotation(rotationUnits::deg));
            Brain.Screen.render();

        }
    }
};




