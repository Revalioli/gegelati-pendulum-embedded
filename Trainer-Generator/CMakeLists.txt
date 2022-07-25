cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME Trainer_Generator)
project(${PROJECT_NAME})

# Build type is released if not specified via command line
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()


set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Link with pthread
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
#add libmath during non visual studio builds
set(CMAKE_EXTRA_LIB m)
# Debug or release
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    MESSAGE("Generate Debug project")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -pg -Wall")
else()
    MESSAGE("Generate Release project")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
endif()

# === GEGELATI ===
find_package(GEGELATI)


# === Executable target ===

add_definitions( -DPROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
        -DRESULT_EXPORT_PATH="./Results"
        -DRESULT_DOTFILE_PATH="./Results/dotfiles"
        )
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Results/dotfiles)
file(GLOB_RECURSE
        files
        ./src/instructions.*
        ./src/pendulum.*
        ./params.json
        )
include_directories(${GEGELATI_INCLUDE_DIRS})

set(TRAIN_TARGET Trainer)
add_executable(${TRAIN_TARGET} src/mainTrainer.cpp ${files})
target_link_libraries(${TRAIN_TARGET} ${GEGELATI_LIBRARIES})

set(GENERATE_TARGET Generator)
add_executable(${GENERATE_TARGET} src/mainGenerator.cpp ${files})
target_link_libraries(${GENERATE_TARGET} ${GEGELATI_LIBRARIES})




# Test variables
message("Project configuration :")
message("Build type : ${CMAKE_BUILD_TYPE}")
message("Source directory : ${CMAKE_CURRENT_SOURCE_DIR}")
message("Binary directory : ${CMAKE_BINARY_DIR}")
