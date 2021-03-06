  #pragma config(Sensor, dgtl1,  rightencoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftencoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  compass,        sensorQuadEncoder)
#pragma config(Motor,  port1,           in1,           tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           FrontRight,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           RearRight,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           in2,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           in3,           tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           clawR,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           clawL,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           FrontLeft,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           RearLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          in4,           tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int requestedLeftSpeed 	= 0;
int requestedRightSpeed = 0;
short speedIncrement 		= 1;
short stallSpeed 				= 20;
int maxSpeed 						= 30;
int distance						= 360*10;
int time_dur1					 	= 2000;
int time_dur2				 		= 1000;
int pwr 								= 120;
////////////////////////////////
/*
task autodrive(){
int a = pushed(0);
while (true){
if (pushed(0) == 1){
if (REn < distance || LEn < distance){
REn = abs(SensorValue[rightencoder]);
LEn = abs(SensorValue[leftencoder]);
motor[FrontRight] = Pwr;
motor[RearRight] = Pwr;
motor[FrontLeft] = Pwr;
motor[RearLeft] = Pwr;
if (REn > (distance - slow_at) || LEn > (distance - slow_at) ){
if (Pwr < 20){
Pwr = 20;
}else{
Pwr = Pwr - 1;
}
}
}else{
motor[FrontRight] = 0;
motor[RearRight] = 0;
motor[FrontLeft] = 0;
motor[RearLeft] = 0;
}
}
}
}
*/
//////////////////////////////////////////
/*
task auto(){
clearTimer(T1);
clearTimer(T2);
while	(time1[T1] < time_dur){
task intake;
}
while(time[T1] < time_dur2){


}
*/

//////////////////////////////////////
int isDecelerating (int requested, int current){
	if (abs(requested)< abs(current) && (requested * current) > 0 || (requested * current) < 0)
	{
		return 1;
		}else{
		return 0;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
int max (short x , short y )
{
	return x < y ? y : x;
}
//////////////////////////////////////////////////////////////////////////////////////////
task intake(){
	while (true){
		if (vexRT[Btn6U] == 1){
			motor[in1] = 127;
			motor[in2]= 127;
			motor[in3] = 127;
			motor[in4] = 127;
			}else if (vexRT[Btn6D] == 1){
			motor[in1] = -127;
			motor[in2]= -127;
			motor[in3] = -127;
			motor[in4] = -127;
			}else{
			motor[in1] = 0;
			motor[in2]= 0;
			motor[in3] = 0;
			motor[in4] = 0;
		}
	}
}
/////////////////////////////////////////
task claw(){
	while (true){
		/*
		if (vexRT[Btn5U] == 1){
		motor[clawR] = 127;
		motor[clawL] = 127;
		}else if (vexRT[Btn5D] == 1){
		motor[clawR] = -127;
		motor[clawL] = -127;
		}else{
		motor[clawL] = 0;
		motor[clawR]= 0;

		}
		*/
		motor[clawL] = vexRT[Ch2];
		motor[clawR] = vexRT[Ch2];
	}

}
//////////////////////////////////////////////////////////////////////////////////////////
task accelerator()
{
	int currentRightSpeed 	= 0;
	int currentLeftSpeed 	= 0;
	while(true){

		if (requestedLeftSpeed != currentLeftSpeed)
		{


			short Increment = speedIncrement;

			if (isDecelerating (requestedLeftSpeed, currentLeftSpeed) == 1){
				Increment = 20;

			}


			//Eliminate overshoot
			else if (abs(requestedLeftSpeed - currentLeftSpeed) < Increment){
				Increment = abs(requestedLeftSpeed - currentLeftSpeed);
			}

			if (requestedLeftSpeed > currentLeftSpeed) {
				currentLeftSpeed = currentLeftSpeed + Increment;
				}else {
				currentLeftSpeed = currentLeftSpeed - Increment;
			}
			motor[FrontLeft] = -currentLeftSpeed;
			//	motor[MiddleLeft]= -currentLeftSpeed;
			motor[RearLeft] = -currentLeftSpeed;

		}
		if (requestedRightSpeed != currentRightSpeed)
		{


			short Increment = speedIncrement;

			if (isDecelerating (requestedRightSpeed, currentRightSpeed) == 1){
				Increment = 15;
			}

			else if (abs(requestedRightSpeed - currentRightSpeed) < Increment) {
				Increment = abs(requestedRightSpeed - currentRightSpeed);
			}

			if (requestedRightSpeed > currentRightSpeed) {
				currentRightSpeed = currentRightSpeed + Increment;
				}else {
				currentRightSpeed = currentRightSpeed - Increment;
			}
			motor[FrontRight] = currentRightSpeed;
			//	motor[MiddleRight]= currentRightSpeed;
			motor[RearRight] = currentRightSpeed;

		}
		wait1Msec(10);

	}
}
//////////////////////////////////////////////////////////////////////////////////////////
task joystickMonitor()
{
	while(true)
	{
		if (vexRT[Btn5D] ==1)
		{
			maxSpeed 		= 30;
			speedIncrement	= 1;
		}
		else
		{
			maxSpeed		= 127;
			speedIncrement	 = 5;
		}


		float scale 		= (float)maxSpeed / 127.0;
		requestedLeftSpeed 	= (short)(float)((-1 * vexRT[Ch4]) - vexRT[Ch3])*scale;
		requestedRightSpeed	= (short)(float)((-1 * vexRT[Ch4]) + vexRT[Ch3])*scale;
		short maxLR;
		float f;



		if (abs(requestedLeftSpeed)< stallSpeed *scale)
		{
			requestedLeftSpeed 	= 0;
			speedIncrement		= 20;
		}
		if (abs(requestedRightSpeed)< stallSpeed *scale)
		{
			requestedRightSpeed = 0;
			speedIncrement 		= 20;
		}
		maxLR = max( abs (requestedLeftSpeed), abs (requestedRightSpeed));

		if (maxLR > maxSpeed)
		{
			if(abs(requestedLeftSpeed) > abs (requestedRightSpeed))
				f = abs((float)maxSpeed/requestedLeftSpeed);
			else
				f = abs((float)maxSpeed/requestedRightSpeed);
			requestedLeftSpeed = requestedLeftSpeed * f;
			requestedRightSpeed = requestedRightSpeed * f;
		}
		wait1Msec(10);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////

task main(){

	startTask(accelerator);
	startTask (joystickMonitor);
	startTask(intake);
	startTask(claw);
	while(true){
		wait1Msec(10);
	}

}
