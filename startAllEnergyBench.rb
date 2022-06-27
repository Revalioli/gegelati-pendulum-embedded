#!/usr/bin/ruby

require 'fileutils'
require 'serialport'

# =====[ Script parameters ]=====

# Set here the compiler path and program dependencies command
compilerDirPath = "/opt/st/stm32cubeide_1.9.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.linux64_1.0.0.202111181127/tools/bin"
stm32ProgrammerCmd = "STM32_Programmer_CLI"
picocomCmd = "picocom"

# Path to the serial port corresponding to the STM32 board
serialPortPath = "/dev/ttyACM0"

# ===============================



def checkCodeGenFiles directoryName
    # Returns true if directoryName contains files pendulum.c, pendulum.h, pendulum_program.c and pendulum_program.h

    ["pendulum.c", "pendulum.h", "pendulum_program.c", "pendulum_program.h"].each { |f|
        return false if not File.exist? directoryName+'/'+f
    }

    return true
end

def checkExitstatus
    # Exit the script if last call to a subprocess return an exit code different from 0
    exit 1 if $?.exitstatus != 0
end


# === Checking files ===

valid_TPG_directories = []  # Store all subdirectories path containing the required TPG CodeGen files
Dir.open("TPG") { |d|

    d.each_child { |child|
        path = d.path + '/' + child
        
        if File.directory? path
            if checkCodeGenFiles path
                valid_TPG_directories << path
            else
                puts "Directgory #{child} us missing one or more TPG files"
            end
        end
    }

}

puts "Valid TPG subdirectories are #{valid_TPG_directories}"


# Compiling, flashing on STM32, do inference and analyse results for each TPG


currentTime = Time.now.strftime("%Y-%m-%d_%H-%M-%S")

valid_TPG_directories.each { |tpgDirName|

    # === Moving file for compilation ===

    # Source files
    dest = "PendulumEmbeddedSTMProject/Core/Src/Pendulum"
    ["pendulum.c", "pendulum_program.c"].each { |f|
        src = tpgDirName + '/' + f

        FileUtils.cp(src, dest)
    }


    # Header files
    dest = "PendulumEmbeddedSTMProject/Core/Inc/Pendulum"
    ["pendulum.h", "pendulum_program.h"].each { |f|
        src = tpgDirName + '/' + f

        FileUtils.cp(src, dest)
    }

    # === Compile executable ===

    # For now this use the build configuraiton generated before calling this script by STM32CubeIDE
    # It could be a good idea to write a separate makefile so STM32CubeIDE is not needed to build

    # Set comiler path in PATH environment variable
    ENV["PATH"] = compilerDirPath + ':' + ENV["PATH"]

    system("make all -C ./PendulumEmbeddedSTMProject/ReleaseEnergyBench")
    checkExitstatus

    src = "PendulumEmbeddedSTMProject/ReleaseEnergyBench/PendulumEmbeddedSTMProject.elf"
    FileUtils.cp(src, tpgDirName)


    # === Load program on STM32 flash memory ===
    # Load is done using STM32_Programer_CLI, which must be already install

    system("#{stm32ProgrammerCmd} -c port=SWD -w #{tpgDirName}/PendulumEmbeddedSTMProject.elf -rst")
    checkExitstatus


    # === Launching serial interface, start inference ===

    logPath = "#{tpgDirName}/#{currentTime}.log"

    FileUtils.rm(logPath, force: true)  # Remove old log file if any

    logFile = File.open(logPath, "w+");
    SerialPort.open(serialPortPath, baud = 115200) { |serialport|        
        continue = true

        puts "===== Press the STM32 user button to start inference ====="

        while continue
            line = serialport.readline

            puts line
            logFile << line

            continue = false if line == "Exiting energy bench\r\n"
        end
    }
    logFile.close

    # === Analysing resuts ===

    `./plotMeasures.py #{logPath} -S #{tpgDirName} -p #{currentTime} --show`
    checkExitstatus
}



