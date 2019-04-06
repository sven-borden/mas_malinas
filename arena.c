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
#include <main.h>
#include <chprintf.h>
#include <motors.h>
#include <audio/microphone.h>

#include <audio_processing.h>
#include <fft.h>
#include <communications.h>
#include <arm_math.h>
#include <sensors/VL53L0X/VL53L0X.h>
#define ANGLE_MAX 360
#define ANGLE_resolution 18
#define number_of_measure 20
#define TOO_CLOSE_OF_THE_WALL 2 //il me semble que les distances sont en centimètres dans motor

static VL53L0X_Dev_t* device;
BUFFER_NAME_t name = 0;

void init_arena(void){
	//inits the sensor
	VL53L0X_init(device);
}

void gotoarenacenter(void){
	gotoedge();
	//turnleft(135);
	//goforward(false, 250);

}
bool wasteinsight(int16_t angle){

}
void searchwaste(void){

	static uint8_t state = 0;
	int16_t angle = 0;
	switch(state){
	        	case 0:
	        	for(angle = 0; angle < number_of_measure; angle++){
	        		if (wasteinsight(angle) == 1){
	        			state = 1;
	        			break;
	        		}
	        	}
	        	case 1: pickupwaste();
	        			state = 2;

	        	case 2: gohome();

	        	case 3: //selon la fréquence écoutée, goback(dist1,dist2 ou dist3)
	        			turnleft(90);
	        			throwwaste();
	        			state = 4;

	        	case 4: gotoarenacenter();
	        			state = 0;
	        }
}


int16_t findwall(void){

	uint16_t distance[number_of_measure];
	int16_t max_norm_index = -1;
	int16_t max_norm = 0;
	int16_t i = 0;
	//measure all the distances from 0° to 360°
	for(i = 0; i < number_of_measure; i++)
	{
		VL53L0X_startMeasure(device, VL53L0X_DEVICEMODE_SINGLE_RANGING);
		VL53L0X_getLastMeasure(device);
		distance[i] = device->Data.LastRangeMeasure.RangeMilliMeter;
		turnleft(ANGLE_resolution);
	}
	for(i = 0; i < number_of_measure; i++)
	{
			//search for the lowest distance
				if(distance[i] < max_norm){
					max_norm = distance[i];
					max_norm_index = i;
					}
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
void goforward(bool pid_or_not, float distance){
	if(pid_or_not == false){

		if(distance == 0)
		{
			VL53L0X_startMeasure(device, VL53L0X_DEVICEMODE_SINGLE_RANGING);
			while(device->Data.LastRangeMeasure.RangeMilliMeter > TOO_CLOSE_OF_THE_WALL)
				{
				VL53L0X_getLastMeasure(device);
				motor_set_speed(5, 5);
				}

		}
		//else motor_set_position(distance,distance,5,5);




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


}

void pickupwaste(void){
	goforward(false, 0);
	goback(5);
	shoveldown();
	goforward(false, 1);
	shovelup();
}
void goback(float distance){

	//motor_set_position(distance,distance,5,5);

}
void shoveldown(void){


}
void shovelup(void){


}
void gohome(void){
	int16_t *data;
	gotoedge();
	while(1){
		data = get_audio_buffer_ptr(name);
		processAudioData(data, 1024); //numsample = 1024
			//if freq = 1,2 ou 3 -> state = 3, break;
		goforward(true, 0);
		turnleft(90);
		}
}
void throwwaste(void){


}
