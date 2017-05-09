# avr_servo
This library is based on AVR micro controller (AT90USB1286) or LaFortuna board provided by the University of Southampton.

The library was designed to be scalable and can easily manage 8 servos but it could probably support up to 16 servos or more. It uses a timer and a output compare register to get a precise timing of the servos updating each one at a time. Since the pulses are triggered by interrupts it should be easy to implement multitasking.

## Understanding this repository

**servo_library/** is probably what you want to include in your personal project, see the header for explonations. 

This repository contains three seperate servo examples

1. **single_servo/** contains a very simple example of driving a single servo but does not use my servo library.

2. **multiple_servos/** (you guessed it) contains a more elaborate example of using multiple servos with one timer. It uses the servo library.

3. **os_with_servos/** builds on the FortunaOS to take advantage of the rotary encoder to set positions of the servos, as well as containing a demo mode.
