
function displayInBrowser(p::Union{Plot, PlotlyJS.SyncPlot})
    open("/tmp/plotly_view.html", "w") do io
        PlotlyBase.to_html(io, p)
    end

    run(`xdg-open /tmp/plotly_view.html`)
end

function displayInBrowser(html_path::String)
    run(`xdg-open $html_path`)
end


"Return a new plot with the energy measures data of a TPG."
function plotEnergyData(jsonDataPath::String)

    measuresDataJson = JSON3.read(read(jsonDataPath))

    yTick = measuresDataJson["metadata"]["dataTimeMultiplier"]
    yUnit = measuresDataJson["metadata"]["dataTimeUnit"]

    stepData = copy(measuresDataJson["step"])
    currentData  = copy(measuresDataJson["current"])
    powerData = copy(measuresDataJson["power"])

    # Remove meaningless data (step[i] == -1)
    toRemove = findall(==(-1), stepData)
    deleteat!(stepData, toRemove)
    deleteat!(currentData, toRemove)
    deleteat!(powerData, toRemove)

    # Set current in mA
    currentData.*= 1000

    xAxisValues = [i*yTick for i in 0:length(stepData)-1]


    # ===[ Plotting ]===

    plotCurrent = Plot(
        
        [
            scatter(x = xAxisValues, y = currentData, name="Current (mA)", modes="lines", line_color="red", yaxis="y1"), 
            scatter(x = xAxisValues, y = powerData, name="Power (W)", modes="lines", line_color="green", yaxis="y2"),
            scatter(x= xAxisValues, y = stepData, name="Inference n°", modes="lines", line_color="blue", yaxis="y3")
        ],

        Layout(

            title="Energy consumption",
            
            xaxis_title="Execution time ($yUnit)",
            xaxis_domain=[0.0, 0.8],
            xaxis_range=(-2, xAxisValues[end] + 2),

            yaxis_title="Current (mA)",
            yaxis_title_color="red",
            yaxis_title_font_color="red",
            yaxis_tickfont_color="red",
            yaxis_range=(0, 30),

            yaxis2=attr(
                title_text="Power (W)",
                title_color="green",
                side="right",
                overlaying="y",

                range = (0, 0.2),

                title_font_color="green",
                tickfont_color="green",
            ),

            yaxis3=attr(
                title_text="Inference n°",
                title_color="blue",
                side="right",
                position=0.9,
                anchor="free",
                overlaying="y",

                title_font_color="blue",
                tickfont_color="blue",
            )
        ) 
    )

end

function plotExecutionData(jsonDataPath::String)

    data = JSON3.read(read(jsonDataPath))

    # =====[ Averages ]======

    avgStats = data["AverageStats"]
    field_texts = ["Average evaluated programs", "Average evaluated teams", "Average executed lines"]
    field_keys = ["avgEvaluatedPrograms", "avgEvaluatedPrograms", "avgEvaluatedPrograms"]

    table_averages = table(
        cells_values = [
            field_texts,
            [avgStats[d] for d in field_keys]
        ],
    )

    instruction_averages = table(
        header_values = [ "Instruction index", "Average executions"],
        cells_values = [
            keys(avgStats["avgNbExecutionPerInstruction"]),
            [ values(avgStats["avgNbExecutionPerInstruction"])... ]
        ],
    )

    statsPlot = make_subplots(rows=2, cols=2, specs = [Spec(kind="domain") missing; Spec(kind="domain") missing] );
    add_trace!(
        statsPlot,
        table_averages,
        row=1, col=1
    )

    add_trace!(
        statsPlot,
        instruction_averages,
        row=2, col=1
    )

    statsPlot


end