/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Drivetrain           drivetrain    3, 12, 9, 20, 8 
// radialArm            motor_group   19, 18          
// fly1                 motor         5               
// fly2                 motor         2               
// armPos               rotation      17              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// FUNCTIONS------------------------

void armTo(float pGain, int pos){ //P loop
  int error = pos - armPos.position(degrees);
  while(abs(error) >= 1){
    error = pos - armPos.position(degrees);
    radialArm.setVelocity(error * pGain, pct);
    radialArm.spin(fwd);
    wait(20, msec);
  }
  radialArm.stop(hold);
}

int selAuton = 1;
void nextAuton(){
  Brain.Screen.setFillColor(purple);
  Brain.Screen.drawCircle(240, 120, 50);
  Brain.Screen.setCursor(6, 23);
  Brain.Screen.print("NEXT");
  Brain.Screen.setFillColor(black); // Button shading
  
  selAuton ++;
  if(selAuton > 2){
    selAuton = 1;
  }
  Brain.Screen.clearLine(3);
  Brain.Screen.setCursor(3, 1);
  if(selAuton == 1){
    Brain.Screen.print("Close Side");
  } else if(selAuton == 2){
    Brain.Screen.print("Far Side");
  }
}

motor_group flywheel = motor_group(fly1, fly2);

// VARIABLES------------------------



  // PRE-AUTON
  bool started = false;
  bool closeSide = true;

  // AUTON

  // USER CONTROLS
  bool sponsored = false;
  bool endGame = false;

    // Ticks
    int ticks = 0;
    int tickTime = 20;

    // Flywheel
    int flyTime = 0;
    int flyGear = 12;
    bool flying = false;

    // Timer
    int timeLeft;
    int nextTime;



void pre_auton(void) {
  vexcodeInit();
  Brain.Screen.setFillColor(blue);
  Brain.Screen.drawCircle(240, 120, 50);
  Brain.Screen.setCursor(6, 23);
  Brain.Screen.print("NEXT");
  Brain.Screen.setFillColor(black);
  Brain.Screen.setCursor(3, 1);
  Brain.Screen.print("Close Side");

  while(!started){
    Brain.Screen.clearLine(1);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(Drivetrain.heading(degrees));
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print(armPos.position(degrees));

    if(Brain.Screen.pressing()){
      int pressX = Brain.Screen.xPosition();
      int pressY = Brain.Screen.yPosition();
      if(pressX <= 290 && pressX >= 190 && pressY <= 170 && pressY >= 70){
        nextAuton();
      }
    wait(250, msec);
      Brain.Screen.setFillColor(blue);
      Brain.Screen.drawCircle(240, 120, 50);
      Brain.Screen.setCursor(6, 23);
      Brain.Screen.print("NEXT");
      Brain.Screen.setFillColor(black);
    }
    wait(20, msec);
  }

}

void autonomous(void) {

  Drivetrain.setDriveVelocity(100, pct);
  Drivetrain.setTurnVelocity(80, pct);
  Drivetrain.setTimeout(2500, msec);

  if(selAuton == 1){
    armTo(1.5, 135);
    Drivetrain.driveFor(reverse, 30, inches);
    Drivetrain.turnToHeading(180, degrees);
    Drivetrain.driveFor(reverse, 5, inches);
    Drivetrain.driveFor(forward, 8, inches);
    armTo(1.5, 40);
    Drivetrain.turnToHeading(270, degrees);
    Drivetrain.driveFor(reverse, 12, inches);
    Drivetrain.turnToHeading(225, degrees);
    Drivetrain.driveFor(forward, 18, inches);
    armTo(1, 95);  
  } else if(selAuton == 2){
    armTo(1.5, 135);
    Drivetrain.driveFor(reverse, 30, inches);
    Drivetrain.turnToHeading(180, degrees);
    Drivetrain.driveFor(reverse, 5, inches);
    Drivetrain.driveFor(forward, 8, inches);
    armTo(1.5, 40);
    Drivetrain.turnToHeading(270, degrees);
    Drivetrain.driveFor(forward, 24, inches);
    Drivetrain.turnToHeading(0, degrees);
    Drivetrain.driveFor(forward, 36, inches);
  }


  
}

