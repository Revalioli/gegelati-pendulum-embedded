
function displayInBrowser(p::Plot)
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

"Return a tuple with two plots displaying the executions statistics provided."
function plotExecutionData(jsonDataPath::String)

    data = JSON3.read(read(jsonDataPath))
    execStats = data["ExecutionStats"]

    # =====[ Traces ]======

    # === For Topology plot ===

    field_texts = ["Average evaluated teams", "Average evaluated programs", "Average executed lines"]
    field_keys = ["avgEvaluatedTeams", "avgEvaluatedPrograms", "avgExecutedLines"]

    table_averages = table(
        cells_values = [
            field_texts,
            [execStats[d] for d in field_keys]
        ],
    )

    instIdx = keys(execStats["distributionUsedVertices"])
    distrib_visited_teams = bar(
        name = "Number of inferences where each vertex was used",
        x = instIdx,
        y = [execStats["distributionUsedVertices"][i] for i in instIdx],
    )

    instIdx = keys(execStats["distributionEvaluatedTeams"])
    distrib_evaluated_teams = bar(
        name = "Number of inferences per number of evaluated teams",
        x = instIdx,
        y = [execStats["distributionEvaluatedTeams"][i] for i in instIdx],
    )

    instIdx = keys(execStats["distributionEvaluatedPrograms"])
    distrib_evaluated_programs = bar(
        name = "Number of inferences per number of evaluated programs",
        x = instIdx,
        y = [execStats["distributionEvaluatedPrograms"][i] for i in instIdx],
    )

    instIdx = keys(execStats["distributionExecutedLines"])
    distrib_executed_lines = bar(
        name = "Number of inferences per number of executed lines",
        x = instIdx,
        y = [execStats["distributionExecutedLines"][i] for i in instIdx],
    )



    # === For Instruction plot ===

    table_instruction_averages = table(
        header_values = [ "Instruction index", "Average executions"],
        cells_values = [
            keys(execStats["avgNbExecutionPerInstruction"]),
            [ values(execStats["avgNbExecutionPerInstruction"])... ]
        ],
    )

    # Determinating the best layout for all instruction distributions
    nbDist = length(execStats["distributionNbExecutionPerInstruction"])

    square = sqrt(nbDist)
    nbRow = floor(Integer ,square)  # Number of rows of the plot
    nbCol = nbRow + 1   # Number of rows of the plot, must include the column for th average stats

    if (square - nbRow) != 0
        nbCol += 1
        if (square - nbRow) > 0.5
            nbRow += 1
        end
    end

    bar_instructions = []   # Traces for all instructions distributions
    bar_instructions_titles = []
    for (instruction_idx, dist_object) in execStats["distributionNbExecutionPerInstruction"]

        pairs = [(key, val) for (key, val) in dist_object]

        trace = bar(
            name = instruction_idx,
            x = [key for (key, _) in pairs],
            y = [val for (_, val) in pairs]
        )
        push!(bar_instructions, trace)
        push!(bar_instructions_titles, "Instruction $instruction_idx")
    end


    # =====[ Plots ]=====

    # 2 Plots are generated :
    # - Topology plot : averages stats, distributions of teams, programs and lines 
    # - Instruction plot : average stats for each instruction, distributions on instruction uses
    
    # === For Topology plot ===

    topologyPlot = make_subplots(
        rows=3, cols=2,
        specs = [
            Spec(kind="domain", colspan=2)  missing
            Spec(kind="xy")                 Spec(kind="xy")
            Spec(kind="xy")                 Spec(kind="xy")
        ],
        row_heights = [0.45, 0.45, 0.1]
    );

    add_trace!(
        topologyPlot,
        table_averages,
        row=1, col=1
    )
    add_trace!(
        topologyPlot,
        distrib_visited_teams,
        row=2, col=1
    )
    add_trace!(
        topologyPlot,
        distrib_evaluated_teams,
        row=2, col=2
    )
    add_trace!(
        topologyPlot,
        distrib_evaluated_programs,
        row=3, col=1
    )
    add_trace!(
        topologyPlot,
        distrib_executed_lines,
        row=3, col=2
    )

    relayout!(
        topologyPlot,
        title_text = "Execution Stats"
    )


    # === For Instruction plot ===

    specsArr = [
        [Spec(kind="domain", rowspan=nbRow); fill(missing, nbRow-1)] fill(Spec(kind="xy"), (nbRow, nbCol)) 
    ]

    averageColumnWidth = 0.1    # Relative width of the instruction average stats on the left

    # subplot_title is row-ordered (but specs is column order) and just don't count for missing places in the subplot grid
    # This is completly stupid ...
    # If one day they fix their doc or their module, maybe this will need to be corrected
    subplot_titles_matrix = fill("", (nbRow, nbCol))
    subplot_titles_matrix[1] = "This is a table"
    for i in 1:length(bar_instructions_titles)
        subplot_titles_matrix[i+1] =  bar_instructions_titles[i]
    end

    instructionPlot = make_subplots(
        rows = nbRow, cols = nbCol,
        column_widths = [averageColumnWidth; fill( (1-averageColumnWidth) / (nbCol-1) , nbCol-1)],
        specs = specsArr,
        
        subplot_titles = subplot_titles_matrix
    );
    
    add_trace!(
        instructionPlot,
        table_instruction_averages,
        row=1, col=1
    )

    for i in 0:(length(bar_instructions)-1)
        add_trace!(
            instructionPlot,
            bar_instructions[i+1],
            row = (i ÷ (nbCol-1)) + 1, col = (i % (nbCol-1)) + 2
        )
    end

    relayout!(
        instructionPlot,
        title_text = "Instruction stats",
        show_legend = false,
    )

    (topologyPlot, instructionPlot)
    
end