using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor fly1;
extern smartdrive Drivetrain;
extern rotation armPos;
extern motor miniArm;
extern motor_group radialArm;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );