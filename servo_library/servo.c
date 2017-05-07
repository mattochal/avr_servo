/* servo.c

    Copyright Mateusz Ochal (mo4g15@soton.ac.uk)
    University of Southampton


This file shows an example of controlling a standard servo motor using PWM from AVR
chip AT90USB1284.

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "servo.h"

volatile uint8_t current = 0; // current servo to update, if -1 there are no servos to update
volatile uint8_t n = 0;
volatile uint8_t servos[][2];

void os_init_servo(uint8_t arr[][2], uint8_t size){ /* number of servos */
	uint8_t i;
	uint8_t pin;
	uint8_t pos;

	n = size;
	servos = arr;

	for (i = 0; i < n; ++i){
		pin = servos[current][0];
		pos = servos[current][1];

		DDRD  |=  _BV(pin);   /* SERVO pin out */
		PORTD &= ~_BV(pin);   /* SERVO off */
	}
}

uint8_t set_position(uint8_t pin, uint8_t pos){ /* position to go to */
	servos[pin][1] = pos;
}

uint8_t update_all() {
	uint8_t i;
	uint8_t pin;
	uint8_t pos;
	uint8_t delay[];

	int timer = TCNT0;
	int end_timer = TCNT0 + F_CPU/PRESCALER;
	cli();
	for (i = 0; i < n; ++i){
		pin = servos[current][0];
		pos = servos[current][1];

		PORTD |= _BV(pin);   /* SERVO on */
		_delay_ms(1);
		_delay_us((1000 * pos)/255);
		PORTD &= ~_BV(pin);   /* SERVO off */
	}
	sei();
}



