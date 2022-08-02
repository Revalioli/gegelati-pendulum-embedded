#!/usr/bin/bash

# Generate the CodeGen files for all pendulum TPG that are in subfolders of the TPG directory.
# This script uses the Generator C++ target of the Trainer-Generator directory.

checkExitStatus() {
    code=$?
    if [[ $code -ne 0 ]]
    then
        printf "\033[1;91mERROR\033[0m --- Something wrong hapenned, last exit code is $code, exiting script\n"
        exit 1
    fi
}

checkValidDirectory(){
    local d=$1

    [ $# -ne 0 ] && [ -d "$d" ] && 
        ! [ -f "$d/CodeGen/pendulum.c" ] && ! [ -f "$d/CodeGen/pendulum.h" ] && ! [ -f "$d/CodeGen/pendulum_program.c" ] && ! [ -f "$d/CodeGen/pendulum_program.h" ] &&
        [ -f "$d/CodeGen/out_best.dot" ] && [ -f "$d/CodeGen/instructions.cpp" ] && [ -f "$d/CodeGen/params.json" ]

    return  # Last evaluated logical expression stored its "result code" in $?, which is returned by return
}


SCRIPT_DIR=$(cd $(dirname "${BASH_SOURCE[0]}") && pwd)
cd $SCRIPT_DIR/../TPG     # TPG/

# CMake configuration
rm -r ../Trainer-Generator/bin
mkdir ../Trainer-Generator/bin
cmake -S ../Trainer-Generator -B ../Trainer-Generator/bin
checkExitStatus

dirs=$(ls)
for d in $dirs
do

    if (checkValidDirectory $d)
    then
        printf "\033[1;36mGenerating CodeGen for $d\n\033[0m"

        # Using Generator target of the Trainer-Genertor C++ project

        # Copying instructions.cpp
        cp $d/CodeGen/instructions.cpp ../Trainer-Generator/src/
        checkExitStatus

        # Building target
        cmake --build ../Trainer-Generator/bin --target Generator
        checkExitStatus
        echo

        # Do code generation
        ../Trainer-Generator/bin/Release/Generator $d/CodeGen/out_best.dot
        checkExitStatus
        echo

        printf "\033[1;36mSuccessful code generation for $d\n\033[0m"
    elif [ -d $d ]
    then
        printf "\033[1;36mSkip $d (not all required files or TPG already generated)\n\033[0m"
    fi

done