/* servo_main.c

Copyright Mateusz Ochal (mo4g15@soton.ac.uk)
University of Southampton

Based on Task 1 Solution
*/

/* For La Fortuna board

| Port | Pin | Function                  |
|------+-----+---------------------------|
| B    |   7 | Green LED                 |
| E    |   4 | Rotary Encoder A          |
| E    |   5 | Rotary Encoder B          |
| D    |   0 | Servo					    |

*/

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define MIN_STEP  25
#define MAX_STEP  88


void init(void);
int8_t enc_delta(void);


volatile int8_t delta;
volatile int8_t count;



void main(void) {
	uint8_t cnt = MIN_STEP;
	uint8_t i;
	int16_t res;

	init();
	count = 3;
	sei();

	for (;;) {
		res = cnt + enc_delta();
		if (res > MAX_STEP) {
			cnt = MAX_STEP;
		} else if (res < MIN_STEP) {
			cnt = MIN_STEP;
		} else {
			cnt = res;
		}
		OCR0A = cnt;
	}
}



/* Configure I/O Ports */
void init(void) {

	/* 8MHz clock, no prescaling (DS, p. 48) */
	CLKPR = (1 << CLKPCE);
	CLKPR = 0;

	/* Configure I/O Ports */
	DDRB  |=  _BV(PB7);   /* LED pin out */
	DDRD  |=  _BV(PD0);   /* SERVO pin out */

	PORTB &= ~_BV(PB7);   /* LED off */
	PORTD &= ~_BV(PD0);   /* SERVO off */

	DDRE &= ~_BV(PE4) & ~_BV(PE5);  /* Rot. Encoder inputs */
	PORTE |= _BV(PE4) | _BV(PE5);   /* Rot. Encoder pull-ups */

	TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00); /* Fast PWD */
	TCCR0B = _BV(CS02); /* FCPU/256 prescaler */

	OCR0A = MIN_STEP;

	TIMSK0 |= _BV(OCIE0A) | _BV(TOIE0); /* Enable timer and overflow interrupts, DS 14.8.6  */
}


ISR( TIMER0_COMPA_vect ) {
	cli();
	if (count == 2) {
		PORTB &= ~_BV(PB7);   /* LED off */
		PORTD &= ~_BV(PD0);   /* SERVO off */
	}
	sei();
}


ISR( TIMER0_OVF_vect ){
	cli();
	static int8_t last;
	int8_t new, diff;
	uint8_t wheel;

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

	if (count == 0) {
		PORTB |= _BV(PB7);   /* LED on  */
		PORTD |= _BV(PD0);   /* SERVO on */
		count = 3;
	}
	count--;
	sei();
}


/* read two step encoder */
int8_t enc_delta() {
	int8_t val;

	cli();
	val = delta;
	delta &= 1;
	sei();

	return val >> 1;
}
