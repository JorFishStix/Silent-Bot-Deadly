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
// fly1                 motor         10              
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
  while(abs(error) >= 5){
    error = pos - armPos.position(degrees);
    radialArm.setVelocity(error * pGain, pct);
    radialArm.spin(fwd);
    wait(20, msec);
  }
  radialArm.stop(hold);
}

void driveReset(float sec){
  Drivetrain.setTimeout(sec, seconds);
  Drivetrain.driveFor(forward, 100, inches);
  Drivetrain.driveFor(reverse, 3, inches);
  Drivetrain.setTimeout(2500, msec);
}

motor_group flywheel = motor_group(fly1, fly2);

// VARIABLES------------------------



  // PRE-AUTON
  bool started = false;

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
  while(1){
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(Drivetrain.heading(degrees));
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print(armPos.position(degrees));
    Brain.Screen.setCursor(3, 1);
    Brain.Screen.print(timeLeft - 10);
    Brain.Screen.print(" SECONDS LEFT");
    wait(20, msec);
  }
}

void autonomous(void) {
  Brain.Timer.reset();

  flywheel.setVelocity(127, pct);
  flywheel.setMaxTorque(127, pct);
  flywheel.setStopping(coast);
  radialArm.setMaxTorque(127, pct);
  radialArm.setStopping(hold);
  Drivetrain.setStopping(hold);
  Drivetrain.setDriveVelocity(90, pct);
  Drivetrain.setTurnVelocity(90, pct);
  Drivetrain.setTimeout(2500, msec);

  // Launch
  Drivetrain.driveFor(fwd, 25, inches);
  Drivetrain.turnToHeading(250, degrees);
  Drivetrain.driveFor(fwd, 17, inches);
  armTo(.8, 90);
  Drivetrain.driveFor(fwd, 1, inches);
  while(1){
    timeLeft = 60 - Brain.Timer.value();
    flywheel.spin(fwd, 10, volt);
    if(timeLeft <= 10){
      break;
    }
    wait(20, msec);
  }
  flywheel.stop(coast);
  
  // Push
  Drivetrain.driveFor(reverse, 15, inches);
  Drivetrain.turnToHeading(180, degrees);
  Drivetrain.driveFor(fwd, 23, inches);
  Drivetrain.setHeading(180, degrees);
  Drivetrain.driveFor(reverse, 0.5, inches);
  Drivetrain.turnFor(left, 45, degrees);
  armTo(.9, 95);
  Drivetrain.driveFor(fwd, 4, inches);
  Drivetrain.turnToHeading(90, degrees);
  Drivetrain.driveFor(forward, 32, inches);
  Drivetrain.driveFor(forward, 6, inches, false);
  radialArm.spin(reverse, 127, pct);

}

void usercontrol(void) {
  Brain.Timer.reset();
  started = true;
  Controller1.Screen.clearScreen();

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
      timeLeft = 60 - Brain.Timer.value();
      int flySpeed = (fly1.velocity(percent) + fly2.velocity(percent)) / 2;

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
