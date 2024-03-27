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
// fly1                 motor         12              
// Drivetrain           drivetrain    20, 9, 13, 8, 19
// armPos               rotation      14              
// miniArm              motor         16              
// radialArm            motor_group   5, 10           
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

competition Competition;

// Motor Groups

void pre_auton(void) {
  vexcodeInit();
}

void armTo(float pGain, int pos){ //P loop
  int error = pos - armPos.position(degrees);
  while(abs(error) >= 2){
    error = pos - armPos.position(degrees);
    radialArm.setVelocity(error * pGain, pct);
    radialArm.spin(fwd);
    wait(25, msec);
  }
  radialArm.stop(hold);
}

void wallAlign(int head){
  Drivetrain.turnToHeading(head, degrees);
  Drivetrain.setTimeout(1, seconds);
  Drivetrain.driveFor(100, inches);
  Drivetrain.setTimeout(3, seconds);
}

void autonomous(void) {
  // Motor setup
  /*
  Brain.Screen.clearScreen();
  Brain.Screen.print("Configuring motors...");
  Drivetrain.setStopping(hold); // Drivetrain
  Drivetrain.setDriveVelocity(95, pct);
  Drivetrain.setTurnVelocity(127, pct);
  Drivetrain.setTurnConstant(0.9);
  Drivetrain.setTimeout(3, seconds);

  radialArm.setStopping(hold); // radialArm
  radialArm.setVelocity(127, pct);
  radialArm.setMaxTorque(127, pct);
  radialArm.setTimeout(5, sec);

  miniArm.setVelocity(60, pct); // miniArm
  miniArm.setStopping(hold);
  miniArm.setTimeout(2, sec);

  // Actual Auton

  // One tile:
  // a = 24
  // b = 24
  // c = 34

  // Positioned at 147 degrees

  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print("Scoring in net...");
  armTo(1, 115); // Preload
  Drivetrain.driveFor(reverse, 35, inches);
  Drivetrain.turnToHeading(180, degrees);
  Drivetrain.driveFor(fwd, 5, inches);
  armTo(1, 0);

  Brain.Screen.setCursor(3, 1);
  Brain.Screen.print("Getting match load..."); // Match load
  armTo(1, 0);
  miniArm.setPosition(0,degrees);
  miniArm.spinToPosition(130, degrees, false);
  Drivetrain.turnToHeading(180, degrees);
  Drivetrain.driveFor(fwd, 15, inches);
  Drivetrain.turnToHeading(200, degrees);
  Drivetrain.driveFor(fwd, 2, inches);
  Drivetrain.turnToRotation(-90, degrees);
  miniArm.spinToPosition(0, degrees);

  Brain.Screen.setCursor(4, 1);
  Brain.Screen.print("Touching elevation bar...");
  Drivetrain.turnToHeading(90, degrees);  // Elevation Bar
  Drivetrain.driveFor(forward, 17, inches);
  wallAlign(180);
  Drivetrain.driveFor(reverse, 2, inches);
  Drivetrain.turnToHeading(275, degrees);
  Drivetrain.driveFor(reverse, 30, inches);
  armTo(0.7, 100);
  Drivetrain.setTimeout(2, seconds);
  Drivetrain.setDriveVelocity(50, percent);
  Drivetrain.driveFor(reverse, 5, inches);


  Brain.Screen.setCursor(6, 1);
  Brain.Screen.print("Done!");
  */

  /*Drivetrain.setDriveVelocity(80, pct);
  Drivetrain.setTurnVelocity(80, pct);
  Drivetrain.driveFor(fwd, 35, inches);
  Drivetrain.driveFor(reverse, 17, inches);
  */
  // What I added:

  fly1.setVelocity(127, pct);
  fly1.setMaxTorque(127, pct);
  fly1.setStopping(coast);

  radialArm.setStopping(hold); // radialArm
  radialArm.setVelocity(127, pct);
  radialArm.setMaxTorque(127, pct);

  Drivetrain.setDriveVelocity(100, pct);

  Drivetrain.driveFor(fwd, 24, inches);
  Drivetrain.turnToHeading(225, degrees);
  Drivetrain.driveFor(fwd, 20, inches);
  armTo(1, 100);
  Drivetrain.driveFor(2, inches);
  fly1.spin(fwd, 12, volt);
}

