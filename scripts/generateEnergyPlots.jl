#!/usr/bin/julia

using PlotlyJS
using JSON3

include("createPlots.jl")

# =====[ Script parameters ]=====

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

i = 1
while i <= length(ARGS)

    a = ARGS[i]

    if a == "--show"
        global showPlot = true
    else
        println("Unknown option $a")
        exit(1)
    end

    global i += 1
end


# Searching results paths with the energy_data.json file

valid_result_paths = [] # Valid paths to all energy_data.json files

isresultdir(fullPath::String) = occursin(r"^.*[0-9\-_]*_results$", fullPath)
isdirtoevaluate(fullPath::String) = isfile(fullPath * "/energy_data.json") && !isfile(fullPath * "/graph.html") && !isfile(fullPath * "/graph.png")

cd((@__DIR__) * "/../TPG") do 

    tpgDirs = filter(isdir, readdir(join=true))

    for p in tpgDirs

        readdir(p, join=true) |>
            dirs -> filter(isresultdir, dirs) |>
            resDirs -> filter(isdirtoevaluate, resDirs) |>
            validDirs -> append!(valid_result_paths, validDirs .* "/energy_data.json")

    end

end


# =====[ Plotting ]=====


for path in valid_result_paths

    graph = plotEnergyData(path)
    savefig(graph, "$(dirname(path))/graph.png", width=1920, height=1080)
    
    # Saving html version
    open("$(dirname(path))/graph.html", "w") do io
        PlotlyBase.to_html(io, graph)
    end

    if showPlot
        displayInBrowser("$(dirname(path))/graph.html")
        println("Press enter to continue")
        readline()
    end
end
