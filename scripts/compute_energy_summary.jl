using JSON3

dataPath = ARGS[1]  # path to energy_data.json

measuresDataJson = JSON3.read(read(dataPath))

stepData = copy(measuresDataJson["step"])
currentData = copy(measuresDataJson["current"])
powerData = copy(measuresDataJson["power"])

toRemove = findall(==(-1), measuresDataJson["step"])
deleteat!(stepData, toRemove)
deleteat!(currentData, toRemove)
deleteat!(powerData, toRemove)

avgCurrent = sum(currentData) / length(currentData)
avgPower = sum(powerData) / length(powerData)

open("$(dirname(dataPath))/energy_summary.md", "w") do io
    write(io, "# Energy statistics\n")
    write(io, "Average current : $(avgCurrent) A\n")
    write(io, "Average power : $(avgPower) W\n")
end
