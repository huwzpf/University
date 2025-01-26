#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <numeric>
#include <cmath>
#include <algorithm>

#include "rfind.hpp"
#include "load_data.hpp"

int64_t get_execution_time(int n, const std::vector<std::tuple<std::string, std::string, int64_t, int64_t>> data)
{
    // Use an integer to count the total duration in nanoseconds
    int64_t total_duration_ns = 0;
    for (const auto& [str1, str2, start, end] : data) {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < n; i++)
        {
            int64_t ret = custom_rfind(str1, str2, start, end);
        }
        
        auto stop_time = std::chrono::high_resolution_clock::now();
        // Calculate the duration of this execution in nanoseconds
        auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
        total_duration_ns += duration_ns;
    }

    return total_duration_ns;
}

int main() {
    std::string input_filename = "input.txt";
    double clock_precision_ns = static_cast<double>(
        std::chrono::high_resolution_clock::period::num) / 
        std::chrono::high_resolution_clock::period::den * 1e9;
    std::cout << "Clock precision: " << clock_precision_ns << " nanoseconds" << std::endl;

    auto data = load_data(input_filename);
    for (int n = 1; n <=1000; n*=10)
    {
        int64_t total_duration_ns = get_execution_time(n, data);
        double relative_error = (clock_precision_ns * data.size()) / total_duration_ns;
        std::cout << "Custom C++ function - N = " << n << " -  Total execution time = " << total_duration_ns << " ns - Relative error = " << relative_error << std::endl;
    }

    return 0;
}
