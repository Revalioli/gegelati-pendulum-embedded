# Train your Pendulum TPGs with the Trainer-Generator project

* For each training, create a subdirectory in `to_train` with the following files :
    * `instructions.cpp` --> provides a function `void fillInstructionSet(Instructions::Set& set)` which will add all instructions used to train the TPG to an `Instruction::Set` object.
    * `params.json` --> the parameters used for the training.
* Execute the script `start_trainings.sh` in the `scripts` directory.
* Trainings results will be geneated in a `Results` subdirectory for each training.
