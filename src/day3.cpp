#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <string_view>
#include <charconv>
#include <set>
#include <numeric>
#include <chrono>
#include <cassert>
#include <map>
#include "utils.h"

long long get_max_joltage(const std::vector<uint> &batteries, std::map<std::pair<int, int>, long long> &battery_map, int num_batteries, int battery_pos=0){

    if (num_batteries == 1) {
        long long max_battery = 0;
        for (size_t i = battery_pos; i < batteries.size(); ++i) {
            if (batteries[i] > max_battery) {
                max_battery = batteries[i];
            }
        }
        return max_battery;
    }

    long long max_joltage = 0;
    int num_unnallocated_batteries = num_batteries - 1;

    for (size_t i = battery_pos; i < batteries.size() - num_unnallocated_batteries; ++i) {

        if (battery_map.find({i + 1, num_unnallocated_batteries}) == battery_map.end()) {
            battery_map[{i + 1, num_unnallocated_batteries}] = get_max_joltage(
                batteries,
                battery_map,
                num_unnallocated_batteries,
                i + 1
            );
        }

        long long joltage = batteries[i] * std::pow(10, num_unnallocated_batteries) + battery_map[{i + 1, num_unnallocated_batteries}];
        if (joltage > max_joltage) {
            max_joltage = joltage;
        }
    }
    return max_joltage;
}

long long get_max_joltage_from_string(std::string_view line, int num_batteries=2) {
    // split the number into digits
    std::vector<uint> batteries;
    // std::cout << "Processing line: " << line << std::endl;
    for (char ch : line) {
        if (std::isdigit(ch)) {
            batteries.push_back(std::stoi(std::string(1, ch)));
        }
    }

    std::map<std::pair<int, int>, long long> battery_map;
    long long max_joltage = get_max_joltage(batteries, battery_map, num_batteries);
    // std::cout << "Max joltage from " << line << " is " << max_joltage << std::endl;

    // // print battery_map for debugging
    // for (const auto &entry : battery_map) {
    //     std::cout << "Key: (" << entry.first.first << ", " << entry.first.second << "), Value: " << entry.second << std::endl;
    // }

    return max_joltage;
}


int main(int argc, char *argv[])
{
    std::string filename = (argc > 1) ? argv[1] : "inputs/day3.txt";

    try
    {
        std::cout << "Day 3 Solution" << std::endl;
        auto lines = aoc::read_lines(filename);

        auto part1_start = std::chrono::high_resolution_clock::now();

        int part1_result = 0;
        for (const auto &line : lines)
        {
            part1_result += get_max_joltage_from_string(line, 2);
        }

        auto part1_end = std::chrono::high_resolution_clock::now();

        // Calculate the duration
        std::chrono::duration<double> part1_elapsed = part1_end - part1_start;
        
        std::cout << "Part 1: " << part1_result << " in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;

        // part 2
        auto part2_start = std::chrono::high_resolution_clock::now();

        long long part2_result = 0;
        for (const auto &line : lines)
        {
            part2_result += get_max_joltage_from_string(line, 12);
        }

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
