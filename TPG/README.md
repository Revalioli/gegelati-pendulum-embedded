# TPG directory

This directory contains the TPGs you want to evaluate for energy consumption, as well as evaluation results.

A valid TPG directory tree has a structure as follows :

* TPG/
    * *Name for your TPG directory*/
        * CodeGen/
            * out_best.dot **(required)** --> dot file of the TPG
            * instructions.cpp **(required)** --> provides a function `void fillInstructionSet(Instructions::Set& set)` which will add all instructions used to train the TPG to an `Instruction::Set` object
            * params.json **(required)** --> training parameters of the TPG
            * pendulum.c/.h, pendulum_program.c/.h *(generated)* --> codegen files of the TPG
            * PendulumEmbeddedMeasures.elf *(generated)*
            * \+ additional files you would like to put here (like TPG training results)
        * *_results/ *(generated)* --> for each energy measurement
            * energy.log    --> STM32 board output to PC
            * energy_data.json  --> enegy measurements results (extracted from energy.log)
            * executionStats.json --> execution statistics of the TPG during the energy measurement
            * graph.png/.html --> current and power curves of the energy measurement
            * execution_stats.html / instructions_stats.html --> plots for energy_data.json and executionStats.json
        * \+ additional files you would like to put here 
