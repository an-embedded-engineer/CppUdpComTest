#pragma once
#include <string>
#include <cstdio>
#include <vector>

template <typename ... Args>
const std::string Format(const std::string& fmt, Args ... args)
{
    size_t str_len = snprintf(nullptr, 0, fmt.c_str(), args ...);

    size_t buffer_size = str_len + 1;

    std::vector<char> buffer(buffer_size);

    std::snprintf(&buffer[0], buffer_size, fmt.c_str(), args ...);

    std::string formated_str = std::string(buffer.begin(), buffer.end());

    return formated_str;
}
