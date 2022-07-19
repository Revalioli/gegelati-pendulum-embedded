#!/usr/bin/julia

using Plots, Plots.PlotMeasures
using JSON3


function setAxisColor(p::Union{Plots.Plot,Plots.Subplot}, color::Symbol, axis::Symbol = :all)
    if axis == :x 
        plot!(p,
        xforeground_color_guide = color,
        xforeground_color_axis = color,
        xforeground_color_text = color,
        )
    elseif axis == :y
        plot!(p,
        yforeground_color_guide = color,
        yforeground_color_axis = color,
        yforeground_color_text = color,
        )
    elseif axis == :z
        plot!(p,
        zforeground_color_guide = color,
        zforeground_color_axis = color,
        zforeground_color_text = color,
        )
    elseif axis == :all
        plot!(p,
        foreground_color_guide = color,
        foreground_color_axis = color,
        foreground_color_text = color,
        )
    else
        println("Unknown axis")
    end
end

println("LOAD_PATH : ", LOAD_PATH)


dir = "../TPG/Trigo_1"
measuresDataJson = JSON3.read(read("$(dir)/2022-07-19_09-51-26_data.json"))
yTick = measuresDataJson["metadata"]["dataTimeMultiplier"]
yUnit = measuresDataJson["metadata"]["dataTimeUnit"]
yAxisValues = [i*yTick for i in 0:length(measuresDataJson["step"])-1]


gr(size=(1600, 800))

plotCurrent = plot(yAxisValues, measuresDataJson["current"], color=:red, right_margin=100px, left_margin=50px,
    xlabel="Execution time ($yUnit)",
    ylabel="Curren (mA)",
    legend=(0.8, 0.25))
setAxisColor(plotCurrent, :red, :y)

subplotStep = twinx()
plot!(subplotStep, yAxisValues, measuresDataJson["step"], ylabel="Inference n°", color=:blue, legend=(0.8, 0.2))
setAxisColor(subplotStep, :blue, :y)


plotPower = plot(yAxisValues, measuresDataJson["power"], color=:green, right_margin=100px, left_margin=50px,
    xlabel="Execution time ($yUnit)",
    ylabel="Power (W)",
    legend=(0.8,0.25))
setAxisColor(plotPower, :green, :y)

subplotStep = twinx()
plot!(subplotStep, yAxisValues, measuresDataJson["step"], ylabel="Inference n°", color=:blue, legend=(0.8, 0.2))
setAxisColor(subplotStep, :blue, :y)



l = @layout [current power]
plot(plotCurrent, plotPower,bottom_margin=30px)


gui()
readline()