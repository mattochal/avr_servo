/* multiple_servos.c

	Copyright Mateusz Ochal (mo4g15@soton.ac.uk)
	University of Southampton


This file shows an example of controlling a standard servo motor using PWM from AVR
chip AT90USB1286.

This solution is based on the LaFortuna board provided by University of Southampton,
so assumes the presence of a roatary encoder, but you can use a potential divider 
circuit and read an analogue signal from ports instead.

Based on Task 1 Solution
*/

/* For La Fortuna board

| Port | Pin | Function                  |
|------+-----+---------------------------|
| B    |   7 | Green LED                 |
| E    |   4 | Rotary Encoder A          |
| E    |   5 | Rotary Encoder B          |
| D    |   0 | Servo					 |

*/

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "servo.h"

#ifdef SERVO_DEBUG
#include "printf.h"
#endif

#define COUNT 3


void init(void);
int8_t enc_delta(void);


volatile int8_t delta;



void main(void) {
	uint8_t cnt = 0;
	int16_t res;

	init();
	#ifdef SERVO_DEBUG
	init_lcd();
	#endif


	add_servo(PD0, 0);
	add_servo(PD1, 255);
	add_servo(PD2, 0);
	add_servo(PD3, 255);
	add_servo(PD4, 0);
	add_servo(PD5, 255);
	add_servo(PD6, 0);
	add_servo(PD7, 255);

	sei();

	for (;;) {
		/* ---- Delete if no rotary encoder -------- */
		res = cnt + enc_delta()*4;
		if (res > 255) {
			cnt = 255;
		} else if (res < 0) {
			cnt = 0;
		} else {
			cnt = (uint8_t)res;
		}
		/* ---------------------------------------- */
		set_position(0, cnt);
		set_position(1, 255 - cnt);
		set_position(2, cnt);
		set_position(3, 255 - cnt);
		set_position(4, cnt);
		set_position(5, 255 - cnt);
		set_position(6, cnt);
		set_position(7, 255 - cnt);
	}
}



/* Configure I/O Ports */
void init(void) {

	/* 8MHz clock, no prescaling (DS, p. 48) */
	CLKPR = (1 << CLKPCE);
	CLKPR = 0;

	/* Configure I/O Ports */
	DDRB  |=  _BV(PB7);   /* SERVO pin out */

	/* ---- Delete if no rotary encoder --------*/
	DDRE &= ~_BV(PE4) & ~_BV(PE5);  /* Rot. Encoder inputs */
	PORTE |= _BV(PE4) | _BV(PE5);   /* Rot. Encoder pull-ups */
	/* ---------------------------------------- */

	TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00); /* Fast PWD */
	TCCR0B = _BV(CS02); /* FCPU/256 prescaler */

	OCR0A = 0; /* Changing this register between MIN_STEP and MAX_STEP will change the period of timer interrupts */

	TIMSK0 |= _BV(OCIE0A) | _BV(TOIE0); /* Enable timer and overflow interrupts, DS 14.8.6  */
}


ISR( TIMER0_COMPA_vect ) {
	servo_pulse_off();
	next();
}


ISR( TIMER0_OVF_vect ){
	cli();
	static int8_t last;
	int8_t new, diff;
	uint8_t wheel;
	
	servo_pulse_on();

	wheel = PINE;
	new = 0;
	if( wheel  & _BV(PE4) ) new = 3;
	if( wheel  & _BV(PE5) )
		new ^= 1;                  /* convert gray to binary */
	diff = last - new;         /* difference last - new  */
	if( diff & 1 ){            /* bit 0 = value (1) */
		last = new;                /* store new as next last  */
		delta += (diff & 2) - 1;   /* bit 1 = direction (+/-) */
	}
	sei();
}

/* ---- Delete if no rotary encoder --------*/
/* read two step encoder */
int8_t enc_delta() {
	int8_t val;

	cli();
	val = delta;
	delta &= 1;
	sei();

	return val >> 1;
}
/* ---------------------------------------- */
