cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Games_Engineering)

#### Setup Directories ####
# Main output directory
set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#### Practical 1 ####
file(GLOB_RECURSE SOURCES practical_1/*.cpp practical_1/*.h)
add_executable(PRACTICAL_1 ${SOURCES} "practical_1/Warrior.cpp" "practical_1/Warrior.h")
target_include_directories(PRACTICAL_1 PRIVATE ${SFML_INCS})
target_link_libraries(PRACTICAL_1 sfml-graphics)

#### Asset Handling ####
# Define the source and output paths for the assets
set(ASSET_PATH "${CMAKE_SOURCE_DIR}/assets")
set(OUTPUT_ASSET_PATH "${CMAKE_BINARY_DIR}/output/assets")

# Add a custom command to copy the assets after building the project
add_custom_command(TARGET PRACTICAL_1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSET_PATH} ${OUTPUT_ASSET_PATH}
)
