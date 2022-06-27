################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

OBJS += \
./Core/Src/Pendulum/PendulumEnvironment.o \
./Core/Src/Pendulum/pendulum.o \
./Core/Src/Pendulum/pendulum_program.o 




# Each subdirectory must supply rules for building sources it contributes
Core/Src/Pendulum/%.o: $(PROJECT_PATH)/Core/Src/Pendulum/%.cpp Core/Src/Pendulum/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DSTM32 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I$(PROJECT_PATH)/Core/Inc -I$(PROJECT_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc -I$(PROJECT_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I$(PROJECT_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I$(PROJECT_PATH)/Drivers/CMSIS/Include -I"$(PROJECT_PATH)/Core/Inc/Benchmark" -I"$(PROJECT_PATH)/Core/Inc/Pendulum" -I"$(PROJECT_PATH)/Core/Inc/Peripherals" -O1 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Pendulum/%.o: $(PROJECT_PATH)/Core/Src/Pendulum/%.c Core/Src/Pendulum/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I$(PROJECT_PATH)Core/Inc -I$(PROJECT_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc -I$(PROJECT_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I$(PROJECT_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I$(PROJECT_PATH)/Drivers/CMSIS/Include -I"$(PROJECT_PATH)/Core/Inc/Benchmark" -I"$(PROJECT_PATH)/Core/Inc/Pendulum" -I"$(PROJECT_PATH)/Core/Inc/Peripherals" -O1 -ffunction-sections -fdata-sections -Wall --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
