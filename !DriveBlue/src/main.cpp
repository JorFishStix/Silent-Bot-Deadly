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
// radial1              motor         5               
// radial2              motor         10              
// Drivetrain           drivetrain    20, 9, 13, 8, 19
// armPos               rotation      14              
// miniArm              motor         16              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

competition Competition;

// Motor Groups
motor_group radialArm = motor_group(radial1, radial2);

void pre_auton(void) {
  vexcodeInit();
}

void armTo(float pGain, float iGain, int pos){ //PI loop
  int error = pos - armPos.position(degrees);
  int errorSum = 0;
  while(abs(error) >= 5){
    error = pos - armPos.position(degrees);
    errorSum += error;
    radialArm.setVelocity((error * pGain + errorSum * iGain), pct);
    radialArm.spin(fwd);
  }
}

void wallAlign(int head){
  Drivetrain.turnToHeading(head, degrees);
  Drivetrain.setTimeout(1, seconds);
  Drivetrain.driveFor(100, inches);
  Drivetrain.setTimeout(3, seconds);
}

void autonomous(void) {
  // Motor setup
  Drivetrain.setStopping(hold); // Drivetrain
  Drivetrain.setDriveVelocity(60, pct);
  Drivetrain.setTurnVelocity(60, pct);
  Drivetrain.setTurnConstant(0.6);

  radialArm.setStopping(hold); // radialArm
  radialArm.setVelocity(127, pct);
  radialArm.setMaxTorque(127, pct);
  radialArm.setTimeout(5, sec);

  miniArm.setVelocity(60, pct); // miniArm
  miniArm.setStopping(hold);
  miniArm.setTimeout(2, sec);

  // Actual Auton
  
  armTo(0.6, 0.2, 90); // Preload
  Drivetrain.driveFor(reverse, 24, inches);
  Drivetrain.driveFor(fwd, 4, inches);


  wallAlign(240); // Match load
  Drivetrain.driveFor(reverse, 1, inches);
  Drivetrain.turnToHeading(180, degrees);
  armTo(0.6, 0.2, 0);
  miniArm.spinToPosition(270, degrees);
  Drivetrain.driveFor(fwd, 6, inches);
  Drivetrain.turnToRotation(-90, degrees);
  miniArm.spinToPosition(0, degrees);


  Drivetrain.driveFor(fwd, 12, inches); // Elevation Bar
  wallAlign(180);
  Drivetrain.driveFor(reverse, 4, inches);
  Drivetrain.turnToHeading(240, degrees);
  armTo(1, 0.5, 90);
  Drivetrain.driveFor(reverse, 24, inches);
}


void usercontrol(void) {

  // DECLARATION OF VARIABLES
    int nextTime = 0;   // Used to run secondly code
    int timeLeft = 0;   // Used for controller timer & secondly code
      bool endGame = false;
    int ticks = 0;      // Used to check speed of program
    int tickTime = 0;   // Ticks / sec
    int currentSponsor = 0;
      int maxSponsor = 4;
      int nextSwitch = 5;
      bool sponsorDisplay = true;
      bool sponsorWait = false;
    Drivetrain.setStopping(hold);

  // Clear autonomous information
    Brain.Timer.clear(); 
    Controller1.Screen.clearScreen();

  // Configure Controller HUD
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("104 Seconds left");
  // Configure Brain HUD
    Brain.Screen.clearLine(3);
    Brain.Screen.setCursor(3,1);
    Brain.Screen.print("---MOTORS---");

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

    if(Controller1.ButtonRight.pressing() && !sponsorWait && !sponsorDisplay){
      sponsorDisplay = true;
    } else if(Controller1.ButtonRight.pressing() && !sponsorWait && sponsorDisplay){
      sponsorDisplay = false;
    }

    // Every second

    if (timeLeft == nextTime){

      sponsorWait = false;

      // Controller

        // Timer
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.clearLine(1);
          Controller1.Screen.print(timeLeft);
          Controller1.Screen.print(" seconds left");
        
        // Flywheel
          Controller1.Screen.setCursor(3,1);
          Controller1.Screen.clearLine(3);
          Controller1.Screen.print("FLYWHEEL: ");
          Controller1.Screen.print(int(fly1.velocity(percent)));
          Controller1.Screen.setCursor(3, 14);
          Controller1.Screen.print(int(fly1.temperature(percent)));

        // tickTime
          Controller1.Screen.setCursor(3, 18);
          Controller1.Screen.print(tickTime);

        // Endgame
          if (timeLeft <= 15 && !endGame) {
            Controller1.rumble("----");
            endGame = true;
          }

      // Brain

      if(sponsorDisplay){
        if(int(Brain.Timer.value()) == nextSwitch){
          currentSponsor += 1;
          Brain.Screen.clearScreen();
          nextSwitch = int(Brain.Timer.value() + 5);
          if(currentSponsor > maxSponsor){
            currentSponsor = 1;
          }
        

        if(currentSponsor == 1){
            Brain.Screen.drawImageFromFile("bluePlate.png", 0, 0);
        } else if(currentSponsor == 2){
          Brain.Screen.drawImageFromFile("van-griffith.png", 0, 0);
        } else if(currentSponsor == 3){
          Brain.Screen.drawImageFromFile("taco.png", 0, 0);
        } else if(currentSponsor == 4){
          Brain.Screen.drawImageFromFile("badger.png", 0, 0);
        }
        
        }
      }


      if(!sponsorDisplay){

        // Ticks per second
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

    if(!sponsorDisplay){

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

    tickTime = ticks / Brain.Timer.value();
    ticks++;
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