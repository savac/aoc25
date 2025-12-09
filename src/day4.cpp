#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <numeric>
#include <chrono>
#include <map>
#include <cassert>
#include "utils.h"


const std::string SOLUTION_DAY = "4";

std::vector<std::vector<int>> read_matrix(const std::vector<std::string> &lines){
    std::vector<std::vector<int>> mat;
    for (const std::string &line : lines) {
        std::vector<int> row;
        for (char ch : line) {
            if (ch == '.') {
                row.push_back(0);
            } 
            else if (ch == '@') {
                row.push_back(1);
            }
        }
        mat.push_back(row);
    }
    return mat;
}

int access_mat(const std::vector<std::vector<int>> &mat, int i, int j, const int M, const int N) {
    if (i < 0 || i >= M || j < 0 || j >= N) {
        return 0;
    }
    else {
        return mat[i][j];
    }
}

bool is_accessible(const std::vector<std::vector<int>> &mat, int i, int j, const int M, const int N) {
    if (mat[i][j] == 0) {
        return false;
    }

    int count_neighbours = (
        access_mat(mat, i-1, j-1, M, N) +
        access_mat(mat, i-1, j, M, N) +
        access_mat(mat, i-1, j+1, M, N) +
        access_mat(mat, i, j-1, M, N) +
        access_mat(mat, i, j+1, M, N) +
        access_mat(mat, i+1, j-1, M, N) +
        access_mat(mat, i+1, j, M, N) +
        access_mat(mat, i+1, j+1, M, N)
    );

    return (count_neighbours < 4);
}

int count_accessible_logs(const std::vector<std::vector<int>> &mat) {

    const int M = mat.size();
    const int N = mat[0].size();

    int accessible_count = 0;

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            if (is_accessible(mat, i, j, M, N)) {
                accessible_count += 1;
            }
        }
    }
    return accessible_count;
}

int remove_logs(std::vector<std::vector<int>> mat) {
    const int M = mat.size();
    const int N = mat[0].size();

    int total_removed = 0;
    int removed_in_iteration = 0;
    do {
        removed_in_iteration = 0;
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                if (is_accessible(mat, i, j, M, N)) {
                    removed_in_iteration += 1;
                    mat[i][j] = 0;
                }
            }
        }
        total_removed += removed_in_iteration;
    } while (removed_in_iteration > 0);

    return total_removed;
}

int main(int argc, char *argv[])
{
    std::string filename = (argc > 1) ? argv[1] : "inputs/day" + SOLUTION_DAY + ".txt";
    try
    {
        std::cout << "Day 4 Solution" << std::endl;
        auto lines = aoc::read_lines(filename);
        std::vector<std::vector<int>> mat = read_matrix(lines);

        auto part1_start = std::chrono::high_resolution_clock::now();

        int part1_result = count_accessible_logs(mat);

        auto part1_end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> part1_elapsed = part1_end - part1_start;
        
        std::cout << "Part 1: " << part1_result << " in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;

        // part 2
        auto part2_start = std::chrono::high_resolution_clock::now();

        long long part2_result = remove_logs(mat);

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
