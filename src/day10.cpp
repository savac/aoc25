#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <unordered_set>
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
        res.insert(res.end(), res_remaining.begin(), res_remaining.end());

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

bool share_elements(const std::vector<int> &a, const std::vector<int> &b)
{
    for (int x : a)
    {
        for (int y : b)
        {
            if (x == y)
                return true;
        }
    }
    return false;
}

class Puzzle
{
public:
    std::vector<bool> target_lights;
    std::vector<std::vector<size_t>> buttons;
    std::vector<int> joltage;
    std::string puzzle_str;

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

        assert(puzzle.target_lights.size() == puzzle.joltage.size());

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

    static bool joltage_exceeded(
        std::vector<int> &candidate_constraints,
        std::vector<int> &joltage,
        std::vector<std::vector<size_t>> &buttons)
    {
        // sanity check
        std::vector<int> joltage_check(joltage.size(), 0);
        for (size_t i = 0; i < candidate_constraints.size(); ++i)
        {
            auto presses = candidate_constraints[i];
            if (presses > 0)
            {
                auto button = buttons[i];
                while (presses > 0)
                {
                    --presses;
                    for (auto &ind : button)
                    {
                        ++joltage_check[ind];
                    }
                }
            }
        }

        for (size_t i = 0; i < joltage.size(); ++i)
        {
            if (joltage_check[i] > joltage_check[i])
            {
                return true;
            }
        }
        return false;
    }

    static std::pair<bool, std::vector<std::vector<int>>> joltage_search(
        int schedule_position,
        std::vector<int> &schedule,
        std::vector<std::vector<int>> &indicator_to_buttons,
        std::vector<int> constraints,
        std::vector<int> &joltage,
        std::vector<int> free_button_inds,
        int &target_joltage)
    {
        auto indicator = schedule[schedule_position];
        auto buttons_inds = indicator_to_buttons[indicator];

        // std::cout << "Incoming constraints (indicator " + std::to_string(indicator) + "):" << std::endl;
        // aoc::print_vector(constraints);
        auto is_last_node = schedule_position == static_cast<int>(schedule.size()) - 1;
        if (is_last_node)
        {
            for (auto &button_ind : free_button_inds)
            {
                constraints[button_ind] = 0;
            }
            // we can freely distribute the unattributed_joltage to the remaining free buttons
            // assign all of it to the the first free button
            if (free_button_inds.size() > 0)
                constraints[free_button_inds[0]] = target_joltage;

            return {true, {constraints}};
        }

        // we are not at the last node from here on

        // collect all solutions
        std::vector<std::vector<int>> solutions = {};

        // candidate constraints
        BudgetCombinations combinations = BudgetCombinations(free_button_inds.size(), target_joltage);

        bool loop_control = true;
        while (loop_control)
        {
            auto [combinations_ended, combination] = combinations.next();
            loop_control = !combinations_ended;

            std::vector<int> candidate_constraints = constraints;
            for (size_t i = 0; i < free_button_inds.size(); ++i)
            {
                candidate_constraints[free_button_inds[i]] = combination[i];
            }

            auto next_indicator = schedule[schedule_position + 1];
            auto next_target_joltage = joltage[next_indicator];
            for (auto &button_ind : indicator_to_buttons[next_indicator])
            {
                if (candidate_constraints[button_ind] > 0)
                {
                    next_target_joltage -= candidate_constraints[button_ind];
                }
            }

            std::vector<int> next_free_button_inds;
            auto next_buttons_inds = indicator_to_buttons[next_indicator];
            for (auto &button_ind : next_buttons_inds)
            {
                // if has the special value of -1 it means it's unconstrained
                if (candidate_constraints[button_ind] < 0)
                {
                    next_free_button_inds.push_back(button_ind);
                }
            }

            if ((next_target_joltage > 0 && next_free_button_inds.size() > 0) || (next_target_joltage == 0))
            {

                // std::cout << "Candidate constraints (indicator " + std::to_string(indicator) + "):" << std::endl;
                // aoc::print_vector(candidate_constraints);
                auto [success, iteration_solutions] = joltage_search(
                    schedule_position + 1,
                    schedule,
                    indicator_to_buttons,
                    candidate_constraints,
                    joltage,
                    next_free_button_inds,
                    next_target_joltage
                );

                if (success)
                {
                    for (auto &solution : iteration_solutions)
                    {
                        solutions.push_back(solution);
                        // std::cout << "solution found: " << std::endl;
                        // aoc::print_vector(solution);
                    }
                    // return on first solution found
                    break;
                }
            }
            // else {
            //     std::cout << "Surge! Preemptive check!" << std::endl;
            // }
        }

        return {solutions.size() > 0, solutions};
    }

