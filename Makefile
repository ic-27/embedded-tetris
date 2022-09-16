CC=avr-gcc
#DIR2=display_driver
DIR1=inc/
DIR2=drivers/inc
INC= $(DIR1) $(DIR2)
INC_PARAMS=$(foreach d,$(INC),-I$d)
OUTPUT=main.elf

PORT=/dev/ttyACM0

all: main.c drivers/src/display_driver.c
	$(CC) -Os -mmcu=atmega32 -o $(OUTPUT) main.c drivers/src/audio_driver.c drivers/src/display_driver.c src/button_handler.c drivers/src/bt_driver.c src/tetris.c src/display.c src/movement.c $(INC_PARAMS) 
upload: main.elf
	avrdude -p m32 -P $(PORT) -c avrisp -b 19200 -U flash:w:$(OUTPUT)
ext_clk: #check https://www.engbedded.com/fusecalc/
	avrdude -p m32 -P $(PORT) -c avrisp -b 19200 -U lfuse:w:0xbf:m -U hfuse:w:0xd1:m
clean:
	rm $(OUTPUT)
