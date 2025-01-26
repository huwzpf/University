#include <iostream>
#include <fstream>
#include <vector>

#include "rfind.hpp"
#include "load_data.hpp"


int main() {
    std::string inputFilename = "input.txt"; 
    std::string outputFilename = "output-cpp-custom.txt";
    std::ofstream output_file(outputFilename);
    
    if (!output_file.is_open()) {
        return 1;
    }

    auto data = load_data(inputFilename);
    for (const auto& [str1, str2, start, end] : data) {
        int64_t ret = custom_rfind(str1, str2, start, end);
        output_file << ret << std::endl;
    }

    output_file.close();
    return 0;
}
