###############################################################################
# Makefile for the project lin_slave
###############################################################################

## General Flags
PROJECT = lin_slave
MCU = atmega128
TARGET = lin_slave.elf
CC = avr-gcc

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -O0
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\Documents and Settings\student.SZLAB13\Desktop\_demo\SW\__LIN_demo\LIN_slave\LIB_MCU\UART" -I"C:\Documents and Settings\student.SZLAB13\Desktop\_demo\SW\__LIN_demo\LIN_slave\LIB_MCU\LIN_UART" -I"C:\Documents and Settings\student.SZLAB13\Desktop\_demo\SW\__LIN_demo\LIN_slave\LIB_MCU\TIMER" -I"C:\Documents and Settings\student.SZLAB13\Desktop\_demo\SW\__LIN_demo\LIN_slave\." 

## Objects that must be built in order to link
OBJECTS = timer2ovf_isr.o dpy_trm_s01.o lin_slave_example_main.o mcu_avr_atmega128_api.o timer1ovf_isr.o slave_lin.o lin_drv_usart.o lin_lib.o runtime_calibration_lib.o timer_drv.o timer0_lib.o timer1_lib.o timer2_lib.o timer3_lib.o uartnoint_lib.o uart_lib.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) lin_slave.hex lin_slave.eep size

## Compile
timer2ovf_isr.o: ../timer2ovf_isr.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dpy_trm_s01.o: ../dpy_trm_s01.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lin_slave_example_main.o: ../lin_slave_example_main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mcu_avr_atmega128_api.o: ../mcu_avr_atmega128_api.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer1ovf_isr.o: ../timer1ovf_isr.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

slave_lin.o: ../LIB_MCU/LIN_UART/slave_lin.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lin_drv_usart.o: ../LIB_MCU/LIN_UART/lin_drv_usart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lin_lib.o: ../LIB_MCU/LIN_UART/lin_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

runtime_calibration_lib.o: ../LIB_MCU/LIN_UART/runtime_calibration_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer_drv.o: ../LIB_MCU/TIMER/timer_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer0_lib.o: ../LIB_MCU/TIMER/timer0_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer1_lib.o: ../LIB_MCU/TIMER/timer1_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer2_lib.o: ../LIB_MCU/TIMER/timer2_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer3_lib.o: ../LIB_MCU/TIMER/timer3_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uartnoint_lib.o: ../LIB_MCU/UART/uartnoint_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart_lib.o: ../LIB_MCU/UART/uart_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) lin_slave.elf dep/* lin_slave.hex lin_slave.eep

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

