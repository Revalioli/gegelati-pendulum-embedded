# This file regroups different functions to create PlotlyJS plots from data in
# the energy measurements results and execution stats results files.
#
# Import these in a julia script by including this file.


# ===[ Utilities ]===

"""
    displayInBrowser(p::Plot)

Display a PlotlyJS Plot in the default web-browser (use xdg-open command).
"""
function displayInBrowser(p::Plot)
    open("/tmp/plotly_view.html", "w") do io
        PlotlyBase.to_html(io, p)
    end

    run(`xdg-open /tmp/plotly_view.html`)
end

"""
    displayInBrowser(html_path::String)

Open an html file in the default web browser (use xdg-open command).
"""
function displayInBrowser(html_path::String)
    run(`xdg-open $html_path`)
end

"""
    sortSymbolsAsInt!(a::AbstractVector{Symbol})

Sort a Vector of Symbols in ascending order by casting each Symbol to Int.
"""
function sortSymbolsAsInt!(a::AbstractVector{Symbol})

    sort!(a,lt = (x,y) -> parse(Int, string(x)) < parse(Int, string(y)))

end


# ===[ Plot creation ]===


"""
    plotEnergyData(jsonDataPath::String)

Return a Plot object using the energy measures data json file of a TPG.
"""
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

    timeAxis = [i*yTick for i in 0:length(stepData)-1]


    # ===[ Plotting ]===

    Plot(
        
        [
            scatter(x = timeAxis, y = currentData, name="Current (mA)", modes="lines", line_color="red", yaxis="y1"), 
            scatter(x = timeAxis, y = powerData, name="Power (W)", modes="lines", line_color="green", yaxis="y2"),
            scatter(x= timeAxis, y = stepData, name="Inference n°", modes="lines", line_color="blue", yaxis="y3")
        ],

        Layout(

            title="Energy consumption",
            
            xaxis_title="Execution time ($yUnit)",
            xaxis_domain=[0.0, 0.8],
            xaxis_range=(-2, timeAxis[end] + 2),

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


"""
    plotExecutionData(jsonDataPath::String)

Return a tuple with two SyncPlot displaying the executions statistics provided in the json file.
"""
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

    instIdx = keys(execStats["distributionUsedVertices"]) |> collect |> sortSymbolsAsInt!
    distrib_visited_teams = bar(
        x = instIdx,
        y = [execStats["distributionUsedVertices"][i] for i in instIdx],
    )

    instIdx = keys(execStats["distributionEvaluatedTeams"]) |> collect |> sortSymbolsAsInt!
    distrib_evaluated_teams = bar(
        x = instIdx,
        y = [execStats["distributionEvaluatedTeams"][i] for i in instIdx],
    )

    instIdx = keys(execStats["distributionEvaluatedPrograms"]) |> collect |> sortSymbolsAsInt!
    distrib_evaluated_programs = bar(
        x = instIdx,
        y = [execStats["distributionEvaluatedPrograms"][i] for i in instIdx],
    )

    instIdx = keys(execStats["distributionExecutedLines"]) |> collect |> sortSymbolsAsInt!
    distrib_executed_lines = bar(
        x = instIdx,
        y = [execStats["distributionExecutedLines"][i] for i in instIdx],
    )



    # === For Instruction plot ===

    instIdx = keys(execStats["avgNbExecutionPerInstruction"]) |> collect
    sort!(instIdx, by = x -> execStats["avgNbExecutionPerInstruction"][x], rev=true)  # Sort instructions in descending order of average number of executions


    table_instruction_averages = table(
        header_values = [ "Instruction index", "Average executions"],
        cells_values = [
            instIdx,
            [ execStats["avgNbExecutionPerInstruction"][i] for i in instIdx ]
        ],
    )

    bar_instructions = []   # Traces for all instructions distributions
    for (instruction_idx, dist_object) in execStats["distributionNbExecutionPerInstruction"]

        idx = keys(dist_object) |> collect

        trace = bar(
            name = instruction_idx,
            x = idx,
            y = [dist_object[val] for val in idx],
        )
        push!(bar_instructions, trace)

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
        row_heights = [0.45, 0.45, 0.1],
        vertical_spacing = 0.07,
        horizontal_spacing = 0.04,
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
        title_text = "Execution Stats",
        showlegend = false,

        # Axis are numbered, meaning that the 5th x axis has an attribute named "xaxis5" 
        # which can be used to change the axis attribute like any other axis (same idea with yaxis_).
        # The only exception is for the first axis, which has no number (1st x axis = xaxis).
        # Seems to be row ordered.

        xaxis_title = "Team indexes",
        xaxis2_title = "Number evaluated teams",
        xaxis3_title = "Number evaluated programs",
        xaxis4_title = "Number executed lines", 

        yaxis_title = "Number of inferences",
        yaxis2_title = "Number of inferences",
        yaxis3_title = "Number of inferences",
        yaxis4_title = "Number of inferences",

    )


    # === For Instruction plot ===

    instructionPlot = make_subplots(
        rows = 1, cols = 2,
        column_widths = [0.1, 0.9],
        specs = [ Spec(kind="domain") Spec(kind="xy") ],
        vertical_spacing = 0.05,
        horizontal_spacing = 0.05,
        subplot_titles = [missing "Number of inferences per number of execution for each instruction"],
    );


    add_trace!(
        instructionPlot,
        table_instruction_averages,
        row=1, col=1
    )

    for trace in bar_instructions
        # Traces can be stacked in the same subplot
        add_trace!(
            instructionPlot,
            trace,
            row = 1, col = 2,
        )
    end

    relayout!(
        instructionPlot,
        title_text = "Instruction stats",
        xaxis_categoryorder="array",
        xaxis_categoryarray=1:1000,     # TODO assign maximum value found in all bar traces instead of 1000
        xaxis_title = "Number of executions",
        yaxis_title = "Number of inferences",
    )

    (topologyPlot, instructionPlot)
    
end

"""
    plotExecutionSummary(jsonExecutionStatsPaths::Vector{String}, labels::Vector{String})

Plot together data from different execution stats json files for comparison. `jsonExecutionStatsPaths` and `labels`
should have the same length.

Created plots are return in a SyncPlot object.
"""
function plotExecutionSummary(jsonExecutionStatsPaths::Vector{String}, labels::Vector{String})

    if length(jsonExecutionStatsPaths) != length(labels)
        throw(ArgumentError("Vectors jsonExecutionStatsPaths and labels lengths don't match"))
    end

    # Green colorscale
    colorScale = [
        "#3f8125",
        "#4b9a2c",
        "#56b233",
        "#63c73d",
        "#77ce55",
        "#8ad56d",
        "#9edc85",
        "#b1e39e",
        "#c5eab6",
        "#d8f1ce",
        "#ecf8e7",
        "#ffffff"
    ]

    jsonObjects = [ JSON3.read(read(path)) for path in jsonExecutionStatsPaths ]


    barTraces = GenericTrace[]
    for (execData, label) in zip(jsonObjects, labels)

        t = bar(
            x = [ "Avg visited teams", "Avg evaluated programs", "Avg executed lines" ],
            y = [ 
                execData["ExecutionStats"]["avgEvaluatedTeams"],
                execData["ExecutionStats"]["avgEvaluatedPrograms"],
                execData["ExecutionStats"]["avgExecutedLines"]
            ],
            name = label,
        )

        push!(barTraces, t)

    end

    # println(barTraces)
    println(typeof(barTraces))

    p = plot(barTraces);

    relayout!(
        p,
        title = "Execution Stats Summary",
        colorway = colorScale,
    )

    return p

end

"""
    plotBoxesByCategory(filePaths::Vector{String}, categories::Vector{String}, jsonField::String, transform::Function = identity)

Create a box plot using values of one data field from a set of json files statistics.
Each file is associated a category, which is the box trace where the value will be added.
Typically, the category can be the instruction set used by the TPG that generated the data.  

*The data field must be accessible via nested JSON objects, arrays are not supported.*

# Arguments

- **filePaths[in] :** Vector{String} of all json file paths.
- **category[in] :** Vector{String} the assocaited categories for each path.
- **jsonField[in] :** field of the value to be used in all json file, nested objects fields are separeted with a /
(example : to use the field `["top"]["nested"]` give `"top/nested"` to the function).
- **transform[in] :** optional function applied on each json value before adding it to the plot.

# Example

```julia
p = plotBoxesByCategory(
    [ "/path/to/file_1", "/path/to/file_2", "/path/to/file_3" ],
    [ "cat1", "cat2", "cat1" ],
    "Summary/Number",
    d -> d * 1000
)
```
The resulting SyncPlot p will contain 2 box traces (cat1 and cat2).
Json files must have the following structure :
```json
    ...
    Summary : {
        ...
        Number : {Value},
        ...
    },
    ...
}
```
Before ploting, `Value` will be multiplied by 1000.
The resulting SyncPlot p will have to box traces : cat1 with two points, and cat2 with one point.


# Exceptions thrown
- **ArgumentError** : when `filePaths` and `categories` don't have the same length.

"""
function plotBoxesByCategory(filePaths::Vector{String}, categories::Vector{String}, jsonField::String, transform::Function = identity)

    if length(filePaths) != length(categories)
        throw(ArgumentError("Vectors filePaths and categories lengths don't match"))
    end

    # Organise paths by instructionSets in a Dict
    pathBySet = Dict( zip( categories, [String[] for i in 1:length(categories)] ) )

    for (path, set) in zip(filePaths, categories)
        push!(pathBySet[set], path)
    end

    # Split nested fielf in preparation for call to fold
    jsonPath = split(jsonField, '/')

    box_traces = GenericTrace[]
    for k in keys(pathBySet)

        # Get the data in each json files
        data = pathBySet[k] .|>
            read .|>
            JSON3.read .|>
            jsonData -> foldl( (jObject, field) -> jObject[field], jsonPath, init = jsonData)

        transformed_data = transform.(data) # Apply transformation on data

        t = box(
            y = transformed_data,
            boxpoints = "all",
            name = k,
        )

        push!(box_traces, t)

    end

    plot(box_traces);

end