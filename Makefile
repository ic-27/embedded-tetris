CC=avr-gcc
FLAGS=-Wall -Wextra -Os -mmcu=atmega32

DIR1=inc/
DIR2=drivers/inc
INC= $(DIR1) $(DIR2)
INC_PARAMS=$(foreach d,$(INC),-I$d)
OUTPUT=main.elf
PORT=/dev/ttyACM0

all: audio_driver.o display_driver.o bt_driver.o power.o button_handler.o tetris.o display.o movement.o main.o
	$(CC) $(FLAGS) -o $(OUTPUT) $(INC_PARAMS) $^
main.o: main.c
	$(CC) $(FLAGS) $(INC_PARAMS) -c $<
audio_driver.o: drivers/src/audio_driver.c
	$(CC) $(FLAGS) $(INC_PARAMS) -c $<
display_driver.o: drivers/src/display_driver.c
	$(CC) $(FLAGS) $(INC_PARAMS) -c $<
bt_driver.o: drivers/src/bt_driver.c
	$(CC) $(FLAGS) $(INC_PARAMS) -c $<
power.o: drivers/src/power.c
	$(CC) $(FLAGS) $(INC_PARAMS) -c $<
button_handler.o: src/button_handler.c
	$(CC) $(FLAGS) $(INC_PARAMS) -c $<
tetris.o: src/tetris.c
	$(CC) $(FLAGS) $(INC_PARAMS) -c $<
display.o: src/display.c
	$(CC) $(FLAGS) $(INC_PARAMS) -c $<
movement.o: src/movement.c
	$(CC) $(FLAGS) $(INC_PARAMS) -c $<

upload: main.elf
	avrdude -p m32 -P $(PORT) -c avrisp -b 19200 -U flash:w:$(OUTPUT)
ext_clk: #check on https://www.engbedded.com/fusecalc/
	avrdude -p m32 -P $(PORT) -c avrisp -b 19200 -U lfuse:w:0xbf:m -U hfuse:w:0xd1:m
clean:
	rm $(OUTPUT) *.o
