#define PRESCALER 256 /* change this accordingly */

void os_init_servo(uint8_t arr[][2], uint8_t size); /* number of servos */
uint8_t set_position(uint8_t pin, uint8_t pos); /* position to go to */
uint8_t update_all(); /* update all servos, takes 2 ms to complete and cannot be interrupted */