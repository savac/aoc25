#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <string_view>
#include <charconv>
#include <set>
#include <numeric>
#include <chrono>
#include "utils.h"

const std::string SOLUTION_DAY = "2";

long long find_invalid_ids(std::pair<long long, long long> range, bool part1=true) {
    std::set<long long> invalid_ids;

    std::string start_str = std::to_string(range.first);
    int start_len = start_str.size();

    for (int len = 1; len <= start_len / 2; ++len) {
        if (start_len % len != 0) {
            continue;
        }

        if (part1) {
            // part 1 filter, should be exactly two equal halves
            if (!(len == start_len / 2 && start_len % 2 == 0)) {
                continue;
            }
        }

        // iterate over the divisors of the start of the range
        // for 123456, len is from [1,2,3] range
        // use len to get the first chunk of the range start
        // increment and replicate that chunk to form invalid id candidates
        // add to invalid_ids if within range
        std::string start_chunk_str = start_str.substr(0, len);
        long long start_chunk_num = std::stoll(start_chunk_str);
        long long max_chunk_num = std::pow(10, len) - 1;
        for (long long chunk_num = start_chunk_num; chunk_num <= max_chunk_num; ++chunk_num) {
            std::string chunk_str = std::to_string(chunk_num);
            std::string candidate_str;
            for (int i = 0; i < start_len / len; ++i) {
                candidate_str += chunk_str;
            }
            long long candidate = std::stoll(candidate_str);
            if (candidate >= range.first && candidate <= range.second) {
                invalid_ids.insert(candidate);
            }
            if (candidate > range.second) {
                break;
            }
        }
    }

    // if we have exhausted the range.first (start_str) length and have not reached range.second
    // e.g. given 123-2000 range, we are at 999 now we need to consider the next length 4
    std::string end_str = std::to_string(range.second);
    long long next_len_total = 0;
    int end_len = end_str.size();
    if (end_len > start_len) {
        next_len_total = find_invalid_ids({std::pow(10, start_len), range.second}, part1);
    }

    long long current_len_total = std::accumulate(invalid_ids.begin(), invalid_ids.end(), 0LL);

    return current_len_total + next_len_total;
}

int main(int argc, char *argv[])
{
    std::string filename = (argc > 1) ? argv[1] : "inputs/day2.txt";

    try
    {
        std::cout << "Day " + SOLUTION_DAY + " Solution" << std::endl;

        auto lines = aoc::read_lines(filename);
        const auto ranges = aoc::parse_ranges(lines[0]);

        auto part1_start = std::chrono::high_resolution_clock::now();


        long long total_invalid_ids_sum = 0;

        // part 1
        for (const auto &range : ranges) {
            total_invalid_ids_sum += find_invalid_ids(range, true);
        }
        auto part1_end = std::chrono::high_resolution_clock::now();

        // Calculate the duration
        std::chrono::duration<double> part1_elapsed = part1_end - part1_start;
        
        std::cout << "Part 1: " << total_invalid_ids_sum << " in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;

        // part 2
        auto part2_start = std::chrono::high_resolution_clock::now();
        total_invalid_ids_sum = 0;
        for (const auto &range : ranges) {
            total_invalid_ids_sum += find_invalid_ids(range, false);
        }
        auto part2_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> part2_elapsed = part2_end - part2_start;
        std::cout << "Part 2: " << total_invalid_ids_sum << " in " << part2_elapsed.count() * 1e3 << " ms" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