void usercontrol(void) {
  started = true;
  Controller1.Screen.clearScreen();
  Brain.Timer.reset();

  flywheel.setVelocity(127, pct);
  flywheel.setMaxTorque(127, pct);
  flywheel.setStopping(coast);
  radialArm.setVelocity(127, pct);
  radialArm.setMaxTorque(127, pct);
  radialArm.setStopping(hold);
  Drivetrain.setStopping(hold);

  if(Controller1.ButtonRight.pressing()){
    sponsored = false;
  } else{
    Brain.Screen.drawImageFromFile("van-griffith.png", 0, 0);
    sponsored = true;
  }

  while (1) {

    // Variables
      int flySpeed = (fly1.velocity(percent) + fly2.velocity(percent)) / 2;

    timeLeft = 105 - Brain.Timer.value();

    // Flywheel
      if(Controller1.ButtonR1.pressing() && flyTime >= 20){
        if(flying){
          flying = false;
          Controller1.Screen.clearLine(3);
        } else{
          flying = true;
          Controller1.Screen.clearLine(3);
          Controller1.Screen.setCursor(3, 1);
          Controller1.Screen.print("Flywheel Locked");
        }
        flyTime = 0;
      }

      if(flying || Controller1.ButtonR2.pressing()){
        flywheel.spin(fwd, flyGear, volt);
        if(flySpeed <= 60){
          Controller1.rumble(".");
        }
      } else{
        flywheel.stop(coast);
      }

    // Secondly

    if(timeLeft == nextTime){

      // Flywheel
        if(Controller1.ButtonUp.pressing()){
          flyGear ++;
          if(flyGear > 12){
            flyGear = 12;
          }
        } else if(Controller1.ButtonDown.pressing()){
          flyGear --;
          if(flyGear < 1){
            flyGear = 1;
          }
        }

      // Controller Display

        // Timer
          Controller1.Screen.clearLine(1);
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.print(timeLeft);
          Controller1.Screen.print(" seconds left");

        // Flywheel
          Controller1.Screen.clearLine(2);
          Controller1.Screen.setCursor(2,1);
          Controller1.Screen.print(int(fly1.velocity(percent)));
          Controller1.Screen.setCursor(2, 5);
          Controller1.Screen.print(int(fly1.temperature(percent)));
          Controller1.Screen.print(" / ");
          Controller1.Screen.print(int(fly2.velocity(percent)));
          Controller1.Screen.setCursor(2, 15);
          Controller1.Screen.print(int(fly2.temperature(percent)));
          Controller1.Screen.setCursor(2, 19);
          Controller1.Screen.print(flyGear);

        // Endgame
          if(timeLeft <= 15 && !endGame){
            Controller1.rumble("----");
            endGame = true;
          }

      // Brain Display
      if(!sponsored){
        Brain.Screen.clearScreen();

          // Ticks per second
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Ticks / sec: ");
            Brain.Screen.print(tickTime);

          // Battery percent
            Brain.Screen.setCursor(2, 1);
            Brain.Screen.print("Battery: ");
            Brain.Screen.print(Brain.Battery.capacity());

          // Flywheel
            Brain.Screen.setCursor(4, 1);
            Brain.Screen.print("Flywheel: ");
            Brain.Screen.setCursor(4, 11);
            Brain.Screen.print(int(fly1.velocity(pct)));
            Brain.Screen.setCursor(4, 15);
            Brain.Screen.print(int(fly1.temperature(pct)));
      } else{
        Brain.Screen.drawImageFromFile("van-griffith.png", 0, 0);
      }
    // Variable Update
      tickTime = ticks / Brain.Timer.value();

    }
    nextTime = timeLeft - 1;
    ticks ++;
    flyTime ++;
    wait(20, msec);
  }
}


int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}
