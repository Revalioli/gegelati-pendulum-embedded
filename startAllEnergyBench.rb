#!/usr/bin/ruby

require 'fileutils'
require 'serialport'
require 'optparse'

require_relative 'scripts/logToJson'



# =====[ Script parameters ]=====

# Path to the serial port corresponding to the STM32 board
serialPortPath = "/dev/ttyACM0"

# Set to true if you want to display generated graphs. Image files will still be generated.
showGraph = true

# ===============================




C_UINT_MAX = 4294967295     # C language max unsigned int constant value from limits.h


# Get all subdirectories of dir that contained all the required files, given as an array of relative path from dir/subdirectory/.
#
# The returned array contains relative paths from dir for all valid subdirectories.
def getValidDirectories (dir, requiredFiles)

    valid_TPG_directories = []  # Store all subdirectories path containing the required TPG CodeGen files
    Dir.open(dir) { |d|

        d.each_child { |child|
            tpgDirName = d.path + '/' + child
            
            if File.directory? tpgDirName

                # Condition return the first missing file, otherwise nil is returned
                if requiredFiles.find { |f| not File.exist? "#{tpgDirName}/#{f}" }.nil?
                    valid_TPG_directories << tpgDirName
                end

            end
        }

    }

    return valid_TPG_directories

end

# Exit the script if last call to a subprocess return an exit code different from 0
def checkExitstatus
    exit 1 if $?.exitstatus != 0
end


# =====[ Program arguments managment ]=====

# CodeGen : generate CodeGen files for all TPG .dot files in valid subdirectories of TPG/
# Measure : compile, load and run measurements on the STM32 board, and store results, for all valid subdirectories of TPG/
# AnalyzeExecutions : from the measurement results, compute new data and export statistics about the execution that has been measured
# PlotResults : take all statistics from the previous step and render graphs and plots using julia script and module PlotlyJS 

# If false, the step is skipped
stages = { "CodeGen" => true, "Measures" => true, "Analysis" => true, "PlotResults" => true}


OptionParser.new{ |parser|

    parser.on("--skip STAGE"){ |to_skip|
        if stages.key?(to_skip)
            stages[to_skip] = false
            puts "Skipping stage #{to_skip}"
        else
            puts "Unknown stage #{to_skip}"
        end
    }

}.parse!


# =====[ CodeGen stage ]=====

if stages["CodeGen"]
    
    puts "\033[1;31m=====[ CodeGen stage ]=====\033[0m"

    system("./scripts/generate_TPG.sh")
    checkExitstatus

end


# =====[ Measurments stage ]=====

if stages["Measures"]

    puts "\033[1;31m=====[ Measurments stage ]=====\033[0m"


    requiredFiles = ["CodeGen/pendulum.c", "CodeGen/pendulum.h", "CodeGen/pendulum_program.c", "CodeGen/pendulum_program.h"]
    valid_TPG_directories = getValidDirectories("TPG", requiredFiles)

    puts "Valid TPG subdirectories are #{valid_TPG_directories}"


    # Compiling, flashing on STM32, do inference and analyse results for each TPG

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
    
        # Moving .elf binary to the current TPG subdirectory
        srcElf = "bin/PendulumEmbeddedMeasures.elf"
        destElf = "#{tpgDirName}/CodeGen"
        FileUtils.cp(srcElf, destElf)
    
    
        # === Loading program on STM32 flash memory ===

        # Loading is done using the program STM32_Programer_CLI
    
        system("STM32_Programmer_CLI -c port=SWD -w #{destElf}/PendulumEmbeddedMeasures.elf -rst")
        checkExitstatus
    
    
        # === Start serial interface and inference ===

        # Create subdirectory for saving results
        resultPath = "#{tpgDirName}/#{currentTime}_results"
        FileUtils.mkdir(resultPath)
    
        # energy.log store messages received from the STM32 board
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
    
    
        # === Export meaningfull data from the log ===
    
        dataPath = "#{resultPath}/energy_data.json"
        logToJson(logPath, "#{dataPath}")


        # === Compute averages statistics for display at end of measurements
        system("julia --project ./scripts/computeEnergyStats.jl #{dataPath}")
        checkExitstatus
    
        File.open("#{resultPath}/energy_stats.md").each_line { |line|
            
            case line
            when /Average current : (\d+\.?\d*) A/
                currentAvgs[tpgDirName] = $1.to_f
            when /Average power : (\d+\.?\d*) W/
                powerAvgs[tpgDirName] = $1.to_f
            # when /Average step time : (\d+\.?\d*) ([a-zA-Z]*)/    # TODO implement this in julia script
            #     stepTimeAvgs[tpgDirName] = $1.to_f
            #     timeUnits[tpgDirName] = $2
            end
        }.close
    
    }



    # Displaying global results

    puts "===[ Results summary ]==="

    valid_TPG_directories.sort!.each { |tpgDirName|
        puts "#{tpgDirName}"
        puts "\tAverage current : #{(currentAvgs[tpgDirName] * 1000).round(4)} mA"
        puts "\tAverage power : #{powerAvgs[tpgDirName].round(4)} W"
        # puts "\tAverage step time : #{stepTimeAvgs[tpgDirName].round(4)} #{timeUnits[tpgDirName]}"
    }

end



# =====[ Analysis stage ]=====


if stages["Analysis"]

    puts "\033[1;31m=====[ Analysis stage ]=====\033[0m"
    
    puts "TODO analyse execution"

end


# =====[ Plot results stage ]=====

if stages["PlotResults"]
    
    puts "\033[1;31m=====[ PlotResults stage ]=====\033[0m"
    
    system("julia --project ./scripts/generateEnergyPlots.jl")

end
