PROJECT_PATH=../PendulumEmbeddedSTMProject
EXECUTABLE_NAME:=PendulumEmbeddedMeasures.elf

-include sources.mk
-include $(SUBDIRS_MK)

# All Target
all: main-build

# Main-build Target
main-build: $(EXECUTABLE_NAME)

# Compilation
$(EXECUTABLE_NAME): $(OBJS) $(USER_OBJS) $(PROJECT_PATH)/STM32F446RETX_FLASH.ld
	arm-none-eabi-g++ -o "$(EXECUTABLE_NAME)" $(OBJS) $(LIBS) -mcpu=cortex-m4 -T"$(PROJECT_PATH)/STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '


# Other Targets
 
clean:
	rm -f $(EXECUTABLE_NAME) $(OBJS)
	-@echo ' '

.PHONY: all clean main-build