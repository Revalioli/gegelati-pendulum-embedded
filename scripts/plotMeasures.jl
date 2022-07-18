#!/usr/bin/julia

using Plots
using JSON3

dir = "../TPG/Trigo_1"
measuresDataJson = JSON3.read(read("$(dir)/2022-07-18_11-53-12_data.json"))

gr(size=(1200, 800))
l = @layout [a b]
p1 = plot(measuresDataJson["step"], measuresDataJson["current"])
p2 = plot(measuresDataJson["step"], measuresDataJson["power"])
plot(p1, p2, layout=l)
gui()

readline()