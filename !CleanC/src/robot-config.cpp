#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor frontL = motor(PORT20, ratio18_1, false);
motor backL = motor(PORT9, ratio18_1, false);
motor frontR = motor(PORT13, ratio18_1, true);
motor backR = motor(PORT8, ratio18_1, true);
motor fly1 = motor(PORT12, ratio6_1, false);
motor radial1 = motor(PORT5, ratio36_1, true);
motor radial2 = motor(PORT10, ratio36_1, false);
inertial inert = inertial(PORT19);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}