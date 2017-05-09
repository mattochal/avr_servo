/* COMP2215 15/16 Task 5---MODEL ANSWER */

#include "os.h"
#include <avr/io.h>
#include <util/delay.h>
#include "printf.h"
#include "servo.h"

void servo_init();
int collect_delta(int);
int check_switches(int);
int set_servos(int);

#define SERVO_NUM 3
volatile uint8_t servo_pos[SERVO_NUM];
volatile uint8_t demo = 0; /* 1 indicates demo mode*/
volatile int8_t servo_dir[SERVO_NUM];

void main(void) {
    os_init();

    servo_init();

    os_add_task( collect_delta,   5, 1);
    os_add_task( check_switches,  15, 1);
    os_add_task( set_servos,	  30, 1); 
     
    sei();
    for(;;){}
    
}

void servo_init(){
	servo_pos[0] = 50;
	servo_pos[1] = 200;
	servo_pos[2] = 100;
	servo_dir[0] = 1;
	servo_dir[1] = -1;
	servo_dir[2] = 1;
	add_servo(PD0, servo_pos[0]);
	add_servo(PD1, servo_pos[1]);
	add_servo(PD2, servo_pos[2]);
}

void increase(int s, int8_t step){
	int16_t p;
	p = servo_pos[s] + step*4;
	if (p > 255) {
		servo_pos[s] = 255;
	} else if (p < 0) {
		servo_pos[s] = 0;
	} else {
		servo_pos[s] = p;
	}
}

int collect_delta(int state) {
	if (!demo) {
		increase(0, os_enc_delta());
	}
	return state;
}

int set_servos(int state) {
	uint8_t i;
	for (i = 0; i < SERVO_NUM; ++i){
		set_position(i, servo_pos[i]);
	}
	return state;
}

void print_positions(){
	uint8_t i;
	printf("servo position: ");
	for (i = 0; i < SERVO_NUM; ++i){
		printf("%d, ", servo_pos[i]);
	}
	printf("\n");
}

void demo_toggle(){
	if (demo){
		display_string("Demo mode off!\n");
		demo = 0;
	} else {
		display_string("Demo mode on!\n");
		demo = 1;
	}
}

int check_switches(int state) {
	if (!demo){
		if (get_switch_press(_BV(SWN))) {
			increase(1, 1);
		}

		if (get_switch_rpt(SWN)) {
			increase(1, 1);
		}

		if (get_switch_press(_BV(SWE))) {
			increase(2, 1);
		}

		if (get_switch_rpt(_BV(SWE))) {
			increase(2, 1);
		}
			
		if (get_switch_press(_BV(SWS))) {
			increase(1, -1);
		}

		if (get_switch_rpt(_BV(SWS))) {
			increase(1, -1);
		}
			
		if (get_switch_press(_BV(SWW))) {
			increase(2, -1);
		}

		if (get_switch_rpt(_BV(SWW))) {
			increase(2, -1);
		}
	} else {
		uint8_t i;

		for (i = 0; i < SERVO_NUM; ++i){
			increase(i, servo_dir[i]);
			if (servo_pos[i] == 255){
				servo_dir[i] = -1;
			} else if (servo_pos[i] == 0){
				servo_dir[i] = 1;
			}
		}
	}
		
	if (get_switch_long(_BV(SWC))) {
		demo_toggle();
	} else if (get_switch_press(_BV(SWC))) {
		demo_toggle();
	}

	print_positions();
	return state;	
}