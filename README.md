# Advent of Code 2025

My solutions for [Advent of Code 2025](https://adventofcode.com/2025) in C++.

## Project Structure

```
aoc25/
├── CMakeLists.txt       # CMake build configuration
├── src/                 # Solution files
│   ├── day1.cpp        # Day 1 solution
│   ├── day2.cpp        # Day 2 solution
│   └── ...             # etc.
├── inputs/             # Input files
│   ├── day1.txt        # Day 1 input
│   ├── day2.txt        # Day 2 input
│   └── ...             # etc.
├── include/            # Header files (utility functions)
└── README.md           # This file
```

## Prerequisites

- CMake 3.10 or higher
- A C++17 compatible compiler (GCC, Clang, or MSVC)

## Building the Project

```bash
# Configure and build all solutions
cmake -B build
cmake --build build
```

## Running Solutions

```bash
# Run a specific day (e.g., day 1)
./build/day1

# Or with input file as argument
./build/day1 inputs/day1.txt
```

## Adding a New Day

1. Create `src/dayX.cpp` (e.g., `src/day1.cpp`)
2. Add your input to `inputs/dayX.txt` (e.g., `inputs/day1.txt`)
3. Rebuild: `cmake --build build`
4. Run: `./build/dayX`

## Development Tips

- Use `include/` for shared utility functions (parsing, algorithms, etc.)
- Each day's solution is a standalone executable
- CMake automatically detects new `dayX.cpp` files
- Input files are typically read from `inputs/dayX.txt`

## Clean Build

```bash
rm -rf build
```
# aoc25
