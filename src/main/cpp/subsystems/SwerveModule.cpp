#include "subsystems/SwerveModule.h"

#include "cmath"

SwerveMod::SwerveMod(int drivePort,int steerPort,int encPort) //Constructor for Swerve Module
 :
 driveM{drivePort}, //Driving Motor port setting
 steerM{steerPort, rev::CANSparkMaxLowLevel::MotorType::kBrushless}, //Steering Motor port setting
 steerEnc{encPort} //Steering Encoder port Setting
 {}

double SwerveMod::getSteer(swerveConstants::turnUnit unit){ 
//Function to get the Steering angle of the Swerve Module in radians or degrees
 double steerAngle = 0; //create a variable for the steering angle          
 if(unit == swerveConstants::turnUnit::degrees){
    steerAngle = (steerEnc.GetPosition() * swerveConstants::kDegreesRatio); 
 //Multiplies the raw encoder value by a constant to change the value to degrees
 }else if(unit == swerveConstants::turnUnit::radians){
    steerAngle = (steerEnc.GetPosition() * swerveConstants::kRadiansRatio);
 //Multiplies the raw encoder value by a constant to change the value to radians
 }return(steerAngle + startSteer); //Returns the Position of the Steering angle of the Swerve Module
}

double SwerveMod::getCloseAng(int angI, int angF){ //Gets the closest angle to the Swerve Module based on the Initial and Final
 double ang = (angF % 360) - (angI % 360); //Uses the Modulos operator to find the closest angle
 if(fabs(ang) > 180.0){ //converting the -360 to 360 and -180 to 180
   ang = (((ang > 0) - (ang < 0)) * 360) + ang; //Finds if the angle is negative or positive  
 }return(ang);//Returns the closest angle to the two given angles Initial and Finals
}

double SwerveMod::getDrive(){ //Funtion to get the Driving Motors encoder ticks
 return((driveM.GetSelectedSensorPosition()) * swerveConstants::kMetersRatio); 
 //Multiplies the raw ticks of the Driving motor by a constant to get the Meters Driven
}

void SwerveMod::setSteer(double angle){ //sets the steering angle of the Swerve module in degrees
 double desiredAng; //Creates the varible for Desired Angle 
 double currentAng = getSteer(swerveConstants::turnUnit::degrees);
 //Gets the Current angle of the Steering encoder in degrees
 double lCloseAng = (getCloseAng(currentAng, angle));
 //Gets the closest angle based on the current angle and the desired angle on the Left side
 double rCloseAng = (getCloseAng(currentAng, angle + 180.0));
 //Gets the closest angle based on the current angle anf the desired angle on the Right side
 if(fabs(lCloseAng) <= fabs(rCloseAng)){ //If the angle is closer to the left side of the Swerve module
  desiredAng = lCloseAng; //set the desired angle to the left side angle
  steerM.Set(0.4); //set the Steering motor of the Swerve module to turn left
 }else if(fabs(rCloseAng) <= fabs(lCloseAng)){ //If the angle is closer to the right side of the Swerve Module 
  desiredAng = rCloseAng; //set the desired angle to the right side angle
  steerM.Set(-0.4); //set the Steering motor of the Swerve module to turn right
 }if(fabs(desiredAng - currentAng) <= 0.5){ //Checks of the Swerve Module's Steering angle is at the Desired Angle
  steerM.Set(0); //Stops the Swerve Module Steering Motor by setting Voltage power to 0
  steerM.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);  //Sets the Swerve Module Steering Motor to Coast
 }
}

void SwerveMod::setDrive(double pwr){ //Set the Driving Motor to a power
   driveM.Set(ctre::phoenix::motorcontrol::TalonFXControlMode::PercentOutput, pwr);
   //Setting Driving Motor to a power percentage based within (-1.0 to 1.0)
}