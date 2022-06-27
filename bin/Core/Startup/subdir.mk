################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################


OBJS += \
./Core/Startup/startup_stm32f446retx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: $(PROJECT_PATH)/Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -c -I"$(PROJECT_PATH)/Core/Inc/Benchmark" -I"$(PROJECT_PATH)/Core/Inc/Pendulum" -I"$(PROJECT_PATH)/Core/Inc/Peripherals" -x assembler-with-cpp --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

