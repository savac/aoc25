#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <numeric>
#include <chrono>
#include <map>
#include <random>
#include <algorithm>
#include <cassert>
#include <sstream>
#include "utils.h"

using ll = long long;

const std::string SOLUTION_DAY = "7";


std::pair<ll, ll> find_paths(std::vector<std::string> &data) {
    ll splits = 0;

    std::vector<std::string> beams;
    beams.push_back(data[0]);

    // forwards pass, count the splits
    for (size_t i = 1; i < data.size(); ++i) {
        auto row = data[i];
        for (size_t j = 0; j < row.size(); ++j) {
            if ((beams[i - 1][j] == 'S' || beams[i - 1][j] == '|')) {
                if (row[j] == '^') {
                    // split
                    if (j > 0)
                        row[j - 1] = '|';
                    if (j < row.size() - 1)
                        row[j + 1] = '|';
                    splits += 1;
                } else {
                    // propagate
                    row[j] = '|';
                }
            }
        }
        beams.push_back(row);
    }

    // debug
    // for (const auto &row : beams) {
    //     for (const auto &ch : row) {
    //         std::cout << ch;
    //     }
    //     std::cout << std::endl;
    // }

    // backwards pass
    std::vector<ll> accumulator;
    for (const auto &ch : beams.back()) {
        if (ch == '|') {
            accumulator.push_back(1);
        } else {
            accumulator.push_back(0);
        }
    }

    // debug
    // std::string row;
    // for (auto &n : accumulator) {
    //     row += std::to_string(n);
    // }
    // std::cout << row << std::endl;

    for (int i = beams.size() - 2; i >= 0; --i) {
        std::vector<ll> new_accumulator(accumulator.size(), 0);
        for (size_t j = 0; j < accumulator.size(); ++j) {
            if (accumulator[j] > 0) {
                if (j > 0) {
                    // if not left wall
                    if (beams[i].substr(j - 1, 2) == "^|") {
                        new_accumulator[j - 1] += accumulator[j];
                    } 
                }
                if (j < accumulator.size() - 2) {
                    // if not right wall
                    if (beams[i].substr(j, 2) == "|^") {
                        new_accumulator[j + 1] += accumulator[j];
                    } 
                }
                if (beams[i][j] == '|' || beams[i][j] == 'S') {
                    new_accumulator[j] += accumulator[j];
                }
            }
        }
        accumulator = new_accumulator;

        // debug
        // row = "";
        // for (auto &n : accumulator) {
        //     row += std::to_string(n);
        // }
        // std::cout << row << std::endl;
    }

    ll paths = std::accumulate(accumulator.begin(), accumulator.end(), 0LL);

    return {splits, paths};
}


int main(int argc, char *argv[])
{
    std::string filename = (argc > 1) ? argv[1] : "inputs/day" + SOLUTION_DAY + ".txt";
    try
    {
        std::cout << "Day " + SOLUTION_DAY + " Solution" << std::endl;
        auto lines = aoc::read_lines(filename);

        auto part1_start = std::chrono::high_resolution_clock::now();

        auto res = find_paths(lines);

        ll part1_result = res.first;
        auto part1_end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> part1_elapsed = part1_end - part1_start;
        
        std::cout << "Part 1: " << part1_result << " in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;

        // part 2
        auto part2_start = std::chrono::high_resolution_clock::now();

        ll part2_result = res.second;

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
