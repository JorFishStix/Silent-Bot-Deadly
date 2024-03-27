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
// frontL               motor         20              
// backL                motor         9               
// frontR               motor         13              
// backR                motor         8               
// fly1                 motor         12              
// radialArm            motor_group   5, 10           
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

competition Competition;

motor_group lSide = motor_group(frontL, backL);
motor_group rSide = motor_group(frontR, backR);
motor_group dTrain = motor_group(frontL, frontR, backR, backL);

// Motor Groups



void pre_auton(void) {
  vexcodeInit();

}

void autonomous(void) {
  
}

void driving(void){

  int inputTime = 0;
  int interval = 0;
  bool trans = false;
  int lStickV = Controller1.Axis3.position(percent);
  int rStickH = Controller1.Axis1.position(percent);
  
  //  Transmission Toggle
        if(Controller1.ButtonLeft.pressing() && inputTime >= interval && trans) {
        trans = false;
        inputTime = 0;
        Controller1.Screen.setCursor(3,1);
        Controller1.Screen.clearLine(3);
        Controller1.Screen.print("FWD Drive");
      } else if (Controller1.ButtonLeft.pressing() && inputTime >= interval && !trans) {
        trans = true;
        inputTime = 0;
        Controller1.Screen.setCursor(3,1);
        Controller1.Screen.clearLine(3);
        Controller1.Screen.print("REV Drive");
      }

    // Tank toggle

    /*

      if (Controller1.ButtonA.pressing() && tankTime >= interval && tank) {
        tank = false;
        tankTime = 0;
        Controller1.Screen.setCursor(2,1);
        Controller1.Screen.clearLine(2);
        Controller1.Screen.print("Speed Mode");
      } else if (Controller1.ButtonA.pressing() && tankTime >= interval && !tank) {
        tank = true;
        tankTime = 0;
        Controller1.Screen.setCursor(2,1);
        Controller1.Screen.clearLine(2);
        Controller1.Screen.print("Tank Mode");
      } // End tank toggle
    } // End delay

      */

    

    // Deadzone + Transmission (Drivetrain Controls)

    if (abs(rStickH) > 15 || abs(lStickV) > 15) { // Deadzone
    
    // Flips controls for transmission

      if (trans) {
        lSide.spin(reverse, lStickV - rStickH, pct);
        rSide.spin(reverse, lStickV + rStickH, pct);
      } else {
        lSide.spin(fwd, lStickV + rStickH, pct);
        rSide.spin(fwd, lStickV - rStickH, pct);
      }
    } else {          // Brakes
      backL.stop(hold);
      backR.stop(hold);
      frontL.stop(hold);
      frontR.stop(hold);
    }

}



void usercontrol(void) {

  // DECLARATION OF VARIABLES
    int nextTime = 0;   // Used to run secondly code
    int timeLeft = 0;   // Used for controller timer & secondly code
      bool endGame = false;
    int ticks = 0;      // Used to check speed of program
    int tickTime = 0;   // Ticks / sec
    int interval = 0;
    

  // ALERTS
    bool dead;  // Battery pct
    bool slow;  // Program speed
    bool hot;   // Flywheel temp

  // Clear autonomous information
    Brain.Timer.clear(); 
    Controller1.Screen.clearScreen();

  // Configure Controller HUD
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("104 Seconds left");
    Controller1.Screen.setCursor(3, 1);
    Controller1.Screen.print("FWD Drive");
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print("Speed Mode");
  // Configure Brain HUD
    Brain.Screen.clearLine(3);
    Brain.Screen.setCursor(3,1);
    Brain.Screen.print("---MOTORS---");

  // Configure motors
    fly1.setVelocity(100, pct);
    fly1.setMaxTorque(100, pct);
    fly1.setStopping(coast);
    radialArm.setVelocity(100, pct);
    radialArm.setMaxTorque(100, pct);
    radialArm.setStopping(hold);


 
  while (1) {

    // Secondly
    timeLeft = 105 - Brain.Timer.value();

    // Flywheel

      if(Controller1.ButtonR2.pressing()){
        fly1.spin(fwd, 12, volt);
        if(fly1.velocity(pct) <= 60){
            Controller1.rumble(".");
          }
      } else {
        fly1.stop(coast);
      }

    // Every second

    if (timeLeft == nextTime){

      // Controller

        // Timer
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.clearLine(1);
          Controller1.Screen.print(timeLeft);
          Controller1.Screen.print(" seconds left");

        // Alerts

          // TPS low
          if (tickTime < 20 && !slow) {
            Controller1.Screen.clearLine(2);
            Controller1.Screen.setCursor(2,1);
            Controller1.Screen.print("SLOW PROGRAM");
            slow = true;
          } else if (tickTime >= 20){
            slow = false;
            Controller1.Screen.clearLine(2);
            Controller1.Screen.setCursor(2,1);
          }

          // Battery dead
          if (Brain.Battery.capacity() < 25 && !dead) {
            Controller1.Screen.clearLine(2);
            Controller1.Screen.setCursor(2,1);
            Controller1.Screen.print("REPLACE BATTERY");
            dead = true;
          } else {
            dead = false;
          }

          // Flywheel overheated
          if (fly1.temperature() >= 70){
            Controller1.Screen.clearLine(2);
            Controller1.Screen.setCursor(2,1);
            Controller1.Screen.print("FLYWHEEL: ");
            Controller1.Screen.print(int(fly1.temperature()));
            hot = true;
          } else {
            hot = false;
          }

          // Endgame
          if (Brain.Timer.time(sec) >= 80 && !endGame) {
            Controller1.rumble("----");
            endGame = true;
          }

      // Brain

        // Ticks per second
        tickTime = ticks / 1;
        Brain.Screen.clearLine(1);
        Brain.Screen.setCursor(1,1);
        Brain.Screen.print("Ticks / sec: ");
        Brain.Screen.print(tickTime);
        ticks = 0;

        // Battery percent
        Brain.Screen.setCursor(2,1);
        Brain.Screen.clearLine(2);
        Brain.Screen.print("Battery percent: ");
        Brain.Screen.print(Brain.Battery.capacity());

    } // End secondly

        // Flywheel

        Brain.Screen.clearLine(4);
        Brain.Screen.setCursor(4, 1);
        Brain.Screen.print("Flywheel: ");
        Brain.Screen.setCursor(4, 11);
        Brain.Screen.print(int(fly1.velocity(pct)));
        Brain.Screen.setCursor(4, 15);
        Brain.Screen.print(int(fly1.temperature()));


    // Variable update

    ticks++;
    nextTime = timeLeft - 1;

    interval = (ticks / 1) * 0.75;

    wait(20, msec);
  } // End program
}


int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  driving();

  pre_auton();

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