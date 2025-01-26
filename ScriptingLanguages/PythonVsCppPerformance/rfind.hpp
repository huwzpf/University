#include <cstdint>
#include <string>
#include <algorithm>
#include <iostream>
int64_t custom_rfind(const std::string& str, const std::string& sub,
                     int64_t start = 0, int64_t end = -1) 
{
    // Adjust the end parameter
    int64_t str_len = str.length();
    if (end == -1 || end > str_len) {
        end = str_len;
    }

    // Adjust negative indices
    if (start < 0) {
        start = std::max<int64_t>(0, str_len + start);
    }
    if (end < 0) {
        end = std::max<int64_t>(0, str_len + end);
    }

    // Check if search space is valid
    int64_t search_space_len = end - start;
    if (search_space_len < 0){
        return -1;
    }

    // Check if the substring is longer than the search space
    int64_t sub_len = sub.length();
    if (sub_len == 0) {
        return end;
    }
    if (sub_len > search_space_len) {
        return -1;
    }

    // Iterate over the string in reverse within the specified range
    for (int64_t i = end - sub_len; i >= start; --i) {
        if (str.substr(i, sub_len) == sub) {
            return i;
        }
    }
    return -1;
}
