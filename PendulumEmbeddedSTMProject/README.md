# The PendulumEmbeddedSTMProject directory

This is where the C/C++ code for the STM32 board is.

It contains a fonctionnal Pendulum environment and everything that is needed to communicate with an INA219 for current consummption measurements. Utility classes for energy and timing benchmark are provided, and it can also communicate in UART via serial communication with a PC, for example to export measurments.

Files `.project`, `.cproject` and `.mxproject` allows you to open and edit the code as a (STM32CubeIDE)[https://www.st.com/en/development-tools/stm32cubeide.html] project. This may be useful to edit and debug the code and easily change configurations of peripherals using STM32CubeMX.
Notice however that STM32CubeMX is a bit strict about how it generates and overwrites files : it only generates/overwrite main.c file and peripherals files in the Src and Inc directories. If you intend to use STM32CubeMX, you will ne to do a bit of moving-renaming-file-trickeries.

The `mainEnergyBenchmark.cpp` is the entry point for the program flashed when using the `startAllEnergyBenches.rb` script. It runs the provided CodeGen TPG with the Pendulum environment for 1000 inferences, measure the current and power during execution, and send the results to the serial port. It also replay the execution for timing measurement, and also send this to the serial port.
During the execution, it communicates with the `startAllEnergyBenches.rb` via the serial port and use a system of flag messages for synchronisation.

## Connections and peripherals

In the current state of this project, the INA219 sensor must be connected to the STM32 board as follows :

| INA219 pin | STM32 pin |
|------------|-----------|
| Vcc        | 3.3V or 5V |
| GND        | GND       |
| SCL        | PB6       |
| SDA        | PB7       |
| Vin+ and Vin - | IDD pins (JP6) |

This corresponds to the I2C1 peripheral.

Serial communication goes through the USB cable and the ST-Link module, but also via the UART2 module.

Timers 5 and 7 are used for periodic measurements.

Even if not used in the final program, GPIO13 is already configure to take input from the user button (the blue one), and store this input in the global variable `PCSig13`. It can be use freely for any purpose.

## How to replace the current sensor

Want to change for something else than the INA219 current sensor ? Just create a new derived class from `Monitor`, and a new class derived from `Bench`. You can use `INA219Monitor` and `INA219Bench` as references.
You may also want to create your own version of the `PendulumINA219Monitor` class.

Depending on how your current sensor work, you will have to adapt the peripherals used. Make sure you don't deactive the UART2 in the process, or timers 5 and 7.

## How to use another environment

You can create a new environment class based on the `PendulumEnvironment` class and integrate it in the existing code. You will also have to modify the data pointer and the CodeGen files.

You may also create a new derived monitor from `INA219Monitor` to record data specific to your new environment.

## How to use another board

It will require a bit more modifications but is still possible.

**To trade the STM32F446 for another STM32 board,** you will have to find and import the correct HAL headers for your board. The code and #includes in peripheral files and main files should be change. Also your board might not have the same tools for serial communication with a PC, so you may have to adapt it too.
Other files use HAL structs to interact with the board, so no need to modify them.

**If you want to use a completly different board** (from another company), then more files will have to be changed, because the STMicroelectronics HAL library could not be used anymore. At this point it's up to you to adapt the code for what you need and what you want.