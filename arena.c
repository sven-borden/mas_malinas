/*
 * arena.c
 *
 *  Created on: 4 Apr 2019
 *      Author: astridhochart
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <arena.h>
#include <chprintf.h>
#include "motors_advanced.h"
#include <audio/microphone.h>

#include <audio_processing.h>
#include <fft.h>
#include <communications.h>
#include <arm_math.h>
#include "sensors/VL53L0X/VL53L0X.h"
#define ANGLE_MAX 360
#define NUMBER_OF_MEASURE 20
#define ANGLE_RESOLUTION ANGLE_MAX / NUMBER_OF_MEASURE
#define PI                  3.1415926536f
#define WHEEL_DISTANCE      5.35f    //cm TO ADJUST IF NECESSARY. NOT ALL THE E-PUCK2 HAVE EXACTLY THE SAME WHEEL DISTANCE
#define PERIMETER_EPUCK     (PI * WHEEL_DISTANCE)

#define TOO_CLOSE_OF_THE_WALL 2

BUFFER_NAME_t name = 0;

void gohome(void);

void init_arena(void){
	//inits the sensor
	VL53L0X_start();
	motors_advanced_init();
	chThdSleepMilliseconds(200);
}

void gotoarenacenter(void){
	gotoedge();
	//turnleft(135);
	//goforward(false, 250);

}
uint8_t wasteinsight(int16_t angle){
	return 0;
}
void searchwaste(void){

	static uint8_t state = 0;
	int16_t angle = 0;
	switch(state){
	        	case 0:
	        	for(angle = 0; angle < NUMBER_OF_MEASURE; angle++){
	        		if (wasteinsight(angle) == 1){
	        			state = 1;
	        			break;
	        		}
	        	}
	        	case 1: pickupwaste();
	        			state = 2;

	        	case 2: gohome();

	        	case 3: //goback(dist1,dist2 ou dist3)
	        			turnleft(90);
	        			throwwaste();
	        			state = 4;

	        	case 4: gotoarenacenter();
	        			state = 0;
	        }
}


int16_t findwall(void){
	uint8_t max_norm_index = -1;
	uint16_t max_norm = 200;
	uint16_t tmp = 0;
	//measure all the distances from 0° to 360°
	for(uint16_t i = 0; i < NUMBER_OF_MEASURE; i++)
	{
		tmp = VL53L0X_get_dist_mm();
		if(tmp < max_norm){
			max_norm = tmp;
			max_norm_index = i;
		}
		turnleft(ANGLE_RESOLUTION);
	}
	return max_norm_index;
}


void gotowall(void){
	int16_t angle_min = 0;
	angle_min = findwall();
	aligntothewall(angle_min);
	goforward(false,0);

}
void aligntothewall(int16_t angle_min){
	turnleft(angle_min);
}
void goforward(uint8_t pid_or_not, float distance){
	if(pid_or_not == false){

		if(distance == 0)
		{
			// VL53L0X_startMeasure(device, VL53L0X_DEVICEMODE_SINGLE_RANGING);
			// while(device->Data.LastRangeMeasure.RangeMilliMeter > TOO_CLOSE_OF_THE_WALL)
			// {
			// 	VL53L0X_getLastMeasure(device);
			// 	motors_advanced_set_speed(5, 5);
			// }

		}
		//else motors_advanced_set_position(distance,distance,5,5);




	}

}
void gotoedge(void){
	gotowall();
	walltoright();
	//goforward(true, 0);

}

void walltoright(void){


}
void turnleft(int16_t angle){
	float corrected_angle = (float)angle/ANGLE_MAX*PERIMETER_EPUCK;
	motors_advanced_set_position(corrected_angle, corrected_angle, -5, 5);
}

void pickupwaste(void){
	goforward(false, 0);
	goback(5);
	shoveldown();
	goforward(false, 1);
	shovelup();
}
void goback(float distance){

	//motors_advanced_set_position(distance,distance,5,5);

}
void shoveldown(void){


}
void shovelup(void){


}
void gohome(void){
	int16_t *data;
	gotoedge();
	while(1){
		//data = get_audio_buffer_ptr(name);
		//processAudioData(data, 1024); //numsample = 1024
			//if freq = 1,2 ou 3 -> state = 3, break;
		//goforward(true, 0);
		//turnleft(90);
		}
}
void throwwaste(void){


}