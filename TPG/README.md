# TPG directory

This directory contains the CodeGen TPGs you want to evaluate for energy consumption, as well as evaluation results.

A valid TPG directory tree has a structure as follows :

* TPG/
    * *Name for your TPG directory*/
        * CodeGen/
            * out_best.dot  --> dot file of the evaluated TPG
            * pendulum.c/.h, pendulum_program.c/.h **(required)** --> codegen file of the evaluated TPG
            * PendulumEmbeddedMeasures.elf **(generated)**
            * + additional files you would like to put here (like TPG training results)
        * *Date_and_time*_results/ **(generated)**
            * energy.log    --> STM32 board output to PC
            * energy_data.json  --> enegy measurements results (extracted from energy.log)
            * energy_stats.json --> additional energy statistics (computed from energy_data.json)
            * graph.png --> energy measurements results in a graph
        * + additional files you want to put there 

Directories without the required files are ignored.
