#include <iostream>
#include <string>
#include "utils.h"


int rotate(int &position, int &steps, bool &clockwise)
{
    // std::cout << "Rotating from position " << position << " by " << steps << " steps " << (clockwise ? "clockwise" : "counterclockwise") << std::endl;
    int clicks = 0;
    if (clockwise)
    {
        position += steps;
        if (position >= 100) {
            clicks = position / 100;
            position = position % 100;
        }
    }
    else
    {
        if (steps > position) {
            // initial wrap (starting from positive position)
            if (position > 0) {
                clicks = 1;
            }
            position -= steps;
            clicks += (-position) / 100;
        } else if (position == steps) {
            position = 0;
            clicks = 1;
        } else {
            // position > steps
            position -= steps;
        }

        while (position < 0) {
            position += 100;
        }
    }

    // std::cout << "New position: " << position << ", clicks: " << clicks << std::endl;

    return clicks;
}

int main(int argc, char *argv[])
{
    // Default to inputs/day1.txt if no argument provided
    std::string filename = (argc > 1) ? argv[1] : "inputs/day1.txt";

    try
    {
        auto lines = aoc::read_lines(filename);

        std::cout << "Day 1 Solution" << std::endl;
        std::cout << "Read " << lines.size() << " lines from " << filename << std::endl;

        int position = 50;
        int part1_result = 0;
        int part2_result = 0;
        for (const auto &line : lines)
        {
            bool clockwise = (line[0] == 'R');
            int steps = std::stoi(line.substr(1));

            part2_result += rotate(position, steps, clockwise);

            if (position == 0) {
                part1_result += 1;
            }
        }

        std::cout << "Part 1 (stops at 0): " << part1_result << std::endl;
        std::cout << "Part 2 (total 0 clicks): " << part2_result << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
