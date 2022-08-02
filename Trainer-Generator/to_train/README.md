For each TPG you wish to train with the Trainer target, create a subdirectory with :
* **instructions.cpp** with the instructions you want to add to the instruction set (see gegelati-apps/pendulum for the template of the file)
* **params.json** with the parameters for the training

Then execute `start_trainings.sh` (seed the script directory).
Trainings results will then be geneated in a Result subdirectories.