void usercontrol(void) {
  Brain.Screen.clearScreen();

  // DECLARATION OF VARIABLES
    int nextTime = 0;   // Used to run secondly code
    int timeLeft = 0;   // Used for controller timer & secondly code
      bool endGame = false;
    int ticks = 0;      // Used to check speed of program
    int tickTime = 0;   // Ticks / sec
    // bool teamPick = false;
      // bool redTeam = false;
      bool debug = false;
      // int currentSponsor = 0;
      bool drawn = false;
    bool flying = false;
      int inputTime = 0;
    // int inputTime = 0;
    Drivetrain.setStopping(hold);

  // Clear autonomous information
    Brain.Timer.clear(); 
    Controller1.Screen.clearScreen();

  // Configure Controller HUD
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("104 Seconds left");
  // Configure Brain HUD

  // Configure motors
    fly1.setVelocity(127, pct);
    fly1.setMaxTorque(127, pct);
    fly1.setStopping(coast);
    radialArm.setVelocity(127, pct);
    radialArm.setMaxTorque(127, pct);
    radialArm.setStopping(hold);
    miniArm.setVelocity(50, pct);
    miniArm.setMaxTorque(127, pct);
    miniArm.setStopping(hold);

  while (1) {

    timeLeft = 105 - Brain.Timer.value();

    // Flywheel
      if(Controller1.ButtonR1.pressing() && inputTime >= 20){
        if(flying){
          flying = false;
          Controller1.Screen.clearLine(3);
        } else{
          flying = true;
          Controller1.Screen.setCursor(3, 1);
          Controller1.Screen.print("Flywheel Locked");
        }
        inputTime = 0;
      }

      if(Brain.Timer.value() <= 2 && Controller1.ButtonRight.pressing()){
        debug = true;
      }

      if(flying || Controller1.ButtonR2.pressing()){
        fly1.spin(fwd, 12, volt);
        if(fly1.velocity(pct) <= 60){
            Controller1.rumble(".");
          }
      } else{
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

        // Ticks
        if(debug){
          Controller1.Screen.setCursor(2,1);
          Controller1.Screen.clearLine(2);
          Controller1.Screen.print(tickTime);
          Controller1.Screen.print(" TpS -- Battery: ");
          Controller1.Screen.print(int(Brain.Battery.capacity()));
        }

        // Arm
          if(!flying){
          Controller1.Screen.clearLine(3);
          Controller1.Screen.setCursor(3,1);
          Controller1.Screen.print("Arm: ");
          Controller1.Screen.print(int(armPos.angle(deg)));
          Controller1.Screen.setCursor(3, 9);
          Controller1.Screen.print("Target: 87");
          }

        // Endgame
          if (timeLeft <= 15 && !endGame) {
            Controller1.rumble("----");
            endGame = true;
          }

      // Brain

      if(debug){
        // Ticks per second
        Brain.Screen.clearLine(1);
        Brain.Screen.setCursor(3,1);
        Brain.Screen.print("Ticks / sec: ");
        Brain.Screen.print(tickTime);
        // Battery percent
        Brain.Screen.setCursor(2,1);
        Brain.Screen.clearLine(2);
        Brain.Screen.print("Battery percent: ");
        Brain.Screen.print(Brain.Battery.capacity());
      } else if(!drawn){
        Brain.Screen.drawImageFromFile("van-griffith.png", 0, 0);
      }

    } // End secondly

    if(debug){

        // Heading

        Brain.Screen.clearLine(1);
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Heading:");
        Brain.Screen.print(int(Drivetrain.heading(degrees)));


        // Flywheel

        Brain.Screen.clearLine(4);
        Brain.Screen.setCursor(4, 1);
        Brain.Screen.print("Flywheel: ");
        Brain.Screen.setCursor(4, 11);
        Brain.Screen.print(int(fly1.velocity(pct)));
        Brain.Screen.setCursor(4, 15);
        Brain.Screen.print(int(fly1.temperature(pct)));
    }


    // Variable update

    tickTime = ticks / Brain.Timer.value();
    ticks++;
    inputTime ++;
    nextTime = timeLeft - 1;

    wait(20, msec); 
  } // End program
}


int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

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