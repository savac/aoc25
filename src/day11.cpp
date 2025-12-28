#include <iostream>
#include <string>
#include <ranges>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include "utils.h"

using ll = long long;

const std::string SOLUTION_DAY = "11";

class Graph
{
public:
    std::unordered_map<std::string, std::vector<std::string>> nodes;

    static Graph fromLines(std::vector<std::string> &lines)
    {
        auto graph = Graph();
        
        for (auto &line : lines) {
            auto parts = aoc::split(line, ' ');

            auto source_name = std::string(parts[0].begin(), parts[0].end() - 1);
            std::vector<std::string> adjacent_nodes = std::vector<std::string>(parts.begin() + 1, parts.end());

            graph.nodes[source_name] = adjacent_nodes;
        }

        return graph;
    }

    ll count_paths(const std::string &source, const std::string &target, std::unordered_map<std::string, ll> &visited) {
        if (source == target) {
            return 1;
        }

        if (nodes.contains(source)) {
            // found, explore deeper
            ll res = 0;
            for (auto adjacent : nodes[source]) {
                if (!visited.contains(adjacent)) {
                    visited[adjacent] = count_paths(adjacent, target, visited);
                }
                res += visited[adjacent];
            }
            return res;
        } else {
            // not found
            return 0;
        }
    }
};

int main(int argc, char *argv[])
{
    std::string filename = (argc > 1) ? argv[1] : "inputs/day" + SOLUTION_DAY + ".txt";
    try
    {
        std::cout << "Day " + SOLUTION_DAY + " Solution" << std::endl;
        auto lines = aoc::read_lines(filename);

        auto graph = Graph::fromLines(lines);

        auto part1_start = std::chrono::high_resolution_clock::now();

        std::unordered_map<std::string, ll> visited;
        ll part1_result = graph.count_paths("you", "out", visited);
        auto part1_end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> part1_elapsed = part1_end - part1_start;

        std::cout << "Part 1: " << part1_result << " in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;

        // part 2
        auto part2_start = std::chrono::high_resolution_clock::now();

        visited.clear();
        ll part2_result = graph.count_paths("svr", "dac", visited);
        visited.clear();
        part2_result *= graph.count_paths("dac", "fft", visited);
        visited.clear();
        part2_result *= graph.count_paths("fft", "out", visited);

        visited.clear();
        ll part2_result_alt = graph.count_paths("svr", "fft", visited);
        visited.clear();
        part2_result_alt *= graph.count_paths("fft", "dac", visited);
        visited.clear();
        part2_result_alt *= graph.count_paths("dac", "out", visited);
        
        part2_result += part2_result_alt;

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
};
