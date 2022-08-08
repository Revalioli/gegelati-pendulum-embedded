# Gegelati Pendulum Embedded

GEGELATI is an open-source reinforcement learning framework for training AI based on the Tangled Program Graphs (TPG) algorithm.
Feel free to checkout the [Gegelati github repository](https://github.com/gegelati/gegelati) to learn more about the library and the TPGs. You can also read the [paper of Stephen Kelly's about the TPG algorithm](http://stephenkelly.ca/research_files/Kelly-Stephen-PhD-CSCI-June-2018.pdf).

This repository port the Pendulum environment from [Gegelati Apps](https://github.com/gegelati/gegelati-apps) to a microcontroller target, and provide tools to measure the energy consumption of the microcontroller when running a TPG in infernece using the Pendulum environment. The TPG is a C-generated version obtained using the CodeGen features of Gegelati. 

TPG trainings and code generations are not done on the microcontroller, but rather use the Gegelati library on a computer.

The microcontroller used here is the [NUCLEO-64 STM32F446RE](https://www.st.com/en/evaluation-tools/nucleo-f446re.html), and the current sensor for energy measurements is an INA219 breakout, thus the code of this repository is based on the correspondings tools and libraries. Adaptations must be done in order to use another microcontroller or another way to measurement the energy consumption.
This project was done on a Linux environment, instructions provided below will therefore assume you are using this OS.

This work is the result of an internship at the VAADER team of the IETR laboratory.

-----------------------

## Dependencies

You will need to install some libraries and third-party softwares to fully use this repository.
Either install them using your package manager if they are available on it, or follow the instructions on the offical websites for manual installation. All softwares must then be accessible via the PATH.

- [The GEGELATI library](https://github.com/gegelati/gegelati), to train and generate new TPGs, and also to get execution statistics after an energy consumption measurement.
- [The GNU C/C++ arm-none-eabi toolchain](https://developer.arm.com/downloads/-/gnu-rm) to cross compile the C/C++ program for the STM32 board.
- [make](https://www.gnu.org/software/make/) (commonly built-in with your Linux distro)
- [Ruby](https://www.ruby-lang.org/fr/documentation/installation/)
- [Julia](https://www.ruby-lang.org/fr/documentation/installation/)
- [libusb]((https://libusb.info/)) to at least version 1.0.0
- [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html), it will be used to flash programs on the STM32 board (the CLI version is the one used by the scripts).

### Check the installation

For most of these programs, you can run their command with `--version` to check if they are all correctly installed : 

```
arm-none-eabi-gcc --version
arm-none-eabi-g++ --version
make --version
ruby --version
julia --version
STM32_Programmer_CLI --version
```


For STM32CubeProgrammer you can check if it can communicate with your STM32 board : 
* Connect the STM32 board to the PC using the usb port.
* Lanch the GUI version of STM32CubeProgrammer, and on the right panel check that you have the following configuration :
    * the ST-Link option is choosed next to the "Connect" button
    * Port = SWD
    * Frequency = 4000
    * Serial number : the serial number of your board, if "No ST-Link detected" is displayed, click on the refresh button and your board should be detected
* Click on the "Connect" button.
* If no error occured, the content of the FLASH memory should appear on the main window !

If something went wrong at some point, try again to install the software. You can also use [this tutorial](https://wiki.st.com/stm32mpu/wiki/STM32CubeProgrammer#How_to_flash_with_STM32CubeProgrammer) to install libusb, the usb drivers and STM32CubeProgrammer.

### Ruby and Julia packages

Some external packages are required for the scripts. You can just execute `install_packages.sh` or install them manually :
- for Ruby, you need the `serialport` gem.
- for Julia, you need the `PlotlyJS` and `JSON3` packages.

### Optional

If you want to go beyond the current state of this repository, you can install [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) to edit the C/C++ code, as it is provided as a STM32CubeIDE project.

--------------------------------

## Repository overview

To better understand what does what, here are some details on each sub-directories.

### PendulumEmbeddedSTMProject

STM32CubeIDE project of the C/C++ source code for the program flashed on the STM32 board. It contains the embedded version of the Pendulum environment, the peripheral configurations, special classes for benchmark and utilities to communicate with the current sensor. You can also find the source code of the INA219 interface at [this url](https://github.com/Revalioli/INA219_STM32_HAL).

Read the README.md of this directory for more information about the structure of the code, the board default configuration and the default pin connections witht the current sensor.

### bin

Build directory for the C/C++ code of **PendulumEmbeddedSTMProject** used by the main script `startAllEnergyBench.rb` to compile the program for the STM32 (the **bin** subdirectory of **PendulumEmbeddedSTMProject** is only used by the STM32CubeIDE project).

### TPG

This is where all the TPGs you want to evaluate will be. Each TPG has its own subdirectory, with the needed files for code generation and the results of every energy consumption measurement. For more information about this directory, read the corresponding README.md.

### scripts

Most of the scripts to automate processes and generate files and plots are in this directory. You do not need to use them directly if you just want to do measurments on pre-trained TPGs. The corresponding README.md gives more explanation on each script.

### Trainer-Generator

C++ project using the GEGELATI library to train, generate CodeGen and analyse TPGs. You need to setup this directory only if you want to train your Pendulum TPGs with this repo.

-------------------

## Launch the energy measurments

* For each evaluated TPG, setup a subdirectory in TPG/ with the required files.
* If not already done, connect the INA219 current sensor to the STM32 boardn, and the board to your PC with the USB cable.
* Execute `startAllEnergyBenches.rb`, eventually with the desired options. Default configuration :
    * Device file of the STM32 board for serial communication is `/dev/ttyACM0`
    * For each TPG initial state, a randomly generated seed is used.
    * Result directories prefix will be the current date and time with the format YYYY-MM-DD_HH-MM-SS.
Use the `-h` or `--help` option to learn how to use the script options.
* Results are then generated in each TPG subdirectories, and can be used when the script exit.

Don't hesite to read the `create_plots.jl` file to see what you can do with the data, and to add your own function for plot generation !