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


const std::string SOLUTION_DAY = "6";

std::vector<long long> parse_line_to_ints(std::string line) {
    std::vector<long long> ints;
    std::istringstream iss(line);
    long long num;

    while (iss >> num) {
        ints.push_back(num);
    }

    return ints;
}

std::vector<std::string> parse_line_to_operators(std::string line) {
    std::vector<std::string> ops;
    std::istringstream iss(line);
    std::string op;

    while (iss >> op) {
        ops.push_back(op);
    }

    return ops;
}

long long run_operations(std::vector<std::vector<long long>> &data, std::vector<std::string> &operations) {
    long long res = 0;
    
    for (size_t i = 0; i < operations.size(); ++i) {
        if (operations[i] == "+") {
            for (size_t j = 0; j < data.size(); ++j) {
                res += data[j][i];
            }
        }
        else if (operations[i] == "*") {
            long long this_res = 1;
            for (size_t j = 0; j < data.size(); ++j) {
                this_res *= data[j][i];
            }
            res += this_res;
        }
    }
    return res;
}

long long run_operations_part2(std::vector<std::vector<long long>> &data, std::vector<std::string> &operations) {
    long long res = 0;
    
    for (size_t i = 0; i < operations.size(); ++i) {
        if (operations[i] == "+") {
            for (size_t j = 0; j < data[i].size(); ++j) {
                res += data[i][j];
            }
        }
        else if (operations[i] == "*") {
            long long this_res = 1;
            for (size_t j = 0; j < data[i].size(); ++j) {
                this_res *= data[i][j];
            }
            res += this_res;
        }
    }
    return res;
}

bool is_numeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

std::string strip_whitespaces(const std::string& str) {
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
    return result;
}


std::vector<std::vector<long long>> parse_cephalopod_data(std::vector<std::vector<char>> &data_char) {
    std::vector<std::vector<long long>> exercise_term_mat;

    std::vector<long long> terms;

    for (size_t i = 0; i < data_char[0].size(); ++i) {
        std::string term;
        for (size_t j = 0; j < data_char.size(); ++j) {
            term += data_char[j][i];
        }
        term = strip_whitespaces(term);

        if (term.empty()) {
            // exercise finished
            exercise_term_mat.push_back(terms);
            terms.clear();
        } else if (is_numeric(term)) {
            long long term_num = std::stoll(term);
            terms.push_back(term_num);
        } else {
            std::cerr << "Invalid conversion of " + term + "." << std::endl;
            exit(1);
        }
    }

    // final term
    if (!terms.empty()) {
        exercise_term_mat.push_back(terms);
    }

    return exercise_term_mat;
} 

int main(int argc, char *argv[])
{
    std::string filename = (argc > 1) ? argv[1] : "inputs/day" + SOLUTION_DAY + ".txt";
    try
    {
        std::cout << "Day " + SOLUTION_DAY + " Solution" << std::endl;
        auto lines = aoc::read_lines(filename);

        std::vector<std::vector<long long>> data;    
        for (size_t i = 0; i < lines.size() - 1; ++i) {
            data.push_back(parse_line_to_ints(lines[i]));
        }
        std::vector<std::string> ops = parse_line_to_operators(lines.back());
        for (size_t i = 0; i < data.size(); ++i) {
            assert(data[i].size() == ops.size());
        }

        // for part 2 read data as array of chars preserving spaces
        std::vector<std::vector<char>> data_char;
        for (size_t i = 0; i < lines.size() - 1; ++i) {
            std::vector<char> row;
            for (char c : lines[i]) {
                row.push_back(c);
            }
            data_char.push_back(row);
        }
        for (size_t i = 1; i < data_char.size(); ++i) {
            assert(data_char[i].size() == data_char[0].size());
        }

        auto part1_start = std::chrono::high_resolution_clock::now();

        long long part1_result = run_operations(data, ops);
        auto part1_end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> part1_elapsed = part1_end - part1_start;
        
        std::cout << "Part 1: " << part1_result << " in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;

        // part 2
        auto part2_start = std::chrono::high_resolution_clock::now();

        auto exercise_term_mat = parse_cephalopod_data(data_char);

        long long part2_result = run_operations_part2(exercise_term_mat, ops);

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
