# avr_servo
This repository contains instructions and explanation of how to hook up a servo to an AVR micro controller (AT90USB1286) or your LaFortuna.

### Understanding this repository

This repository contains three seperate servo examples

**single_servo/** contains a very simple example of driving a single servo

**multiple_servos/** (you guessed it) contains a more elaborate example of using multiple servos with one timer. It uses the servo library.

**os_with_servos/** builds on the FortunaOS to take advantage of the rotary encoder to set positions of the servos, as well as containing a demo mode

**servo_library/** is probably what you want to include in your personal project, see the header for explonations
