#!/usr/bin/ruby

require 'json'

def logToJson(logPath, jsonPath)
    # Format the log file from a energy measurment to a json file, easily parsable by any language.

    logStart = "##### Log Start #####"
    logEnd = "##### Log End #####"

    floatRegex = /-?\d+\.?\d*/
    integerRegex = /-?\d+/
    textRegex = /[a-zA-Z]+/

    dataLine = /(#{integerRegex})\t(#{floatRegex})\t(#{floatRegex})/
    paramLine = /dataTimeUnit : (#{textRegex})\tdataTimerMultiplier : (#{floatRegex})\tStartAngle : (#{floatRegex})\tStartVelocity : (#{floatRegex})/
    headerLine = /Step\tCurrent\tPower/
    inferenceTimingLine = /\=\=\= T_(#{textRegex}) : (\d* #{textRegex})/


    jsonHash = {"metadata" => {}, "step" => [], "current" => [], "power" => [] }
    skipLine = false

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
            when inferenceTimingLine
                jsonHash["metadata"]["inferenceT#{$1}"] = $2
            end

        end

        }.close

        File.write(jsonPath, JSON.pretty_generate(jsonHash))

end
