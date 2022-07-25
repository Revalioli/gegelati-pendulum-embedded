#include <iostream>
#include <fstream>
#include <ctime>
#include <gegelati.h>

#include "pendulum.h"
#include "instructions.h"


int main() {
    std::cout << "=====[ Trainer target ]=====" << std::endl << std::endl;

    std::cout << "Environment initialisation" << std::endl;

    /* Settings */

    // Instruction set
    Instructions::Set set;
    fillInstructionSet(set);
    // Learning parameters
    Learn::LearningParameters params;
    File::ParametersParser::loadParametersFromJson(PROJECT_ROOT_PATH "/params.json", params);

    /* Trainings objects */

    // Learning environment
    Pendulum pendulumLE({ 0.05, 0.1, 0.2, 0.4, 0.6, 0.8, 1.0 });
    // Learning agent
    size_t seed = time(nullptr);
    Learn::ParallelLearningAgent la(pendulumLE, set, params);
    la.init(seed);

    /* Log and output setup */

    // Log
    std::ofstream trainLogFile(RESULT_EXPORT_PATH "/train.log");
    Log::LABasicLogger trainLog(la, trainLogFile);
    Log::LABasicLogger consoleLog(la);
    // Create an exporter for graphs
    File::TPGGraphDotExporter dotExporter(RESULT_DOTFILE_PATH "/out_0000.dot", *la.getTPGGraph());
    // Logging best policy stat.
    std::ofstream stats;
    stats.open(RESULT_EXPORT_PATH "/bestPolicyStats.md");
    Log::LAPolicyStatsLogger policyStatsLogger(la, stats);
    // Export used parameters
    File::ParametersParser::writeParametersToJson(RESULT_EXPORT_PATH "/exported_params.json", params);


    /* Training */
    std::cout << "Starting training for " << params.nbGenerations << " generations" << std::endl;

    for (int i = 0; i < params.nbGenerations; i++){
        // Setting dot exporter for generation graph
        char buff[100];
        sprintf(buff, RESULT_EXPORT_PATH "/dotfiles/out_%04d.dot", i);
        dotExporter.setNewFilePath(buff);
        dotExporter.print();

        la.trainOneGeneration(i);
    }

    std::cout << "End training, generating data on best policy" << std::endl;

    // Remove all root except the best one
    la.keepBestPolicy();
    // Remove all introns from the TPG
    la.getTPGGraph()->clearProgramIntrons();

    // Export the final graph
    dotExporter.setNewFilePath(RESULT_EXPORT_PATH "/out_best.dot");
    dotExporter.print();

    // Export topology stats on the winning root
    TPG::PolicyStats ps;
    ps.setEnvironment(la.getTPGGraph()->getEnvironment());
    ps.analyzePolicy(la.getBestRoot().first);
    std::ofstream bestStats;
    bestStats.open(RESULT_EXPORT_PATH "/out_best_stats.md");
    bestStats << ps;
    bestStats.close();
    stats.close();

    // Delete instructions
    for (unsigned int i = 0; i < set.getNbInstructions(); i++) {
        delete (&set.getInstruction(i));
    }

    trainLogFile.close();

    std::cout << "End program" << std::endl;

    return 0;
}
