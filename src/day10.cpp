#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include "utils.h"

using ll = long long;

const std::string SOLUTION_DAY = "10";

// [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
// [...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
// [.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}

// implement the equivalent of Pythons itertools.combinations
std::vector<std::vector<int>> combinations(std::vector<int> pool, int r)
{
    // assuming button can only be pressed once
    // combinations(3, 2) → 01, 12, 02
    int n = static_cast<int>(pool.size());

    if (r > n)
    {
        std::cerr << "Cannot request more presses than buttons." << std::endl;
        exit(1);
    }

    std::vector<std::vector<int>> res;
    std::vector<int> combo;
    std::vector<int> indices;

    for (int i = 0; i < r; ++i)
    {
        indices.push_back(i);
    }

    for (const auto &i : indices)
    {
        combo.push_back(pool[i]);
    }
    res.push_back(combo);

    while (true)
    {
        int i;
        bool completed_loop = true;
        for (i = r - 1; i >= 0; --i)
        {
            if (indices[i] != i + n - r)
            {
                completed_loop = false;
                break;
            }
        }

        // completed loop without break
        if (completed_loop)
            break;

        indices[i] += 1;
        for (int j = i + 1; j < r; ++j)
        {
            indices[j] = indices[j - 1] + 1;
        }
        combo.clear();
        for (const auto &ind : indices)
        {
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

// Generator of for a vector of integers of legth n that sum up to the given budget:
// e.g.
// BudgetCombinations(3, 3) -> [0,0,3], [0,1,2], [0,2,1] etc.
// BudgetCombinations(2, 1) -> [0,1], [1,0]
class BudgetCombinations
{
private:
    int n;
    int budget;
    int current;
    // this has to be pointer otherwise it complains that the class in not defined
    std::unique_ptr<BudgetCombinations> remaining_combination;

public:
    BudgetCombinations(int n_val, int budget_val)
        : n(n_val), budget(budget_val)
    {
        current = budget;
        if (n > 1 && budget > 0)
        {
            remaining_combination = std::make_unique<BudgetCombinations>(n - 1, 0);
        }
    }

    std::pair<bool, std::vector<int>> next()
    {
        if (n == 0)
        {
            return {true, {}};
        }

        if (n == 1)
        {
            return {true, {budget}};
        }

        if (budget == 0)
        {
            std::vector<int> this_res(n, 0);
            return {true, {this_res}};
        }

        std::vector<int> res = {current};

        auto [ended_remaining, res_remaining] = remaining_combination->next();
        for (auto &rem : res_remaining)
        {
            res.push_back(rem);
        }

        if (ended_remaining)
        {
            if (current <= 0)
            {
                return {true, res};
            }
            else
            {
                --current;
                remaining_combination = std::make_unique<BudgetCombinations>(n - 1, budget - current);
            }
        }

        return {false, res};
    }
};

enum CombinationsStatus {
    InProgress,
    EndedOk,
    EndedError
};

class BudgetCombinationsWithLimits
{
private:
    std::vector<int> free_indices;
    std::vector<int> constraints;
    std::vector<int> llim;
    std::vector<int> ulim;
    int budget;

    // this has to be pointer otherwise it complains that the class in not defined
    std::unique_ptr<BudgetCombinationsWithLimits> next_combinations;
    std::vector<int> next_free_indices;
    int current_ind;

public:
    BudgetCombinationsWithLimits(
        std::vector<int> free_indices_val,
        std::vector<int> constraints_val,
        std::vector<int> llim_val,
        std::vector<int> ulim_val,
        int budget_val
    ) : free_indices(free_indices_val), constraints(constraints_val), llim(llim_val), ulim(ulim_val), budget(budget_val) {

        if (free_indices.size() > 1 && budget > 0) {
            
            current_ind = free_indices[0];
            next_free_indices = std::vector<int>(free_indices.begin() + 1, free_indices.end());

            std::vector<int> next_free_indices(free_indices.begin() + 1, free_indices.end());
            constraints[current_ind] = std::min(budget, ulim[current_ind]);
            next_combinations = std::make_unique<BudgetCombinationsWithLimits>(
                next_free_indices,
                constraints,
                llim,
                ulim,
                budget - constraints[current_ind]
            );
        }
    }

    std::pair<CombinationsStatus, std::vector<int>> next()
    {
        // can have 3 outcomes:
        // EndedOk: exhausted all combinations but found one valid one
        // EndedError: exhausted and not found any valid combinations
        // InProgress: found a valid combination but not exhausted yet
        

        if (free_indices.size() == 0) {
            if (budget == 0)
            {
                return {EndedOk, constraints};
            } else {
                return {EndedError, constraints};
            }
        }

        if (free_indices.size() == 1) {
            if (budget >= llim[free_indices[0]] && budget <= ulim[free_indices[0]]) {
                auto res = constraints;
                res[free_indices[0]] = budget;
                return {EndedOk, res};
            } else {
                return {EndedError, constraints};
            }   
        }

        if (budget == 0) {
            auto res = constraints;
            for (auto &ind : free_indices) {
                if (llim[ind] > 0) {
                    return {EndedError, res};
                } else {
                    res[ind] = 0;
                }
            }
            return {EndedOk, res};
        }

        // budget > 0 and free_indices.size() > 1

        auto [next_status, possible_constraints] = next_combinations->next();
        
        if (next_status == EndedOk) {
            if (constraints[current_ind] <= llim[current_ind]) {
                return {EndedOk, possible_constraints};
            }
            else {
                --constraints[current_ind];
                
                next_combinations = std::make_unique<BudgetCombinationsWithLimits>(
                    next_free_indices,
                    constraints,
                    llim,
                    ulim,
                    budget - constraints[current_ind]
                );
                return {InProgress, possible_constraints};
            }
        }
        
        if (next_status == InProgress) {
            return {InProgress, possible_constraints};
        } 

        if (next_status == EndedError) {
            if (constraints[current_ind] <= llim[current_ind]) {
                return {EndedError, possible_constraints};
            }
            else {
                --constraints[current_ind];
                
                next_combinations = std::make_unique<BudgetCombinationsWithLimits>(
                    next_free_indices,
                    constraints,
                    llim,
                    ulim,
                    budget - constraints[current_ind]
                );
                return next();
            }
        } 
    }
};

bool share_elements(const std::vector<int> &a, const std::vector<int> &b)
{
    for (int x : a) {
        for (int y : b) {
            if (x == y)
                return true;
        }
    }
    return false;
};

class Puzzle
{
public:
    std::vector<bool> target_lights;
    std::vector<std::vector<size_t>> buttons;
    std::vector<int> joltage;
    std::string puzzle_str;
    std::vector<std::vector<int>> indicator_to_buttons;

    static Puzzle fromString(const std::string &line)
    {
        auto puzzle = Puzzle();
        puzzle.puzzle_str = line;
        auto line_components = aoc::split(line, ' ');
        for (const auto &component : line_components)
        {

            // lights
            if (component[0] == '[')
            {
                for (size_t i = 1; i < component.size() - 1; ++i)
                {
                    if (component[i] == '.')
                    {
                        puzzle.target_lights.push_back(false);
                    }
                    else if (component[i] == '#')
                    {
                        puzzle.target_lights.push_back(true);
                    }
                    else
                    {
                        std::cerr << "Input error: " + line << std::endl;
                        exit(1);
                    }
                }
            }

            // buttons
            if (component[0] == '(')
            {
                std::vector<size_t> button;
                for (auto &ind : aoc::split(component.substr(1, component.size() - 2), ','))
                {
                    button.push_back(std::stoi(ind));
                }
                puzzle.buttons.push_back(button);
            }

            // joltage
            if (component[0] == '{')
            {
                for (auto &ind : aoc::split(component.substr(1, component.size() - 2), ','))
                {
                    puzzle.joltage.push_back(std::stoi(ind));
                }
            }
        }

        // sort the buttons in terms of how many buttons they affect
        // in descending order
        std::sort(puzzle.buttons.begin(), puzzle.buttons.end(),
                  [](const std::vector<size_t> &a, const std::vector<size_t> &b)
                  { return a.size() > b.size(); });

        // indicator to button look-up
        puzzle.indicator_to_buttons = std::vector<std::vector<int>>(puzzle.joltage.size());
        for (size_t button_ind = 0; button_ind < puzzle.buttons.size(); ++button_ind)
        {
            for (auto &indicator : puzzle.buttons[button_ind])
            {
                puzzle.indicator_to_buttons[indicator].push_back(button_ind);
            }
        }

        assert(puzzle.target_lights.size() == puzzle.joltage.size());

        // // Initialize i2b matrix (indicators x buttons)
        // size_t num_indicators = puzzle.joltage.size();
        // size_t num_buttons = puzzle.buttons.size();
        // puzzle.i2b.resize(num_indicators, std::vector<int>(num_buttons, 0));

        // // Fill the matrix: 1 if button affects indicator, 0 otherwise
        // for (size_t button_ind = 0; button_ind < num_buttons; ++button_ind)
        // {
        //     for (auto &indicator : puzzle.buttons[button_ind])
        //     {
        //         puzzle.i2b[indicator][button_ind] = 1;
        //     }
        // }

        // // Print the matrix
        // std::cout << "Indicator to Button Matrix:" << std::endl;
        // std::cout << "   ";
        // for (size_t b = 0; b < num_buttons; ++b)
        // {
        //     std::cout << "B" << b << " ";
        // }
        // std::cout << std::endl;

        // for (size_t i = 0; i < num_indicators; ++i)
        // {
        //     std::cout << "I" << i << " ";
        //     for (size_t b = 0; b < num_buttons; ++b)
        //     {
        //         std::cout << puzzle.i2b[i][b] << "  ";
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << std::endl;

        return puzzle;
    }

    int solve_part1()
    {
        // std::cout << puzzle_str << std::endl;

        std::vector<int> all_button_inds;
        for (size_t i = 0; i < buttons.size(); ++i)
        {
            all_button_inds.push_back(i);
        }

        for (size_t num_presses = 1; num_presses < buttons.size(); ++num_presses)
        {
            std::vector<bool> lights(target_lights.size());
            auto button_inds_combinations = combinations(all_button_inds, num_presses);
            for (auto &button_inds : button_inds_combinations)
            {
                std::vector<bool> lights;
                for (size_t i = 0; i < target_lights.size(); ++i)
                {
                    lights.push_back(false);
                }
                for (auto &button_ind : button_inds)
                {
                    auto button = buttons[button_ind];
                    for (auto &indicator : button)
                    {
                        lights[indicator] = !lights[indicator];
                    }
                }
                if (lights == target_lights)
                {
                    return num_presses;
                }
            }
        }
        std::cerr << "Could not solve." << std::endl;
        exit(1);
    }

    void get_limits(std::vector<int> &llim, std::vector<int> &ulim)
    {

        // std::cout << "starting limits" << std::endl;
        // aoc::print_vector(llim);
        // aoc::print_vector(ulim);

        bool updated = true;
        while (updated)
        {
            updated = false;

            // ll
            for (size_t i = 0; i < indicator_to_buttons.size(); ++i)
            {
                auto button_inds = indicator_to_buttons[i];
                // only update those that start at start_ind
                for (size_t j = 0; j < button_inds.size(); ++j)
                {
                    int this_ll = joltage[i];
                    for (size_t k = 0; k < button_inds.size(); ++k)
                    {
                        if (k != j)
                        {
                            this_ll -= ulim[button_inds[k]];
                        }
                    }
                    if (this_ll > llim[button_inds[j]] && this_ll <= ulim[button_inds[j]])
                    {
                        llim[button_inds[j]] = this_ll;
                        updated = true;
                    }

                }
            }

            // ul
            for (size_t i = 0; i < indicator_to_buttons.size(); ++i)
            {
                auto button_inds = indicator_to_buttons[i];
                for (size_t j = 0; j < button_inds.size(); ++j)
                {
                    int other_ll_total = 0;
                    for (size_t k = 0; k < button_inds.size(); ++k)
                    {
                        if (k != j)
                        {
                            other_ll_total += llim[button_inds[k]];
                        }
                    }
                    auto candidate_ul = joltage[i] - other_ll_total;
                    if (candidate_ul < ulim[button_inds[j]] && candidate_ul >= llim[button_inds[j]])
                    {
                        ulim[button_inds[j]] = candidate_ul;
                        updated = true;
                    }
                }
            }
        }

        for (size_t t = 0; t < llim.size(); ++t) {
            if (llim[t] > ulim[t]) {
                std::cerr << "limits bad" << std::endl;
            }
        }

        // std::cout << "updated limits" << std::endl;
        // aoc::print_vector(button_ll);
        // aoc::print_vector(button_ul);
    }

    int presses_search(
        int button_ind, 
        std::vector<int> presses, 
        std::vector<int> &joltage_budget, 
        std::vector<int> &llim, 
        std::vector<int> &ulim
    ) {

        for (int press_candidate = ulim[button_ind]; press_candidate >= llim[button_ind]; --press_candidate) {
            // if (button_ind == 0)
            //     std::cout << "Button index 0, " + std::to_string(press_candidate) << std::endl;
            // std::cout << "Button index " + std::to_string(button_ind) << std::endl;
            // aoc::print_vector(buttons[button_ind]);
            // aoc::print_vector(joltage_budget);
            // aoc::print_vector(presses);

            auto candidate_joltage_budget = joltage_budget;

            bool budget_ok = true;

            for (auto &indicator : buttons[button_ind]) {
                if (candidate_joltage_budget[indicator] >= press_candidate) {
                    candidate_joltage_budget[indicator] -= press_candidate;
                } else {
                    budget_ok = false;
                    break;
                }
            }

            if (budget_ok) {
                presses[button_ind] = press_candidate;
                // std::cout << "ok" << std::endl;
                // aoc::print_vector(presses);
                // aoc::print_vector(candidate_joltage_budget);

                // if this was last button
                if (button_ind == static_cast<int>(buttons.size() - 1)) {
                    auto total_budget = std::accumulate(candidate_joltage_budget.begin(), 
                        candidate_joltage_budget.end(), 0);
                    // std::cout << "last, " + std::to_string(total_budget) << std::endl;
                    if (total_budget == 0) {
                        // std::cout << "final presses" << std::endl;
                        // aoc::print_vector(presses);
                        return std::accumulate(presses.begin(), presses.end(), 0);;
                    } else {
                        // std::cout << "-1" << std::endl;
                        return -1;
                    }
                } 

                // more buttons to press

                // review the limits as we have already fixed one or more buttons
                auto next_llim = llim;
                auto next_ulim = ulim;
                for (int i = 0; i <= button_ind; ++i) {
                    next_llim[i] = presses[i];
                    next_ulim[i] = presses[i];
                }
                get_limits(next_llim, next_ulim);

                auto search_res = presses_search(button_ind + 1, presses, candidate_joltage_budget, next_llim, next_ulim);
                
                // if valid solution found return
                if (search_res >= 0) {
                    return search_res;
                }

                // if got to the end of the range
                if (press_candidate == llim[button_ind]) {
                    return search_res;
                }

                // else try next press_candidate
                // std::cout << std::to_string(limits_ok) << std::endl;
            }
        }

        // std::cout << button_ind << std::endl;
        // aoc::print_vector(llim);
        // aoc::print_vector(ulim);
        // aoc::print_vector(presses);
        // std::cerr << "should not be here!" << std::endl;
        // exit(1);
        return -1;
    }

    int solve_part2_alternative() {
        
        std::cout << puzzle_str << std::endl;

        std::vector<int> llim(buttons.size(), 0);
        std::vector<int> ulim(buttons.size(), std::numeric_limits<int>::max());

        for (size_t i = 0; i < indicator_to_buttons.size(); ++i)
        {
            for (auto &button_ind : indicator_to_buttons[i])
            {
                if (ulim[button_ind] > joltage[i])
                {
                    ulim[button_ind] = joltage[i];
                }
            }
        }

        get_limits(llim, ulim);

        // -1 means it's still unconstrained
        std::vector<int> presses(buttons.size(), 0);

        // assume buttons are ranked in term of how many indicators they affect, in descending order

        auto min_presses = presses_search(0, presses, joltage, llim, ulim);
        std::cout << "solution found: " + std::to_string(min_presses) << std::endl;

        return min_presses;

    }
};

ll solve_all(std::vector<Puzzle> &puzzles, bool part1)
{
    ll res = 0;
    for (auto &puzzle : puzzles)
    {
        if (part1)
        {
            res += puzzle.solve_part1();
        }
        else
        {
            auto start = std::chrono::high_resolution_clock::now();

            res += puzzle.solve_part2_alternative();
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> part1_elapsed = end - start;

            std::cout << "Solved in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;
        }
    }
    return res;
};

int main(int argc, char *argv[])
{
    std::string filename = (argc > 1) ? argv[1] : "inputs/day" + SOLUTION_DAY + ".txt";
    try
    {
        std::cout << "Day " + SOLUTION_DAY + " Solution" << std::endl;
        auto lines = aoc::read_lines(filename);

        std::vector<Puzzle> puzzles;

        for (const auto &line : lines)
        {
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

        // BudgetCombinations combos = BudgetCombinations(4, 3);

        // while (true)
        // {
        //     auto [combos_ended, combo] = combos.next();

        //     std::string row;
        //     for (auto &v : combo)
        //     {
        //         row += std::to_string(v) + ',';
        //     }
        //     std::cout << row << std::endl;

        //     if (combos_ended)
        //         break;
        // }

        auto part1_start = std::chrono::high_resolution_clock::now();

        ll part1_result = solve_all(puzzles, true);
        auto part1_end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> part1_elapsed = part1_end - part1_start;

        std::cout << "Part 1: " << part1_result << " in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;

        // part 2
        auto part2_start = std::chrono::high_resolution_clock::now();

        ll part2_result = solve_all(puzzles, false);

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
