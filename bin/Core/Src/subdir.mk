################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

OBJS += \
./Core/Src/ina219.o \
./Core/Src/mainEnergyBenchmark.o \
./Core/Src/serialCommunication.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: $(PROJECT_PATH)/Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I$(PROJECT_PATH)/Core/Inc -I$(PROJECT_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc -I$(PROJECT_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I$(PROJECT_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I$(PROJECT_PATH)/Drivers/CMSIS/Include -I"$(PROJECT_PATH)/Core/Inc/Benchmark" -I"$(PROJECT_PATH)/Core/Inc/Pendulum" -I"$(PROJECT_PATH)/Core/Inc/Peripherals" -O1 -ffunction-sections -fdata-sections -Wall --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o: $(PROJECT_PATH)/Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DSTM32 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I$(PROJECT_PATH)/Core/Inc -I$(PROJECT_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc -I$(PROJECT_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I$(PROJECT_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I$(PROJECT_PATH)/Drivers/CMSIS/Include -I"$(PROJECT_PATH)/Core/Inc/Benchmark" -I"$(PROJECT_PATH)/Core/Inc/Pendulum" -I"$(PROJECT_PATH)/Core/Inc/Peripherals" -O1 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

