all: keypad_demo interface

keypad_demo: keypad_demo.c keypad.c
	gcc keypad_demo.c keypad.c -Wall -lwiringPi -o keypad_demo -fsanitize=undefined
interface: interface.c keypad.c lora.c
	gcc interface.c keypad.c lora.c -Wall -lwiringPi -lncurses -lm -o interface -fsanitize=undefined
