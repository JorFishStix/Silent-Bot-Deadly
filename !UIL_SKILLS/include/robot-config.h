using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern smartdrive Drivetrain;
extern motor_group radialArm;
extern motor fly1;
extern motor fly2;
extern rotation armPos;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );