# Gegelati Pendulum Embedded

GEGELATI is an open-source reinforcement learning framework for training AI based on the Tangled Program Graphs (TPG) algorithm.
Feel free to checkout the [Gegelati github repository](https://github.com/gegelati/gegelati) to learn more about the library and the TPGs. You can also read the [original Stephen Kelly's paper on TPGs](http://stephenkelly.ca/research_files/Kelly-Stephen-PhD-CSCI-June-2018.pdf).

This repository main goal is to port the Pendulum environment from the [Gegelati Apps](http://stephenkelly.ca/research_files/Kelly-Stephen-PhD-CSCI-June-2018.pdf) to a microcontroller target, and measure the energy consumption of the microcontroller when running a pre-trained TPG using the Pendulum environment. The TPG is a C-generated version obtained using the CodeGen features of Gegelati. 

TPG trainings and code generations are not done on the microcontroller, but using the Gegelati library on a general purpose computer.

The microcontroller used here was a [NUCLEO-64 STM32F446RE](https://www.st.com/en/evaluation-tools/nucleo-f446re.html), and the current sensor for energy measurements is an INA219 breakout, thus the code of this repository is based on correspondings tools and libraries. Adaptations must be done in order to use another microcontroller or another way to measurement the energy consumption.
Moreover, this project was made on Linux, instructions provided below will therefore assume you are using this OS.

This work is the result of an internship at the VAADER team of the IETR laboratory.

## How to setup the repository

Some third-party softwares are needed to fully use this repository.
You can either install them using your package manager if they are available on it, or follow the instructions on the offical websites for a manual installation.

### Cross compile toolchain

In order to cross compile the C/C++ program for the STM32 board, you will need to install :
* arm-none-eabi-gcc (the gcc version for ARM processors)
* make (commonly built-in with your Linux distro)

You can also update your Linux if your are not sure you got all the C/C++ librairies.

### Scripting languages

Make sure you have the following programs installed on your computer :
* [Ruby](https://www.ruby-lang.org/fr/documentation/installation/)
* [Python](https://wiki.python.org/moin/BeginnersGuide/Download)
* [Julia](https://www.ruby-lang.org/fr/documentation/installation/)

Ruby scripts are use to automate all the process when measuring energy consumption on different TPGs. Python and Julia scripts process data from the energy measurment and from the TPGs, and output them as nice graphs and figures.

In addition to these programs, some specifics packages are required for each one :
* Ruby gems :
    * [serialport](https://rubygems.org/gems/serialport/)
* Python packages :
    * Numpy
    * Matplotlib
* Julia packages :
    * Plots
    * JSON3

You can install them using the script `install_packages.sh`.

### STMicroelectronics softwares

To interact with the STM32 board, you will need to :
* update [libusb]((https://libusb.info/)) to at least version 1.0.0
* install [STMCubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) as it will be used to flash the compiled programs on the STM32 board. Normally, this comes with a GUI and a CLI version (the latter is the one used by the scripts).

Then you can check if can communicate with the STM32 board.
* Connect the STM32 board to the PC using the usb port.
* Lanch the GUI version of STM32CubeProgrammer, and on the right panel check that you have the following configuration :
    * the ST-Link option is choosed next to the "Connect" button
    * Port = SWD
    * Frequency = 4000
    * Serial number : the serial number of your board, if "No ST-Link detected" is displayed, click on the refresh button your board should be detected
* Click on the "Connect" button.
* If no error occured, the content of the FLASH memory should appear on the main window !

If something went wrong at some point, try again to install the software. You can also use [this tutorial](https://wiki.st.com/stm32mpu/wiki/STM32CubeProgrammer#How_to_flash_with_STM32CubeProgrammer) to install libusb, the usb drivers and STM32CubeProgrammer.

**Optional**

Even if not required to start energy measurments, you can install [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) if you want to go beyond the current state of this repository.
The C/C++ source code is available as a STM32CubeIDE project, and it would make easier any code modification and debug.


## Repository overview

### PendulumEmbeddedSTMProject

**TODO**

### bin

**TODO**

### TPG

**TODO**

### Other scripts

**TODO**


## Launch the energy measurments

**TODO**