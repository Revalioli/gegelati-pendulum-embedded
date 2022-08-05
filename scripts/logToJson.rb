require 'json'

##
#   Parse a energy measurment log file and export data in a json file, with a few other information.
#
#   **logPath** path of the parsed .log file.
#   **jsonPath** path of the json file to be written.
#   **seed** the seed used for the measurement in the .log file, or nil if not specified.
#
#   **Return :** the Hash used to generate the json file.
def logToJson(logPath, jsonPath, seed=nil)
    # Format the log file from a energy measurment to a json file, easily parsable by any language.

    logStart = "##### Log Start #####"
    logEnd = "##### Log End #####"

    floatRegex = /-?\d+\.?\d*/
    integerRegex = /-?\d+/
    textRegex = /[a-zA-Z]+/

    dataLine = /(#{integerRegex})\t(#{floatRegex})\t(#{floatRegex})/
    paramLine = /dataTimeUnit : (#{textRegex})\tdataTimerMultiplier : (#{floatRegex})\tStartAngle : (#{floatRegex})\tStartVelocity : (#{floatRegex})/
    headerLine = /Step\tCurrent\tPower/
    executionTimingLine = /\=\=\= T_(#{textRegex}) : (#{floatRegex}) (#{textRegex})/


    jsonHash = {"metadata" => {}, "summary" => {}, "step" => [], "current" => [], "power" => [] }
    skipLine = false

    # Parsing energy.log
    File.open(logPath).each_line { |line|
        
        if skipLine
            # Not in a log section at the moment
            skipLine = !(line == logStart)
        elsif line == logEnd
            # True if we reached the end of the log section
            skipLine = false;
        else
            # Parsing time
            
            case line
            when paramLine
                jsonHash["metadata"]["dataTimeUnit"] = $1
                jsonHash["metadata"]["dataTimeMultiplier"] = $2.to_f
                jsonHash["metadata"]["startAngle"] = $3.to_f
                jsonHash["metadata"]["startVelocity"] = $4.to_f
            when headerLine
                # Skip header
                next
            when dataLine
                jsonHash["step"] << $1.to_i
                jsonHash["current"] << $2.to_f
                jsonHash["power"] << $3.to_f
            when executionTimingLine
                timing_name = $1
                value = $2
                unit = $3
                jsonHash["summary"]["executionT#{timing_name}"] = "#{value.sub(/.?0*$/, '')} #{unit}"
            end

        end

    }.close


    # Remove invalid data

    toRemove = []

    jsonHash["step"].each.with_index { |s, idx|
        toRemove << idx if s < 0
    }

    toRemove.each { |delete_index|
        jsonHash["step"].delete_at(delete_index)
        jsonHash["current"].delete_at(delete_index)
        jsonHash["power"].delete_at(delete_index)
    }

    # Compute additional information

    jsonHash["summary"]["averageCurrent"] = jsonHash["current"].sum / jsonHash["current"].length
    jsonHash["summary"]["averagePower"] = jsonHash["power"].sum / jsonHash["power"].length
    
    timeUnit = jsonHash["metadata"]["dataTimeUnit"]
    timeMultiplier = jsonHash["metadata"]["dataTimeMultiplier"]

    measureStepTime = timeMultiplier  # In seconds

    case timeUnit
    when "ms"
        measureStepTime = timeMultiplier / 10**3
    when "us"
        measureStepTime = timeMultiplier / 10**6
    when "s"    # Do nothing
    else
        puts "Unknown time unit, default to s"
    end

    jsonHash["summary"]["totalEnergy"] = jsonHash["power"].sum { |p| p * measureStepTime } # In J


    jsonHash["metadata"]["seed"] = seed



    File.write(jsonPath, JSON.pretty_generate(jsonHash))

    jsonHash    # Return to caller

end
