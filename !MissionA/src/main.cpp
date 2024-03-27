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
// frontL               motor         15              
// backL                motor         12              
// frontR               motor         13              
// backR                motor         14              
// inert                inertial      8               
// gpsensor             gps           7               
// intake               motor         5               
// fly                  motor         6               
// endgame1             motor         19              
// endgame2             motor         20              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "string"

using namespace vex;

// A global instance of competition
competition Competition;

motor_group lSide = motor_group(frontL, backL);
motor_group rSide = motor_group(frontR, backR);
motor_group dTrain = motor_group(frontL, frontR, backR, backL);
// motor_group inPlus = motor_group(inPlus1, inPlus2);

task Thread;

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  inert.calibrate();
  gpsensor.calibrate();
  /*
  inPlus.setPosition(0, deg);
  inPlus.setVelocity(100, pct);
  inPlus.setTimeout(800, msec);
  inPlus.setMaxTorque(50, pct);
  inPlus.setStopping(hold);
  */
}

void autonomous(void) {
  gpsensor.calibrate();
  gpsensor.resetHeading();

}


void usercontrol(void) {
  Controller1.Screen.clearScreen();
  Brain.Timer.clear();
  int inputTime = 0;
  bool trans = false;
  bool endGame = false;
  bool dead = false;
  bool slow = false;
  bool tank = false;
  bool flying = false;
  int nextTime = 0;
  int timeLeft = 0;
  int ticks = 0;
  int tickTime = 0;
  int nextBat = 0;
  int tankTime = 0;
  int flyTime = 0;

  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("104 Seconds left");
  Controller1.Screen.setCursor(3, 1);
  Controller1.Screen.print("FWD Drive");
  Controller1.Screen.setCursor(2, 1);
  Controller1.Screen.print("Speed Mode");

  Brain.Screen.clearLine(3);
  Brain.Screen.setCursor(3,1);
  Brain.Screen.print("---MOTORS---");

  intake.setBrake(hold);
  intake.setVelocity(100, pct);

  

  while (1) {

    // Drivetrain variables

    int lStickV = Controller1.Axis3.position(pct);
    int rStickH = Controller1.Axis1.position(pct);


    if(Brain.Timer.time() >= 5){

    if (Controller1.ButtonLeft.pressing() && inputTime >= (tickTime * .75) && trans) {
      trans = false;
      inputTime = 0;
      Controller1.Screen.setCursor(3, 1);
      Controller1.Screen.clearLine(3);
      Controller1.Screen.print("FWD Drive");
    } else if (Controller1.ButtonLeft.pressing() && inputTime >= (tickTime * .75) && !trans) {
      trans = true;
      inputTime = 0;
      Controller1.Screen.setCursor(3, 1);
      Controller1.Screen.clearLine(3);
      Controller1.Screen.print("REV Drive");
    }

    // Tank controls

    if (Controller1.ButtonA.pressing() && tankTime >= (tickTime * .75) && tank) {
      tank = false;
      tankTime = 0;
      Controller1.Screen.setCursor(2, 1);
      Controller1.Screen.clearLine(2);
      Controller1.Screen.print("Speed Mode");
    } else if (Controller1.ButtonA.pressing() && tankTime >= (tickTime * .75) && !tank) {
      tank = true;
      tankTime = 0;
      Controller1.Screen.setCursor(2, 1);
      Controller1.Screen.clearLine(2);
      Controller1.Screen.print("Tank Mode");
    }
    }

    // Deadzone + Transmission (Drivetrain Controls)

    if (tank){
      lStickV /= 2;
      rStickH /= 2;
    }

    if (abs(rStickH) > 15 || abs(lStickV) > 15) {
      if (trans) {
        lSide.spin(reverse, lStickV - rStickH, pct);
        rSide.spin(reverse, lStickV + rStickH, pct);
      } else {
        lSide.spin(fwd, lStickV + rStickH, pct);
        rSide.spin(fwd, lStickV - rStickH, pct);
      }
    } else if (tank){ // Brakes (tank)
      backL.stop(hold);
      backR.stop(hold);
      frontL.stop(hold);
      frontR.stop(hold);
    } else {          // Brakes (speed)
      backL.stop(coast);
      backR.stop(hold);
      frontL.stop(hold);
      frontR.stop(coast);
    }


    // Flywheel

    /*

    if(Controller1.ButtonR2.pressing() && !flying && flyTime >= (tickTime * .5)){
      flyTime = 0;
      flying = true;
    } else if(Controller1.ButtonR2.pressing() && flying && flyTime >= (tickTime * .5)){
      flyTime = 0;
      flying = false;
    }

    if(flying){
      fly.spin(fwd, 100, pct);
      if(fly.velocity(pct) <= 80){
        Controller1.rumble(".");
      }
    } else{
      fly.stop(coast);
    }

    */

    Brain.Screen.clearLine(4);
    Brain.Screen.setCursor(4, 1);
    Brain.Screen.print("Flywheel: ");
    Brain.Screen.setCursor(4, 11);
    Brain.Screen.print(int(fly.velocity(pct)));
    Brain.Screen.setCursor(4, 16);
    Brain.Screen.print(flying);

    // Endgame alert

    if (Brain.Timer.time(sec) >= 80 && !endGame) {
      Controller1.rumble("----");
      endGame = true;
    }


    // Every second
    timeLeft = 105 - Brain.Timer.value();

    if (timeLeft == nextTime) {

      // Print ticks / sec on brain

      tickTime = ticks / Brain.Timer.value();
      Brain.Screen.clearLine(1);
      Brain.Screen.setCursor(1, 1);
      Brain.Screen.print("Ticks / sec: ");
      Brain.Screen.print(tickTime);

      // Controller timer

      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.clearLine(1);
      Controller1.Screen.print(timeLeft);
      Controller1.Screen.print(" seconds left");

      // Alerts

      if (tickTime < 20 && !slow) {                     // TPS alert
        Controller1.Screen.clearLine(2);
        Controller1.Screen.setCursor(2, 1);
        Controller1.Screen.print("SLOW PROGRAM");
        slow = true;
      }

      if (Brain.Battery.capacity() < 25 && !dead) {     // Battery alert
        Controller1.Screen.clearLine(2);
        Controller1.Screen.setCursor(2, 1);
        Controller1.Screen.print("REPLACE BATTERY");
        dead = true;
      }



      // Brain monitors (Secondly)

      Brain.Screen.setCursor(2,1);
      Brain.Screen.clearLine(2);
      Brain.Screen.print("Battery percent: ");
      Brain.Screen.print(Brain.Battery.capacity());

    }

    // Update 'next' variables

    ticks++;
    tankTime++;
    nextTime = timeLeft - 1;
    inputTime ++;
    flyTime ++;
    nextBat = Brain.Battery.capacity() - 1;
    wait(20, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}


// QUOTE BOOK //

/*
"Okay well take your finger out of it, first of all.."
"Stick it where?"
"Look at my load"
"I can't even get inside this one because it's screwed"
"Well put it in already"
"cuz it ain't long enough"
"lemme hit it"
"nicolas can i smash that"
"ok dont put that in th-" "putitinthequotebook"
"I'm taking cooper home" "aw your car's gonna get white all over it :<"
"granburyrobotics.com"
"Little kid, why are you eating that meat so ferociously?"
"Just put it in already!"
"We milk Ethan."
*/