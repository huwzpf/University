#include <fstream>
#include <sstream>
#include <vector>

void removeQuotes(std::string& str) {
    if (!str.empty() && str.front() == '"') {
        str.erase(0, 1);
    }
    if (!str.empty() && str.back() == '"') {
        str.pop_back();
    }
}

std::vector<std::tuple<std::string, std::string, int64_t, int64_t>> load_data(const std::string& inputFilename) {
    std::vector<std::tuple<std::string, std::string, int64_t, int64_t>> data;
    std::ifstream input_file(inputFilename);
    std::string line;

    if (!input_file.is_open()) {
        throw std::runtime_error("Could not open input file");
    }

    while (std::getline(input_file, line)) {
        std::stringstream ss(line);
        std::string str1, str2;
        int64_t num1, num2;
        bool got_num1 = false, got_num2 = false;
        std::getline(ss, str1, ',');
        std::getline(ss, str2, ',');
        removeQuotes(str1);
        removeQuotes(str2);

        if (ss >> num1) {
            got_num1 = true;
            ss.ignore();
            if (ss >> num2) {
                got_num2 = true;
            }
        }

        if(!got_num1)
        {
            num1 = 0; 
        }
        if(!got_num2)
        {
            num2 = -1; 
        }

        data.emplace_back(str1, str2, num1, num2);
    }
    
    input_file.close();
    return data;
}
