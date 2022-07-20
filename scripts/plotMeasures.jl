#!/usr/bin/julia

# using Plots, Plots.PlotMeasures
using PlotlyJS
using JSON3

include("createPlots.jl")


# =====[ Script parameters ]=====

exportPath = "."
showPlot = false

# =====[ ARGS parsing ]=====

"Return the argument of an option, with i the index of the option in ARGS. Exit program if there is no argument."
function getOptionArgument(i::Integer)
    if i+1 <= length(ARGS) && ARGS[i+1][1] != '-'
        return ARGS[i+1]
    else
        println("Missing argument for option $a")
        exit(1)
    end
end

# ARGS[1] must always be data of the energy measurements
if length(ARGS) == 0 || ARGS[1][1] == '-'
    println("No input datajson file was given")
    exit(1)
end

dataPath = ARGS[1]

i = 2
while i <= length(ARGS)

    a = ARGS[i]

    if a == "--path"
        global exportPath = getOptionArgument(i)
        global i += 1
    elseif a == "--show"
        global showPlot = true
    else
        println("Unknown option $a")
        exit(1)
    end

    global i += 1
end


p = plotEnergyData(dataPath)
savefig(p, "$exportPath/graph.png", width=1920, height=1080)

if showPlot
    displayInBrowser(p)
    println("Press enter to continue")
    readline()
end