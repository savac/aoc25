#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include "utils.h"

using ll = long long;
using coord = std::pair<ll, ll>;

const std::string SOLUTION_DAY = "9";

std::vector<coord> read_positions(const std::vector<std::string> &lines) {
    std::vector<coord> positions;
    for (const auto &line : lines) {

        if (line.empty()) {
            break;
        }

        std::vector<std::string> position_str = aoc::split(line, ',');
        coord position = {
            std::stoll(position_str[0]), 
            std::stoll(position_str[1])};
        positions.push_back(position);
    }
    return positions;
}

ll get_area(coord a, coord b) {
    ll width = std::abs(a.first - b.first) + 1; 
    ll height = std::abs(a.second - b.second) + 1;

    return width * height;
}

bool is_valid_rectangle(std::vector<coord> &positions, int i, int j) {

    // define the rectangle
    auto [xmin, xmax] = std::minmax(positions[i].first, positions[j].first);
    auto [ymin, ymax] = std::minmax(positions[i].second, positions[j].second);

    // return false if any segment, [a, b], cuts the rectangle
    for (size_t a = 0; a < positions.size(); ++a) {
        // wrap b
        int b = (a == positions.size() - 1) ? 0 : a + 1;

        // there are 4 half planes defined (in the outwards direction) by the rectangle sides
        // if the segment is in any of these half planes, there is no cut
        // otherwise there must be a cut
        // this only works because we know that the segments are either vertical or horizontal
        bool not_cut = (
            (positions[a].first >= xmax && positions[b].first >= xmax) ||
            (positions[a].first <= xmin && positions[b].first <= xmin) ||
            (positions[a].second >= ymax && positions[b].second >= ymax) ||
            (positions[a].second <= ymin && positions[b].second <= ymin)
        );

        if (!not_cut)
            return false;
    }

    return true;
}

ll find_rectangle(std::vector<coord> &positions, bool part2=false) {
    ll max_area = 0;
    for (size_t i = 0; i < positions.size() - 1; ++i) {
        for (size_t j = i; j < positions.size(); ++j) {
            ll area = get_area(positions[i], positions[j]);

            if (area < max_area) {
                continue;
            }

            if (part2 && !is_valid_rectangle(positions, i, j)) {
                continue;
            }

            max_area = area;
        }
    }

    return max_area;
}


int main(int argc, char *argv[])
{
    std::string filename = (argc > 1) ? argv[1] : "inputs/day" + SOLUTION_DAY + ".txt";
    try
    {
        std::cout << "Day " + SOLUTION_DAY + " Solution" << std::endl;
        auto lines = aoc::read_lines(filename);

        auto positions = read_positions(lines);
        
        auto part1_start = std::chrono::high_resolution_clock::now();

        ll part1_result = find_rectangle(positions);
        auto part1_end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> part1_elapsed = part1_end - part1_start;
        
        std::cout << "Part 1: " << part1_result << " in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;

        // part 2
        auto part2_start = std::chrono::high_resolution_clock::now();

        ll part2_result = find_rectangle(positions, true);

        auto part2_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> part2_elapsed = part2_end - part2_start;
        std::cout << "Part 2: " << part2_result << " in " << part2_elapsed.count() * 1e3 << " ms" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
