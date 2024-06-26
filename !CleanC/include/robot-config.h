using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor frontL;
extern motor backL;
extern motor frontR;
extern motor backR;
extern motor fly1;
extern motor radial1;
extern motor radial2;
extern inertial inert;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );