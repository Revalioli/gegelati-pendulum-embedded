#!/usr/bin/ruby

require 'fileutils'


# =====[ Script parameters ]=====


# Set here the compiler and STM32 programs path
compilerDirPath = "/opt/st/stm32cubeide_1.9.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.linux64_1.0.0.202111181127/tools/bin"

# === Checking files ===

def checkCodeGenFiles directoryName
    # Returns true if directoryName contains files pendulum.c, pendulum.h, pendulum_program.c and pendulum_program.h

    ["pendulum.c", "pendulum.h", "pendulum_program.c", "pendulum_program.h"].each { |f|
        return false if not File.exist? directoryName+'/'+f
    }

    return true
end

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

    exit 1 if $?.exitstatus != 0

    puts "Successful compilation !"

    src = "PendulumEmbeddedSTMProject/ReleaseEnergyBench/PendulumEmbeddedSTMProject.elf"
    FileUtils.cp(src, tpgDirName)


    # === Flashing on STM32 ===
    # Flashing is done using STM32_Programer_CLI, which must be already install and accesible using PATH

    system("STM32_Programmer_CLI -c port=SWD -w #{tpgDirName}/PendulumEmbeddedSTMProject.elf -rst")

    exit 1 if $?.exitstatus != 0


    # === Launching picocom, start inference ===

    `rm #{tpgDirName}/picocom.log`

    system("picocom /dev/ttyACM0 -b 115200 -g #{tpgDirName}/picocom.log")
    exit 1 if $?.exitstatus != 0

    # === Analysing resuts ===

    `ResultsAnalysis/plotMeasures.py #{tpgDirName}/picocom.log -s`
    exit 1 if $?.exitstatus != 0
}



