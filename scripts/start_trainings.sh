#!/usr/bin/bash

# Start trainings for every parameters in subdirectories of Trainer-Generator/to_train.
# See the corresponding README.md in Trainer-Generator/to_train

checkExitStatus() {
    code=$?
    if [[ $code -ne 0 ]]
    then
        printf "\033[1;91mERROR\033[0m --- Something wrong hapenned, last exit code is $code, exiting script\n"
        exit 1
    fi
}

SCRIPT_DIR=$(cd $(dirname "${BASH_SOURCE[0]}") && pwd)
cd $SCRIPT_DIR/../Trainer-Generator     # Train-Generator
checkExitStatus

# CMake configuration
rm -r bin
mkdir -p bin
cd bin
cmake ..
checkExitStatus
cd ..

# For every directory in Trainer-Generator/to_train ...
dirs=$(ls to_train --ignore=README.md)
for d in $dirs
do
    # ... copy the necessary files and start the training

    cp to_train/$d/instructions.cpp src/
    checkExitStatus
    cp to_train/$d/params.json .
    checkExitStatus

    cd bin  # Train-Generator/bin

    cmake --build . --target Trainer
    checkExitStatus
    echo

    cd Release  # Train-Generator/bin/Release
    ./Trainer
    checkExitStatus
    echo

    # Save results
    cp -R Results ../../to_train/$d
    checkExitStatus
    rm Results/dotfiles/*
    rm Results/*.*

    cd ../..    # Train-Generator

done
