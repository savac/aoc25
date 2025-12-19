#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <unordered_set>
#include "utils.h"

using ll = long long;
using coord = std::array<ll, 3>;

const std::string SOLUTION_DAY = "8";

std::vector<coord> read_positions(const std::vector<std::string> &lines) {
    std::vector<coord> positions;
    for (const auto &line : lines) {

        if (line.empty()) {
            break;
        }

        // std::cout << line << std::endl;
        std::vector<std::string> position_str = aoc::split(line, ',');
        coord position = {
            std::stoll(position_str[0]), 
            std::stoll(position_str[1]), 
            std::stoll(position_str[2])};
        positions.push_back(position);
    }
    return positions;
}

double distance(std::vector<coord> &positions, int i, int j) {
    return std::sqrt(
        std::pow(positions[i][0] - positions[j][0], 2) +
        std::pow(positions[i][1] - positions[j][1], 2) +
        std::pow(positions[i][2] - positions[j][2], 2)
    );
}

std::pair<ll, ll> find_circuits(std::vector<coord> &positions, int top_pairs) {
    std::vector<std::pair<std::pair<int, int>, double>> pairs;

    for (size_t i = 0; i < positions.size() - 1; ++i) {
        for (size_t j = i + 1; j < positions.size(); ++j) {

            double this_distance = distance(positions, i, j);
            pairs.push_back({{i, j}, this_distance});
        }
    }

    // sort by the second element of the pair
    std::sort(pairs.begin(), pairs.end(), [](const auto &a, const auto &b) {
        return a.second < b.second;
    });

    // use -1 for part 2 to continue until the end
    if (top_pairs > -1 && top_pairs < static_cast<int>(pairs.size()))
        pairs.resize(top_pairs);

    std::vector<std::unordered_set<int>> circuits;

    // for (const auto &p : pairs) {
    //     std::string row;
    //     std::cout << '(' + std::to_string(p.first.first) + ',' + std::to_string(p.first.second) + ')' << std::endl;
    // }


    // we'll work with position indices now, denoted by a and b
    // distribute them in connected circuits
    ll part2_res = 0;
    for (const auto &this_pair : pairs) {

        int ia = -1;
        int ib = -1;
        int a = this_pair.first.first;
        int b = this_pair.first.second;

        // find out if any of the pair elements are in an existing circuit
        for (size_t i = 0; i < circuits.size(); ++i) {
            if (circuits[i].find(a) != circuits[i].end()) {
                ia = i;
                break;
            }
        }
        for (size_t i = 0; i < circuits.size(); ++i) {
            if (circuits[i].find(b) != circuits[i].end()) {
                ib = i;
                break;
            }
        }

        // if neither was found
        if (ia == -1 && ib == -1) {
            circuits.push_back(std::unordered_set<int>{a, b});
        } 
        // a in existing circuit, b isn't
        else if (ia > -1 && ib == -1) {
            circuits[ia].insert(b);
        }
        // vice versa
        else if (ia == -1 && ib > -1) {
            circuits[ib].insert(a);
        }
        // both in existing circuits
        else if (ia > -1 && ib > -1) {
            if (ia != ib) {
                circuits[ia].insert(circuits[ib].begin(), circuits[ib].end());
                circuits.erase(circuits.begin() + ib);
            }
        } 
        else {
            std::cerr << "Should not be here." << std::endl;
        }

        // std::cout << std::to_string(a) + ',' + std::to_string(b) + ',' + std::to_string(ia) + ',' + std::to_string(ib) << std::endl;
        // std::cout << "all sets" << std::endl;

        // for (const auto &circuit : circuits) {
        //     std::string row;
        //     for (const auto &el : circuit) {
        //         row += std::to_string(el) + ',';
        //     }
        //     std::cout << row << std::endl;
        // }
        // std::cout << "===" << std::endl;

        // stop when there is only one left and that it contains all positions
        if (circuits.size() == 1 && circuits[0].size() == positions.size()) {
            part2_res = positions[a][0] * positions[b][0];
            break;
        }
    }


    std::vector<int> circuit_sizes;
    for (const auto &this_circuit : circuits) {
        circuit_sizes.push_back(this_circuit.size());
    }

    std::sort(circuit_sizes.begin(), circuit_sizes.end(), [](const auto &a, const auto &b) {
        return a > b;
    });

    circuit_sizes.resize(3);
    

    ll part1_res = 1LL * circuit_sizes[0] * circuit_sizes[1] * circuit_sizes[2];

    return {part1_res, part2_res};
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

        ll part1_result = find_circuits(positions, 1000).first;
        auto part1_end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> part1_elapsed = part1_end - part1_start;
        
        std::cout << "Part 1: " << part1_result << " in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;

        // part 2
        auto part2_start = std::chrono::high_resolution_clock::now();

        ll part2_result = find_circuits(positions, -1).second;

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
