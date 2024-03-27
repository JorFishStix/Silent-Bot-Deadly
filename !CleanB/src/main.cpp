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
// radial1              motor         5               
// radial2              motor         10              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

competition Competition;

motor_group lSide = motor_group(frontL, backL);
motor_group rSide = motor_group(frontR, backR);
motor_group dTrain = motor_group(frontL, frontR, backR, backL);
motor_group radialArm = motor_group(radial1, radial2);

// Motor Groups



void pre_auton(void) {
  vexcodeInit();
}

void autonomous(void) {
  
}

int driving(){

  int inputTime = 0;
  int ticks = 0;
  int interval = 100;
  bool trans = false;
  frontL.setMaxTorque(127, pct);
  backL.setMaxTorque(127, pct);
  frontR.setMaxTorque(127, pct);
  backR.setMaxTorque(127, pct);

  while(1){

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

      // Deadzone + Transmission (Drivetrain Controls)

      if (abs(rStickH) > 5 || abs(lStickV) > 5) { // Deadzone
      
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

    inputTime ++;
    ticks ++;
    interval = ticks / Brain.Timer.value() * 0.75;
  }
  return 1;

}



void usercontrol(void) {

  // DECLARATION OF VARIABLES
    int nextTime = 0;   // Used to run secondly code
    int timeLeft = 0;   // Used for controller timer & secondly code
      bool endGame = false;
    int ticks = 0;      // Used to check speed of program
    int tickTime = 0;   // Ticks / sec
    int interval = 0;
    bool teamPick = false; // Screen color
      bool drawn = false;
    

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
  // Configure Brain HUD
    Brain.Screen.clearLine(3);
    Brain.Screen.setCursor(3,1);
    Brain.Screen.print("---MOTORS---");

  // Configure motors
    fly1.setVelocity(127, pct);
    fly1.setMaxTorque(127, pct);
    fly1.setStopping(coast);
    radialArm.setMaxTorque(127, pct);
    radialArm.setStopping(hold);
  



  while (1) {

    timeLeft = 105 - Brain.Timer.value();

    // Team Color
      if(!teamPick){
        if(Brain.Timer.value() <= 5 && Controller1.ButtonUp.pressing()){
          Brain.Screen.setFillColor(red);
          Brain.Screen.drawRectangle(0, 0, 480, 240);
          teamPick = true;
        } else if(Brain.Timer.value() <= 5 && Controller1.ButtonDown.pressing()){
          Brain.Screen.setFillColor(blue);
          Brain.Screen.drawRectangle(0, 0, 480, 240);
          teamPick = true;
        }
      } if(teamPick && !drawn){
        Brain.Screen.drawLine(239, 20, 239, 200);
      }

    // Flywheel

      if(Controller1.ButtonR2.pressing()){
        fly1.spin(fwd, 12, volt);
        if(fly1.velocity(pct) <= 60){
            Controller1.rumble(".");
          }
      } else {
        fly1.stop(coast);
      }

    // radialArm

    if(Controller1.ButtonX.pressing()){
      radialArm.spin(forward, 12, volt);
    } else if( Controller1.ButtonB.pressing()){
        radialArm.spin(reverse, 12, volt);
    } else {
        radialArm.stop(hold);
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
          } else if (tickTime >= 20 && slow){
            Controller1.Screen.clearLine(2);
            slow = false;
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
            Controller1.Screen.print("FLY HOT - ");
            Controller1.Screen.print(int(fly1.temperature()));
            hot = true;
          } else if(hot && fly1.temperature() < 70){
            Controller1.Screen.clearLine(2);
            hot = false;
          }

          // Endgame
          if (timeLeft <= 15 && !endGame) {
            Controller1.rumble("----");
            endGame = true;
          }

      // Brain

      if(!teamPick){

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
      }

    } // End secondly

    if(!teamPick){

        // Flywheel

        Brain.Screen.clearLine(4);
        Brain.Screen.setCursor(4, 1);
        Brain.Screen.print("Flywheel: ");
        Brain.Screen.setCursor(4, 11);
        Brain.Screen.print(int(fly1.velocity(pct)));
        Brain.Screen.setCursor(4, 15);
        Brain.Screen.print(int(fly1.temperature()));

        // Radial Arm

        Brain.Screen.clearLine(5);
        Brain.Screen.setCursor(5, 1);
        Brain.Screen.print("Arm 1: ");
        Brain.Screen.setCursor(5, 8);
        Brain.Screen.print(int(radial1.position(degrees)));
        Brain.Screen.setCursor(5, 12);
        Brain.Screen.print("Arm 2: ");
        Brain.Screen.setCursor(5, 20);
        Brain.Screen.print(int(radial2.position(degrees)));
    }


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