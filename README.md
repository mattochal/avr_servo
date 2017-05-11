# avr_servo
This library is based on AVR micro controller (AT90USB1286) or LaFortuna board provided by the University of Southampton.

The library was designed to be scalable and can easily manage 8 servos but it could probably support up to 16 servos or more. (see **multiple_servos/** to see the example). It uses a timer and a output compare register to get a precise timing of the servos updating each one at a time. All of my examples use a prescaler of F_CPU\64 or F_CPU\256 with 8MHz clock speed, so there is an upper bound on the number of servos as the timing between pulses gets longer with more servos. 

Since the pulses are triggered by interrupts it should be easy to implement multitasking. (see **os_with_servos/** to see an example)

## Understanding this repository

**servo_library/** is probably what you want to include in your personal project, see the header for explonations. 

This repository contains three seperate servo examples

1. **single_servo/** contains a very simple example of driving a single servo but does not use my servo library.

2. **multiple_servos/** (you guessed it) contains a more elaborate example of using multiple servos with one timer. It uses the servo library.

3. **os_with_servos/** builds on the FortunaOS to take advantage of the rotary encoder to set positions of the servos, as well as containing a demo mode.

### Compiling and running the examples
For that you need the standard AVR development suite:
* AVR GCC compiler and AVR C libraries 
  * For macOS : (https://www.obdev.at/products/crosspack/index.html "CrossPack for AVR development")
  * For Windows : (https://sourceforge.net/projects/winavr/ "WinAVR")
* DFU programmer
  * get from (http://dfu-programmer.github.io/ "here")
  
Compile the examples with the Univeral Makefile, which are in each example folder and you should be good to go!

## Credit
Many thanks to:
* **Klaus Peter-Zauner** for providing the Universal Makefile used in the examples
* **Steve Gunn** for the LCD library used in the examples
* **Frank Vahid, Tony Givargis, and Bailey Miller. Univ. of California, Riverside and Irvine** for providing the RIOS
