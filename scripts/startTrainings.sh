checkExitStatus() {
    code=$?
    if [[ $code -ne 0 ]]
    then
        echo "Something wrong hapenned, last exit code is $code, exiting script"
        exit 1
    fi
}

SCRIPT_DIR=$(cd $(dirname "${BASH_SOURCE[0]}") && pwd)
cd $SCRIPT_DIR/../Trainer-Generator

# CMake configuration
rm -r bin/*
cd bin
cmake ..
checkExitStatus
cd ..

# For every directory in Trainer-Generator/to_train ...
dirs=$(ls to_train)
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

    cd Release  # Train-Generator/bin/Release
    ./Trainer
    checkExitStatus

    # Save results
    cp -R Results ../../to_train/$d
    checkExitStatus
    rm Results/dotfiles/*
    rm Results/*.*

    cd ../..    # Train-Generator

done
