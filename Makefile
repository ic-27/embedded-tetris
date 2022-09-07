CC=avr-gcc
DIR1=spi_driver
DIR2=display_driver
DIR3=drivers/audio_driver
DIR4=common
DIR5=button_handler
INC=$(DIR1) $(DIR2) $(DIR3) $(DIR4) $(DIR5)
INC_PARAMS=$(foreach d,$(INC),-I$d)
OUTPUT=main.elf

PORT=/dev/ttyACM0

all: main.c display_driver/display_driver.c
	$(CC) -Os -mmcu=atmega32 -o $(OUTPUT) main.c drivers/audio_driver/audio_driver.c display_driver/display_driver.c button_handler/button_handler.c $(INC_PARAMS) 
upload: main.elf
	avrdude -p m32 -P $(PORT) -c avrisp -b 19200 -U flash:w:$(OUTPUT)
ext_clk: #check https://www.engbedded.com/fusecalc/
	avrdude -p m32 -P $(PORT) -c avrisp -b 19200 -U lfuse:w:0xbf:m -U hfuse:w:0xd1:m
clean:
	rm $(OUTPUT)
