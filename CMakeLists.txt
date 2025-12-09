cmake_minimum_required(VERSION 3.10)
project(aoc25 VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Function to add a day's executable
function(add_day DAY_NUM)
    set(TARGET_NAME day${DAY_NUM})
    if(EXISTS ${PROJECT_SOURCE_DIR}/src/day${DAY_NUM}.cpp)
        add_executable(${TARGET_NAME} src/day${DAY_NUM}.cpp)
    endif()
endfunction()

# Add executables for each day (1-25)
foreach(DAY RANGE 1 25)
    add_day(${DAY})
endforeach()

# Optional: Add a main executable for testing
if(EXISTS ${PROJECT_SOURCE_DIR}/src/main.cpp)
    add_executable(${PROJECT_NAME} src/main.cpp)
endif()
