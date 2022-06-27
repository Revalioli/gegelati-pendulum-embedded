################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 

OBJS += \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.o \
./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32F4xx_HAL_Driver/Src/%.o: $(PROJECT_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/%.c Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I$(PROJECT_PATH)/Core/Inc -I$(PROJECT_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc -I$(PROJECT_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I$(PROJECT_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I$(PROJECT_PATH)/Drivers/CMSIS/Include -I"$(PROJECT_PATH)/Core/Inc/Benchmark" -I"$(PROJECT_PATH)/Core/Inc/Pendulum" -I"$(PROJECT_PATH)/Core/Inc/Peripherals" -O1 -ffunction-sections -fdata-sections -Wall --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

