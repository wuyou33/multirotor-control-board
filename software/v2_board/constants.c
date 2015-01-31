#include "constants.h"
#include "packets.h"

ADDRESST ADDRESS;
TELEMETRIEST TELEMETRIES;
ONOFFT ONOFF;
SETPOINTST SETPOINTS;
POSITIONST POSITIONS;
CONTROLLERST CONTROLLERS;
LANDINGT LANDING;
COMMANDST COMMANDS;
unsigned char GET_STATUS;

void constInit(){
	adr64Setter(ADDRESS.COORDINATOR,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
	adr64Setter(ADDRESS.KC1,0x00,0x13,0xA2,0x00,0x40,0xB5,0x99,0xB9);
	adr64Setter(ADDRESS.K1,0x00,0x13,0xA2,0x00,0x40,0xB5,0x99,0xBF);
	adr64Setter(ADDRESS.K2,0x00,0x13,0xA2,0x00,0x40,0xB5,0x99,0xBD);
	adr64Setter(ADDRESS.K3,0x00,0x13,0xA2,0x00,0x40,0xB5,0x99,0xB8);
	adr64Setter(ADDRESS.BROADCAST,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF);
	ADDRESS.UNKNOWN16[0] = 0xFF;
	ADDRESS.UNKNOWN16[1] = 0xFE;

	TELEMETRIES.GROUND_DISTANCE_ESTIMATED = 0x00;
	TELEMETRIES.GROUND_DISTANCE = 0x01;
	TELEMETRIES.ELEVATOR_SPEED = 0x02;
	TELEMETRIES.AILERON_SPEED = 0x03;
	TELEMETRIES.ELEVATOR_SPEED_ESTIMATED=0x04;
	TELEMETRIES.AILERON_SPEED_ESTIMATED=0x05;
	TELEMETRIES.ELEVATOR_POS_ESTIMATED=0x06;
	TELEMETRIES.AILERON_POS_ESTIMATED=0x07;
	TELEMETRIES.THROTTLE_CONTROLLER_OUTPUT=0x08;
	TELEMETRIES.THROTTLE_SPEED=0x09;
	TELEMETRIES.AILERON_CONTROLLER_OUTPUT=0x0A;
	TELEMETRIES.ELEVATOR_CONTROLLER_OUTPUT=0x0B;
	TELEMETRIES.THROTTLE_SETPOINT=0x0E;
	TELEMETRIES.ELEVATOR_POS_SETPOINT=0x0F;
	TELEMETRIES.AILERON_POS_SETPOINT=0x10;
	TELEMETRIES.ELEVATOR_VEL_SETPOINT=0x11;
	TELEMETRIES.AILERON_VEL_SETPOINT=0x12;
	TELEMETRIES.ELEVATOR_ACC=0x15;
	TELEMETRIES.AILERON_ACC=0x16;
	TELEMETRIES.VALID_GUMSTIX=0x17;
	TELEMETRIES.ELEVATOR_DESIRED_SPEED_POS_CONT=0x18;
	TELEMETRIES.AILERON_DESIRED_SPEED_POS_CONT=0x19;
	TELEMETRIES.ELE_DES_SPEED_POS_CONT_LEADER=0x1A;
	TELEMETRIES.AIL_DES_SPEED_POS_CONT_LEADER=0x1B;
	TELEMETRIES.OUTPUT_THROTTLE=0x1C;
	TELEMETRIES.OUTPUT_ELEVATOR=0x1D;
	TELEMETRIES.OUTPUT_AILERON=0x1E;
	TELEMETRIES.OUTPUT_RUDDER=0x1F;
	TELEMETRIES.BLOB_DISTANCE=0x20;
	TELEMETRIES.BLOB_HORIZONTAL=0x21;
	TELEMETRIES.BLOB_VERTICAL=0x22;
	TELEMETRIES.PITCH_ANGLE=0x23;
	TELEMETRIES.ROLL_ANGLE=0x24;
	
	ONOFF.OFF=0x00;
	ONOFF.ON=0x01;
	
	
	SETPOINTS.THROTTLE_SP=0x01;
	SETPOINTS.ELEVATOR_POSITION=0x02;
	SETPOINTS.AILERON_POSITION=0x03;
	SETPOINTS.ELEVATOR_VELOCITY=0x04;
	SETPOINTS.AILERON_VELOCITY=0x05;
	
	POSITIONS.ABSOLUT=0x01;
	POSITIONS.RELATIV=0x02;
	
	CONTROLLERS.OFF=0x01;
	CONTROLLERS.VELOCITY=0x02;
	CONTROLLERS.POSITION=0x03;
	CONTROLLERS.MPC=0x04;
	
	LANDING.ON_GROUND=0x00;
	LANDING.LANDING=0x01;
	LANDING.STABILIZATION=0x02;
	LANDING.TAKE_OFF=0x03;
	LANDING.FLIGHT=0x04;
	
	COMMANDS.TELEMETRY=0x01;
	COMMANDS.TELEMETRY_COORDINATOR=0x02;
	COMMANDS.LANDING=0x11;
	COMMANDS.SET_SETPOINTS=0x12;
	COMMANDS.CONTROLLERS=0x13;
	COMMANDS.TRAJECTORY_FOLLOW=0x15;
	COMMANDS.TRAJECTORY_POINTS=0x16;
	COMMANDS.GUMSTIX=0x17;
	COMMANDS.FOLLOWER_SET=0x18;
	COMMANDS.TIME=0x19;


	
	GET_STATUS=0x95;
	
	telemetryToCoordinatorArr[TELEMETRIES.GROUND_DISTANCE_ESTIMATED]=1;
	telemetryToCoordinatorArr[TELEMETRIES.BLOB_DISTANCE]=1;
	telemetryToCoordinatorArr[TELEMETRIES.BLOB_HORIZONTAL]=1;
	telemetryToCoordinatorArr[TELEMETRIES.BLOB_VERTICAL]=1;
	telemetryToCoordinatorArr[TELEMETRIES.VALID_GUMSTIX]=1;
}