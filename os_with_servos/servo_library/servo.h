#ifndef SERVO_H
#define SERVO_H

/* Comment out / include the following line to disable / enable debug mode 
#define SERVO_DEBUG */

/* Output Compare Register that is used for senting the */
#define SERVO_OCR OCR0A

/* Data Direction Register and Data Register the servos are connected to */
#define SERVO_PORT_DDR DDRD
#define SERVO_PORT PORTD

/* Port D on the LaFortuna have 8 pins */
#define MAX_SERVOS 8

/* MAX_POS and MIN_POS assume F_CPU/64 prescaler with 8 MHz clock speed.
	and specity to the range of the SERVO_OCR value */
#define MAX_SERVO_OCR 70
#define MIN_SERVO_OCR 40

void add_servo(uint8_t pin, uint8_t pos); /* pin - pin number on the specified port register.
											pos - initial position between 0 and 255. */

void set_position(uint8_t n, uint8_t pos); /* n - servo order number (not pin number).
											pos - position to go to. */

void servo_pulse_on(); /* Turn signal on for current servo 
							Call this at the beginning of the overflow inturrupt ISR. */

void servo_pulse_off(); /* Turn signal off for current servo.
							Call this from the timer compare interrupt ISR. */

void next(); /* Go to the next servo.
				Call this imediatly after servo_pulse_off() 
				or imediatly before servo_pulse_on() to update all servos. */

#endif /* SERVO_H */