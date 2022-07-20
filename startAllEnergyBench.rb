#!/usr/bin/ruby

require 'fileutils'
require 'serialport'

require_relative 'scripts/logToJson'



# =====[ Script parameters ]=====

# Path to the serial port corresponding to the STM32 board
serialPortPath = "/dev/ttyACM0"

# Set to true if you want to display generated graphs. Image files will still be generated.
showGraph = true

# ===============================




C_UINT_MAX = 4294967295     # C language max unsigned int constant value from limits.h


def checkCodeGenFiles directoryName
    # Returns true if directoryName contains files pendulum.c, pendulum.h, pendulum_program.c and pendulum_program.h

    ["pendulum.c", "pendulum.h", "pendulum_program.c", "pendulum_program.h"].each { |f|
        return false unless File.exist? directoryName+'/'+f
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
        tpgDirName = d.path + '/' + child
        path = tpgDirName + "/CodeGen"
        
        if File.directory? path
            if checkCodeGenFiles(path)
                valid_TPG_directories << tpgDirName
            else
                puts "Directory #{child} is missing one or more TPG files"
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

    # === Compiling executable ===

    # No matter the TPG, the program on the STM32 will always initialise itself the same way, so as its random number generator.
    # We want the TPGs to have random initial state, so the seed use to initialise it is geerated via this ruby script
    system("make all -C ./bin TPG_SEED=#{rand(C_UINT_MAX)} TPG_CODEGEN_PATH=../#{tpgDirName}/CodeGen")
    checkExitstatus

    srcElf = "bin/PendulumEmbeddedMeasures.elf"
    destElf = "#{tpgDirName}/CodeGen"
    FileUtils.cp(srcElf, destElf)


    # === Loading program on STM32 flash memory ===
    # Loading is done using STM32_Programer_CLI which must be already install

    system("STM32_Programmer_CLI -c port=SWD -w #{destElf}/PendulumEmbeddedMeasures.elf -rst")
    checkExitstatus


    # === Launching serial interface, start inference ===

    resultPath = "#{tpgDirName}/#{currentTime}_results"
    FileUtils.mkdir(resultPath)

    logPath = "#{resultPath}/energy.log"


    logFile = File.open(logPath, "w+");
    SerialPort.open(serialPortPath, baud = 115200) { |serialport|
        until (serialport.readline == "START\r\n") do end   # Waiting for STM32 to synchronise
        serialport << "\n"  # The STM32 is waiting for a newline character, which will start the inference

        continue = true
        while continue
            line = serialport.readline

            puts line
            logFile << line

            continue = (line != "END\r\n")
        end
    }
    logFile.close


    # === Analysing and export data ===

    `./scripts/plotMeasures.py #{logPath} -S #{resultPath}`
    checkExitstatus

    logToJson(logPath, "#{resultPath}/energy_data.json")
    

    File.open("#{resultPath}/energy_stats.md").each_line { |line|
        
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