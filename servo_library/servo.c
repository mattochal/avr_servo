/* servo.c

    Copyright Mateusz Ochal (mo4g15@soton.ac.uk)
    University of Southampton

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "rios.h"

uint8_t current = 0; // current servo to update, if -1 there are no servos to update
uint8_t n = 0;
uint8_t servos[][2];

void os_init_servo(uint8_t arr[][2], uint8_t size){ /* number of servos */
	n = size;
	servos = arr;
	DDRD  |=  _BV(pin);   /* SERVO pin out */
	PORTD &= ~_BV(pin);   /* SERVO off */
}

uint8_t set_position(uint8_t pin, uint8_t pos){ /* position to go to */
	
}

uint8_t update_all(){
	uint8_t i;
	uint8_t pin;
	uint8_t pos;

	
	cli();
	for (i = 0; i < size; ++i){
		pin = servos[current][1];
		pos = servos[current][1];

		PORTD |= _BV(pin);   /* SERVO on */
		PORTD &= ~_BV(pin);   /* SERVO off */

		__delay_us();
	}
	sei();
}

uint8_t update_next(){
	if (size == 0) {
		return -1;
	}

	current = (++current) % size;

	uint8_t pin = positions[current];
	uint8_t pos = positions[current];

	pin += high
	__delay( 1000 + pos*1000/255 );
	pin |= LOW
}