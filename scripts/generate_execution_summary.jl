#!/usr/bin/julia

using PlotlyJS
using JSON3

include("create_plots.jl")


# ARGS[1] : name of the energy measures batch to plot (typically ends in _results)

if length(ARGS) < 1
    println("The name of the energy measures batch is missing")
    exit(1)
end

executionStatsPaths = []

cd((@__DIR__) * "/../TPG") do
    global executionStatsPaths = filter(isfile, readdir(join=true) .* "/$(ARGS[1])/executionStats.json")     # Get all TPG subdirectories
end

labels = [ i |> dirname |> dirname |> basename for i in executionStatsPaths ]

p = plotExecutionSummary(executionStatsPaths, labels)

displayInBrowser(p.plot)