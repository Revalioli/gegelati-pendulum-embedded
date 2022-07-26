#include <iostream>
#include <fstream>
#include <filesystem>
#include <gegelati.h>

#include "instructions.h"


int main(int argc, char *argv[]) {
    std::cout << "=====[ Generator target ]=====" << std::endl << std::endl;

    /*
     * Program require 1 arguments :
     * - argv[1] : path to the TPG .dot file
     *
     * This program also require that the associated params.json file is accessible
     * in the same directory as the .dot file, and that instructions.cpp was
     * compiled with the same instruction set used for training the TPG in the .dot file.
     */

    if(argc != 2){
        std::cerr << "Wrong number of arguments, the TPG .dot file path is required." << std::endl;
        exit(1);
    }

    std::filesystem::path dotPath(argv[1]);

    /* Settings */

    // Instruction set
    Instructions::Set set;
	fillInstructionSet(set);
    // Parameters
    Learn::LearningParameters params;
    File::ParametersParser::loadParametersFromJson(
            (dotPath.parent_path().string() + "/params.json").c_str(),
            params
            );

    /* Setup TPG environment for import */

    std::cout << "Importing .dot file" << std::endl;

    // Placeholder DataHandler for creating the Environment object
    Data::PrimitiveTypeArray<double> currentState{ 2 };
    std::vector<std::reference_wrapper<const Data::DataHandler>> data = { currentState };
    // Setup TPG::Graph where to import the TPG
    Environment dotEnv(set, data, params.nbRegisters, params.nbProgramConstant);
    TPG::TPGGraph dotGraph(dotEnv);
    // Import TPG from .dot file
    File::TPGGraphDotImporter dot(dotPath.c_str(), dotEnv, dotGraph);
    dot.importGraph();

    /* Code Generation */

    std::cout << "CodeGen files generation" << std::endl;

    CodeGen::TPGGenerationEngineFactory factory(CodeGen::TPGGenerationEngineFactory::switchMode);
    std::unique_ptr<CodeGen::TPGGenerationEngine> tpggen = factory.create(
            "pendulum",
            dotGraph,
            dotPath.parent_path().string() + "/"
            );
    tpggen->generateTPGGraph();

    std::cout << "End program" << std::endl;

    return 0;
}
