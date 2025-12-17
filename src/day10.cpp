#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cassert>
#include "utils.h"

using ll = long long;

const std::string SOLUTION_DAY = "10";

// [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
// [...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
// [.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}

// implement the equivalent of Pythons itertools.combinations
std::vector<std::vector<int>> combinations(std::vector<int> pool, int r) {
    // assuming button can only be pressed once
    // combinations(3, 2) → 01, 12, 02
    int n = static_cast<int>(pool.size());

    if (r > n) {
        std::cerr << "Cannot request more presses than buttons." << std::endl;
        exit(1);
    }

    std::vector<std::vector<int>> res;
    std::vector<int> combo;
    std::vector<int> indices;

    for (int i = 0; i < r; ++i) {
        indices.push_back(i);
    }

    for (const auto &i : indices) {
        combo.push_back(pool[i]);
    }
    res.push_back(combo);

    while (true) {
        int i;
        bool completed_loop = true;
        for (i = r - 1; i >= 0; --i) {
            if (indices[i] != i + n - r) {
                completed_loop = false;
                break;
            }
        }

        // completed loop without break
        if (completed_loop)
            break;

        indices[i] += 1;
        for (int j = i + 1; j < r; ++j) {
            indices[j] = indices[j - 1] + 1;
        }
        combo.clear();
        for (const auto &ind : indices) {
            combo.push_back(pool[ind]);
        }
        res.push_back(combo);
    }

    // std::string row;
    // for (auto &combo : res) {
    //     row = "";
    //     for (auto &el : combo) {
    //         row += std::to_string(el) + ',';
    //     }
    //     std::cout << row << std::endl;
    // }

    return res;
}

class Puzzle {
public:
    std::vector<bool> target_lights;
    std::vector<std::vector<size_t>> buttons;
    std::vector<int> joltage;
    std::string puzzle_str;

    static Puzzle fromString(const std::string &line) {
        auto puzzle = Puzzle();
        puzzle.puzzle_str = line;
        auto line_components = aoc::split(line, ' ');
        for (const auto &component : line_components) {

            // lights
            if (component[0] == '[') {
                for (size_t i = 1; i < component.size() - 1; ++i) {
                    if (component[i] == '.') {
                        puzzle.target_lights.push_back(false);
                    }
                    else if (component[i] == '#') {
                        puzzle.target_lights.push_back(true);
                    }
                    else {
                        std::cerr << "Input error: " + line << std::endl;
                        exit(1);
                    }
                }
            }

            // buttons
            if (component[0] == '(') {
                std::vector<size_t> button;
                for (auto &ind : aoc::split(component.substr(1, component.size() - 2), ',')) {
                    button.push_back(std::stoi(ind));
                }
                puzzle.buttons.push_back(button);
            }

            //joltage
            if (component[0] == '{') {
                for (auto &ind : aoc::split(component.substr(1, component.size() - 2), ',')) {
                    puzzle.joltage.push_back(std::stoi(ind));
                }
            }

        }

        assert(puzzle.target_lights.size() == puzzle.joltage.size());

        return puzzle;
    }

    int solve() {
        // std::cout << puzzle_str << std::endl;

        std::vector<int> all_button_inds;
        for (size_t i = 0; i < buttons.size(); ++i) {
            all_button_inds.push_back(i);
        }

        for (size_t num_presses = 1; num_presses < buttons.size(); ++num_presses) {
            std::vector<bool> lights(target_lights.size());
            auto button_inds_combinations = combinations(all_button_inds, num_presses);
            for (auto &button_inds : button_inds_combinations) {
                std::vector<bool> lights;
                for (size_t i = 0; i < target_lights.size(); ++i) {
                    lights.push_back(false);
                }
                for (auto &button_ind : button_inds) {
                    auto button = buttons[button_ind];
                    for (auto &indicator : button) {
                        lights[indicator] = !lights[indicator];
                    }
                }
                if (lights == target_lights){
                    return num_presses;
                }
            }
        }
        std::cerr << "Could not solve." << std::endl;
        exit(1);
    }
};

ll get_part1(std::vector<Puzzle> &puzzles) {
    ll res = 0;
    for (auto &puzzle : puzzles) {
        res += puzzle.solve();
    }
    return res;
}

int main(int argc, char *argv[])
{
    std::string filename = (argc > 1) ? argv[1] : "inputs/day" + SOLUTION_DAY + ".txt";
    try
    {
        std::cout << "Day " + SOLUTION_DAY + " Solution" << std::endl;
        auto lines = aoc::read_lines(filename);

        std::vector<Puzzle> puzzles;

        for (const auto &line : lines) {
            puzzles.push_back(Puzzle::fromString(line));
        }
        
        // std::vector<int> input = {1,2,3};
        // auto res = combinations({1,10,100,1000,}, 3);
        // std::string row;
        // for (auto &combo : res) {
        //     row = "";
        //     for (auto &el : combo) {
        //         row += std::to_string(el) + ',';
        //     }
        //     std::cout << row << std::endl;
        // }

        auto part1_start = std::chrono::high_resolution_clock::now();

        ll part1_result = get_part1(puzzles);
        auto part1_end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> part1_elapsed = part1_end - part1_start;
        
        std::cout << "Part 1: " << part1_result << " in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;

        // part 2
        auto part2_start = std::chrono::high_resolution_clock::now();

        ll part2_result = 0;

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
