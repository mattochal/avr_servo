

void os_init_servo(uint8_t n); /* number of servos */
uint8_t add_servo(uint8_t pin); /* pin number */
uint8_t add_servo(uint8_t pin, uint8_t pos); /* pin number and initial position */
uint8_t set_position(uint8_t pos); /* position to go to */
