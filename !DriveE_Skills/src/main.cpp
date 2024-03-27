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
  while(abs(error) >= 1){
    error = pos - armPos.position(degrees);
    radialArm.setVelocity(error * pGain, pct);
    radialArm.spin(fwd);
    wait(20, msec);
  }
  radialArm.stop(hold);
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
  while(!started){
    Brain.Screen.clearLine(1);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(Drivetrain.heading(degrees));
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print(armPos.position(degrees));
    wait(20, msec);
  }

}

void autonomous(void) {

  Drivetrain.setDriveVelocity(100, pct);
  Drivetrain.setTurnVelocity(80, pct);
  Drivetrain.setTimeout(2500, msec);

  armTo(.8, 82);
  Drivetrain.driveFor(forward, 2, inches);
  flywheel.spin(fwd, 11, volt);
}


int main() {
  Competition.autonomous(autonomous);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}
