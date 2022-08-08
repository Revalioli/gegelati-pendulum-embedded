# Scripts

Use of these scripts is automated by the `startAllEnergyBenches.rb` script.


## `start_training.sh`
Start TPG trainings with the Trainer-Genrator project (read corresponding README.md for more instructions).

## `generate_TPG.sh`
Generate CodeGen files for every TPGs in TPG/ directory.

## `create_plots.jl`
Provide julia functions to generate plots with PlotlyJS.

## `generate_energy_plots.jl`
For each TPG, generate plots for the energy measurement curves and execution statistics.

## `generate_execution_summary.jl`
Generate a summary of the average execution stats for all TPGs in TPG/ for comparison.

## `logToJson.rb`
Provide a function to parse energy.log files into a json file.

## `install_julia_packages.jl`
Setup the julia project files and install julia packages required by julia scripts.


