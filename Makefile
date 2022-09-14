CC=avr-gcc
#DIR2=display_driver
DIR1=drivers/display_driver
DIR3=drivers/audio_driver
DIR4=common
DIR5=button_handler
DIR6=drivers/bt_driver
DIR7=inc/
INC= $(DIR1) $(DIR3) $(DIR4) $(DIR5) $(DIR6) $(DIR7)
INC_PARAMS=$(foreach d,$(INC),-I$d)
OUTPUT=main.elf

PORT=/dev/ttyACM0

all: main.c drivers/display_driver/display_driver.c
	$(CC) -Os -mmcu=atmega32 -o $(OUTPUT) main.c drivers/audio_driver/audio_driver.c drivers/display_driver/display_driver.c button_handler/button_handler.c drivers/bt_driver/bt_driver.c src/tetris.c src/display.c $(INC_PARAMS) 
upload: main.elf
	avrdude -p m32 -P $(PORT) -c avrisp -b 19200 -U flash:w:$(OUTPUT)
ext_clk: #check https://www.engbedded.com/fusecalc/
	avrdude -p m32 -P $(PORT) -c avrisp -b 19200 -U lfuse:w:0xbf:m -U hfuse:w:0xd1:m
clean:
	rm $(OUTPUT)
