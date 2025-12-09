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
#include "utils.h"


const std::string SOLUTION_DAY = "5";

std::vector<std::pair<long long, long long>> parse_ranges(std::vector<std::string> lines) {
    std::vector<std::pair<long long, long long>> ranges;

    for (const std::string &line : lines) {
        auto dash = line.find('-');
        std::string start_str(line.substr(0, dash));
        std::string end_str(line.substr(dash + 1));

        long long start = std::stoll(start_str);
        long long end = std::stoll(end_str);

        assert(start <= end);

        ranges.push_back({start, end});
    }
    return ranges;
}

std::vector<long long> parse_ids(std::vector<std::string> lines) {
    std::vector<long long> ids;

    for (const std::string &line : lines) {
        long long id = std::stoll(line);
        ids.push_back(id);
    }
    return ids;
}

bool in_range(std::pair<long long, long long> range, long long id) {
    return (id >= range.first && id <= range.second);
}

int count_ids_in_ranges(std::vector<std::pair<long long, long long>> ranges, std::vector<long long> ids){
    int fresh_ids = 0;
    for (auto &id : ids) {
        for (auto &range : ranges) {
            if (in_range(range, id)) {
                fresh_ids += 1;
                break;
            }
        }
    }
    return fresh_ids;
}

bool is_overlapping(std::pair<long long, long long> range_a, std::pair<long long, long long> range_b) {
    return !(range_a.second < range_b.first || range_a.first > range_b.second);
}

std::pair<long long, long long> merge_two_ranges(std::pair<long long, long long> &range_a, std::pair<long long, long long> &range_b) {
    long long min_range = (range_a.first < range_b.first) ? range_a.first : range_b.first;
    long long max_range = (range_a.second > range_b.second) ? range_a.second : range_b.second;

    assert(min_range < max_range);
    return {min_range, max_range};
}

bool merge_ranges(std::vector<std::pair<long long, long long>> &ranges) {

    // find any overlap, merge the two overlapping ranges
    for (size_t i = 0; i < ranges.size() - 1; ++i) {
        for (size_t j = i+1; j < ranges.size(); ++j) {

            // if found an overlap, merge the overlapping ranges and return a new vector of ranges
            if (is_overlapping(ranges[i], ranges[j])) {
                std::pair<long long, long long> merged_range = merge_two_ranges(ranges[i], ranges[j]);
                ranges.erase(ranges.begin() + std::max(i, j));
                ranges.erase(ranges.begin() + std::min(i, j));
                ranges.push_back(merged_range);
                return true;
            }
        }
    }
    return false;
}

long long count_fresh_ids(std::vector<std::pair<long long, long long>> ranges) {
    while (merge_ranges(ranges)) {}
    long long total_count = 0;
    for (const auto &range : ranges) {
        total_count += (range.second - range.first + 1);
    }

    return total_count;
}

int main(int argc, char *argv[])
{
    std::string filename = (argc > 1) ? argv[1] : "inputs/day" + SOLUTION_DAY + ".txt";
    try
    {
        std::cout << "Day " + SOLUTION_DAY + " Solution" << std::endl;
        auto lines = aoc::read_lines(filename);

        // split lines vector to two parts: ranges and ids, on empty line
        auto empty_line_it = std::find(lines.begin(), lines.end(), "");
        std::vector<std::string> range_lines(lines.begin(), empty_line_it);
        std::vector<std::string> id_lines(empty_line_it + 1, lines.end());

        std::vector<std::pair<long long, long long>> ranges = parse_ranges(range_lines);
        const std::vector<long long> ids = parse_ids(id_lines);

        // debug print ranges and ids
        // for (const auto &range : ranges) {
        //     std::cout << "Range: " << range.first << "-" << range.second << std::endl;
        // }
        // std::cout << "IDs: ";
        // for (const auto &id : ids) {
        //     std::cout << id << " ";
        // }
        // std::cout << std::endl;

        auto part1_start = std::chrono::high_resolution_clock::now();

        long long part1_result = count_ids_in_ranges(ranges, ids);

        auto part1_end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> part1_elapsed = part1_end - part1_start;
        
        std::cout << "Part 1: " << part1_result << " in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;

        // part 2
        auto part2_start = std::chrono::high_resolution_clock::now();


        auto rng = std::default_random_engine {};
        std::shuffle(ranges.begin(), ranges.end(), rng);

        long long part2_result = count_fresh_ids(ranges);

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
