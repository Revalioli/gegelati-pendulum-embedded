using JSON3

dataPath = ARGS[1]  # path to energy_data.json

measuresDataJson = JSON3.read(read(dataPath))

# Averages

stepData = copy(measuresDataJson["step"])
currentData = copy(measuresDataJson["current"])
powerData = copy(measuresDataJson["power"])

toRemove = findall(==(-1), measuresDataJson["step"])
deleteat!(stepData, toRemove)
deleteat!(currentData, toRemove)
deleteat!(powerData, toRemove)

avgCurrent = sum(currentData) / length(currentData)
avgPower = sum(powerData) / length(powerData)

# Energy consumption

timeUnit = measuresDataJson["metadata"]["dataTimeUnit"]
timeMultiplier = measuresDataJson["metadata"]["dataTimeMultiplier"]

measureStepTime = 0.0;  # In seconds

if timeUnit == "ms"
    measureStepTime = timeMultiplier / 10^3
elseif timeUnit == "us"
    measureStepTime = timeMultiplier / 10^6
elseif timeUnit != "s"
    println("Unknown time unit, default to s")
    measureStepTime = timeMultiplier
end

totalEnergy = 0.0

for p in powerData
    global totalEnergy += p * measureStepTime
end


# Write results in file

writePath = ""

if dirname(dataPath) |> isempty
    global writePath = "energy_summary.md"
else
    global writePath = "$(dirname(dataPath))/energy_summary.md"
end

open(writePath, "w") do io
    write(io, "# Energy statistics\n")
    write(io, "Average current : $avgCurrent A\n")
    write(io, "Average power : $avgPower W\n")
    write(io, "Average step execution time : $(measuresDataJson["metadata"]["inferenceTavg"])\n")
    write(io, "Total energy consumption : $totalEnergy J\n")
end
