#ifndef AOC_UTILS_H
#define AOC_UTILS_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace aoc
{

    // Read entire file into a string
    inline std::string read_file(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file: " + filename);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // Read file line by line into a vector
    inline std::vector<std::string> read_lines(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file: " + filename);
        }
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }
        return lines;
    }

    // Split string by delimiter
    inline std::vector<std::string> split(const std::string &str, char delimiter)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Trim whitespace from both ends
    inline std::string trim(const std::string &str)
    {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");
        return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
    }

    std::vector<std::pair<long long, long long>> parse_ranges(std::string_view line) {
        std::vector<std::pair<long long, long long>> ranges;
        while (!line.empty()) {
            auto comma = line.find(',');
            auto part = line.substr(0, comma);
            auto dash = part.find('-');

            std::string start_str(part.substr(0, dash));
            std::string end_str(part.substr(dash + 1));

            long long start = std::stoll(start_str);
            long long end = std::stoll(end_str);

            ranges.push_back({start, end});
            if (comma == std::string_view::npos) break;
            line.remove_prefix(comma + 1);
        }
        return ranges;
    }

    template <typename T>
    void print_vector(const std::vector<T>& vec) {
        std::cout << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << vec[i];
            if (i < vec.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }

} // namespace aoc

#endif // AOC_UTILS_H
