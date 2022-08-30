CC=avr-gcc
DIR1=spi_driver
DIR2=display_driver
DIR3=drivers/audio_driver
INC=$(DIR1) $(DIR2) $(DIR3)
INC_PARAMS=$(foreach d,$(INC),-I$d)
OUTPUT=main.elf

all: main.c display_driver/display_driver.c
	$(CC) -Os -mmcu=atmega32 -o $(OUTPUT) main.c drivers/audio_driver/audio_driver.c display_driver/display_driver.c $(INC_PARAMS)
upload: main.elf
	avrdude -p m32 -P /dev/ttyACM0 -c avrisp -b 19200 -U flash:w:$(OUTPUT)
clean:
	rm $(OUTPUT)
