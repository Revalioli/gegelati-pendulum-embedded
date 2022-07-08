#!/usr/bin/ruby

require 'fileutils'
require 'serialport'

# =====[ Script parameters ]=====

# Set here the compiler path and program dependencies command
compilerDirPath = "/opt/st/stm32cubeide_1.9.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.linux64_1.0.0.202111181127/tools/bin"
stm32ProgrammerCmd = "STM32_Programmer_CLI"

# Path to the serial port corresponding to the STM32 board
serialPortPath = "/dev/ttyACM0"

# Set to true if you want to display the complete 
showGraph = true

# ===============================


C_UINT_MAX = 4294967295     # C language max unsigned int constant value from limits.h


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
            if checkCodeGenFiles(path)
                valid_TPG_directories << path
            else
                puts "Directory #{child} us missing one or more TPG files"
            end
        end
    }

}

puts "Valid TPG subdirectories are #{valid_TPG_directories}"


# === Compiling, flashing on STM32, do inference and analyse results for each TPG ===


currentAvgs = {}
powerAvgs = {}
stepTimeAvgs = {}
timeUnits = {}

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


    # === Compiling executable ===

    # Adds comiler path in PATH environment variable, so the make subprocess will inherit
    ENV["PATH"] = compilerDirPath + ':' + ENV["PATH"]

    # No matter the TPG, the program on the STM32 will always initialise itself the same way, so as its random number generator.
    # We want the TPGs to have random initial state, so the seed use to initialise it is geerated via this ruby script
    system("make all -C ./bin TPG_SEED=#{rand(C_UINT_MAX)}")
    checkExitstatus

    src = "bin/PendulumEmbeddedMeasures.elf"
    FileUtils.cp(src, tpgDirName)


    # === Loading program on STM32 flash memory ===
    # Loading is done using STM32_Programer_CLI which must be already install

    system("#{stm32ProgrammerCmd} -c port=SWD -w #{tpgDirName}/PendulumEmbeddedMeasures.elf -rst")
    checkExitstatus


    # === Launching serial interface, start inference ===

    logPath = "#{tpgDirName}/#{currentTime}.log"

    FileUtils.rm(logPath, force: true)  # Remove old log file if any

    logFile = File.open(logPath, "w+");
    SerialPort.open(serialPortPath, baud = 115200) { |serialport|
        serialport.flush_input  # In case there is already something in the buffer

        until (serialport.readline == "START\r\n") do end   # Waiting for STM32 to synchronise
        serialport << "\n"  # The STM32 is waiting for a newline character, which will start the inference

        continue = true
        while continue
            line = serialport.readline

            puts line
            logFile << line

            continue = false if line == "END\r\n"
        end
    }
    logFile.close


    # === Analysing resuts ===

    `./plotMeasures.py #{logPath} -S #{tpgDirName} -p #{currentTime}`
    checkExitstatus

    File.open("#{tpgDirName}/#{currentTime}_measuresStats.md").each_line { |line|
        
        case line
        when /Average current : (\d+\.?\d*)/
            currentAvgs[tpgDirName] = $1.to_f
        when /Average power consumption : (\d+\.?\d*)/
            powerAvgs[tpgDirName] = $1.to_f
        when /Average step time : (\d+\.?\d*) ([a-zA-Z]*)/
            stepTimeAvgs[tpgDirName] = $1.to_f
            timeUnits[tpgDirName] = $2
        end
    }.close

}


# Displaying global results

puts "===[ Results summary ]==="

valid_TPG_directories.sort!.each { |tpgDirName|
    puts "#{tpgDirName}"
    puts "\tAverage current : #{currentAvgs[tpgDirName].round(4)}"
    puts "\tAverage power consumption : #{powerAvgs[tpgDirName].round(4)}"
    puts "\tAverage step time : #{stepTimeAvgs[tpgDirName].round(4)} #{timeUnits[tpgDirName]}"
}