    int solve_part2()
    {
        // we'll traverse a graph where each node is an indicator
        // every indicator is associated with one or more buttons
        // the count of buttons per indicator defines where the nodes (indicators) are in the graph traversal schedule
        // the nodes (indicators) with fewer associated buttons must be visited first
        // we propagate any constraints as we traverse the graph; constraints are candidate number of presses for a specific button
        // at every node, taking into the account the constraints, we generate candidate number of presses for the unconstrained buttons
        // these come as a list of candidates which all should be iterated over
        // generated candidates must also comply with another constraint, the target joltage for the node (indicator)
        // each transition carries all the constraints, even for the button not associated with the node
        // if any of the nodes cannot generate candidate numbers for the unconstrained buttons
        // if we reach the final node and a valid candidate can be created we record the number of button presses and stop?

        std::cout << puzzle_str << std::endl;

        // -1 means it's still unconstrained
        std::vector<int> constraints(buttons.size(), -1);

        // rearrange buttons so that the ones associates with most indicators are first
        // these buttons are associated with fewer presses overall so it's worth
        // pressing them first
        std::sort(buttons.begin(), buttons.end(), [](const std::vector<size_t> &a, const std::vector<size_t> &b)
                  { return a.size() > b.size(); });

        // indicator to button look-up
        std::vector<std::vector<int>> indicator_to_buttons(joltage.size());
        for (size_t button_ind = 0; button_ind < buttons.size(); ++button_ind)
        {
            for (auto &indicator : buttons[button_ind])
            {
                indicator_to_buttons[indicator].push_back(button_ind);
            }
        }

        std::vector<int> schedule(joltage.size());
        for (size_t i = 0; i < schedule.size(); ++i)
        {
            schedule[i] = i;
        }

        // schedule based on indicator_to_buttons (i.e. how many buttons are associated with indicator)
        std::sort(schedule.begin(), schedule.end(),
                  [&indicator_to_buttons](size_t i1, size_t i2)
                  { return indicator_to_buttons[i1].size() < indicator_to_buttons[i2].size(); });

        // attempt to optimise the schedule so that the subsequent the indicator
        // has the fewest new buttons with respect to constrained_buttons set
        std::unordered_set<int> constrained_buttons;
        for (size_t i = 0; i < schedule.size() - 1; ++i)
        {
            for (auto &button_ind : indicator_to_buttons[schedule[i]])
            {
                constrained_buttons.insert(button_ind);
            }

            // find the indicator with the fewest new buttons with respect to constrained_buttons set
            int fewest_new_buttons = std::numeric_limits<int>::max();
            size_t best_position = i + 1;

            // look through remaining indicators
            for (size_t j = i + 1; j < schedule.size(); ++j)
            {
                auto &candidate_buttons = indicator_to_buttons[schedule[j]];

                // count how many buttons are not in constrained_buttons (i.e., new buttons)
                int new_button_count = 0;
                for (auto &button_ind : candidate_buttons)
                {
                    if (constrained_buttons.count(button_ind) == 0)
                    {
                        ++new_button_count;
                    }
                }

                if (new_button_count < fewest_new_buttons)
                {
                    fewest_new_buttons = new_button_count;
                    best_position = j;
                }
            }

            // swap the best candidate to position i+1
            if (best_position != i + 1)
            {
                std::swap(schedule[i + 1], schedule[best_position]);
            }
        }

        std::cout << "Scheduled indicator/buttons:" << std::endl;
        for (auto &ind : schedule)
        {
            std::cout << std::to_string(ind) << std::endl;
            aoc::print_vector(indicator_to_buttons[ind]);
        }
        std::cout << "vvvvvv" << std::endl;
        
        auto [success, solutions] = joltage_search(
            0,
            schedule,
            indicator_to_buttons,
            constraints,
            joltage,
            indicator_to_buttons[schedule[0]],
            joltage[schedule[0]]
        );

        if (success)
        {
            int min_presses = std::numeric_limits<int>::max();
            for (auto &solution : solutions)
            {
                // std::string row = "solution found: ";
                // for (auto &el : solution) {
                //     row += std::to_string(el) + ',';
                // }
                // std::cout << row << std::endl;

                // // sanity check
                // std::vector<int> joltage_check(joltage.size(), 0);
                // for (size_t i = 0; i < solution.size(); ++i) {
                //     auto presses = solution[i];
                //     auto button = buttons[i];
                //     while (presses > 0) {
                //         --presses;
                //         for (auto &ind : button) {
                //             ++joltage_check[ind];
                //         }
                //     }
                // }
                // std::cout << "joltage_check" << std::endl;
                // aoc::print_vector(joltage_check);
                // std::cout << "wanted joltage" << std::endl;
                // aoc::print_vector(joltage);
                // std::cout << "=========" << std::endl;

                auto presses = std::accumulate(solution.begin(), solution.end(), 0);
                if (presses < min_presses)
                    min_presses = presses;

                std::cout << "solution found: " + std::to_string(presses) << std::endl;
            }
            return min_presses;
        }
        else
        {
            std::cerr << "Solution not found." << std::endl;
            exit(1);
        }
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

            res += puzzle.solve_part2();
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> part1_elapsed = end - start;

            std::cout << "Solved in " << part1_elapsed.count() * 1e3 << " ms" << std::endl;
        }
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

        BudgetCombinations combos = BudgetCombinations(4, 3);

        while (true)
        {
            auto [combos_ended, combo] = combos.next();

            std::string row;
            for (auto &v : combo)
            {
                row += std::to_string(v) + ',';
            }
            std::cout << row << std::endl;

            if (combos_ended)
                break;
        }

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
}
