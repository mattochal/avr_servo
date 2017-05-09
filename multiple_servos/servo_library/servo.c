/* servo.c

    Copyright Mateusz Ochal (mo4g15@soton.ac.uk)
    University of Southampton


This file shows an example of controlling a standard servo motors using PWM from AVR
chip AT90USB1286.

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "servo.h"

#ifdef SERVO_DEBUG
#include "printf.h"
#endif

typedef struct servo {
	uint8_t position;		/* as proportion of the angle */
	uint8_t pin;			/* pin number on the SERVO_PORT */
} servo;

servo servos[MAX_SERVOS];
volatile int8_t servoNum = 0;
volatile int8_t current = -1;  /* current servo to update, if -1 there are no servos to update */

void add_servo(uint8_t pin, uint8_t position) {
	int s;

	s = servoNum;

	if (s >= MAX_SERVOS) {
		s = -1;
	} else {
		SERVO_PORT_DDR |=  _BV(pin); /* configue pin as output */
		SERVO_PORT &= ~_BV(pin); /* set low */
		servos[s].pin = pin;
		set_position(servoNum, position);
		servos[s].position = position;
		servoNum = s + 1; /* New task fully initialized */
	}
}

void set_position(uint8_t n, uint8_t pos) { /* servo number and position to go to */
	uint8_t p;
	if (n < servoNum) {
		p = MIN_SERVO_OCR + (uint8_t)(pos * (MAX_SERVO_OCR - MIN_SERVO_OCR) / 255.0);
		servos[n].position = p;
	}
}

void next(){
	cli();
	current += 1;
	if (current >= servoNum){
		current = 0;
	}
	sei();
}

void servo_pulse_on() {
	cli();
	SERVO_PORT |= _BV(servos[current].pin);  /* SERVO on */
	SERVO_OCR = servos[current].position; /* assuming F_CPU/64 */
	sei();
}

void servo_pulse_off(){
	cli();
	SERVO_PORT &= ~_BV(servos[current].pin); /* pulse off */
	sei();
}


