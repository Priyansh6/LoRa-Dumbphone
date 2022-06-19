all: keypad_demo interface

keypad_demo: keypad_demo.c keypad.c
	gcc keypad_demo.c keypad.c -Wall -lwiringPi -o keypad_demo
interface: interface.c keypad.c
	gcc interface.c keypad.c -Wall -lwiringPi -lncurses -o interface
