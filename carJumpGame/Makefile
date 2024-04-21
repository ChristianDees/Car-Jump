# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:carJump.elf

#additional rules for files
carJump.elf: ${COMMON_OBJECTS} updates.o wdt_handler.o buzzer.o state_machines.o random_int.o p2_interrupt_handler.o switches.o wdt_c_handler.o int_to_str.o environment.o characters.o clock.o hud.o main.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

load: carJump.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf
