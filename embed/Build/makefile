CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m3 \
	   -mthumb \
	   -mfloat-abi=soft \
	   -Os \
	   -ffunction-sections \
	   -fdata-sections \
	   -Wl,--gc-sections \
	   -Wall \
	   -fstack-usage \
#	   -fcyclomatic-complexity \
#	   -nostdlib
CPPFLAGS=-DSTM32F103xB \
	 -DUSE_HAL_DRIVER \
	 -I../Drivers/CMSIS/Device/ST/STM32F1/Include \
	 -I../Drivers/CMSIS/CMSIS/Core/Include \
	 -I../Drivers/STM32f1xx_HAL_Driver/Inc \
	 -I../Drivers/SystemConfig/Inc \
	 -I../Drivers/Middleware/STM32_USB_Device_Library/Class/CDC/Inc \
	 -I../Drivers/Middleware/STM32_USB_Device_Library/Core/Inc \
	 -I../Drivers/USB_DEVICE/App \
	 -I../Drivers/USB_DEVICE/Target \
	 -I../Inc
LINKER_FILE=linker_script.ld
LDFLAGS=-T ../$(LINKER_FILE)
OCD=C:/workspace/installs/openocd/bin/openocd.exe

all : program.elf

program.elf: main.o source.o startup.o system_stm32f1xx.o hal_drivers.o system_config.o usb_drivers.o usb_device.o
	@echo 'Building $@'
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(wildcard *.o) -o $@

main.o: ../main.c
	@echo 'Building $@'
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

source.o: $(wildcard ../src/*.c)
	@echo 'Building $@'
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

startup.o: ../startup.s
	@echo 'Building $@'
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

system_stm32f1xx.o: ../Drivers/CMSIS/Device/ST/STM32F1/Source/Templates/system_stm32f1xx.c
	@echo 'Building $@'
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

hal_drivers.o : $(wildcard ../Drivers/STM32F1xx_HAL_Driver/Src/*.c)
	@echo 'Building $@'
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

system_config.o : $(wildcard ../Drivers/SystemConfig/Src/*.c)
	@echo 'Building $@'
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

usb_drivers.o : $(wildcard ../Drivers/Middleware/STM32_USB_Device_Library/Class/CDC/Src/*.c) $(wildcard ../Drivers/Middleware/STM32_USB_Device_Library/Core/Src/*.c)
	@echo 'Building $@'
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

usb_device.o : $(wildcard ../Drivers/USB_DEVICE/App/*.c) $(wildcard ../Drivers/USB_DEVICE/Target/*.c)
	@echo 'Building $@'
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

rm : rm_objects rm_elf

rm_objects :
	@echo 'Deleting .o and .su objects'
	@rm -f *.o
	@rm -f *.su

rm_elf :
	@echo 'Deleting .elf objects'
	@rm -f *.elf

flash : program.elf
	@echo 'Flashing $@'
	@$(OCD) -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program program.elf verify reset exit"