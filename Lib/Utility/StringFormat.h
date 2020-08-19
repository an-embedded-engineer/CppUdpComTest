#pragma once
#include "AppException.h"

#include <string>
#include <cstdio>
#include <vector>
#include <iostream>
#include <memory>

#if 0

template <typename ... Args>
const std::string StringFormat(const std::string& fmt, Args ... args)
{
    size_t str_len = snprintf(nullptr, 0, fmt.c_str(), args ...);

    size_t buffer_size = str_len + 1;

    std::vector<char> buffer(buffer_size);

    std::snprintf(&buffer[0], buffer_size, fmt.c_str(), args ...);

    std::string formated_str = std::string(buffer.begin(), buffer.end());

    return formated_str;
}
#else

namespace detail
{
    template<typename T>
    auto Convert(T&& value)
    {
        if constexpr (std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value)
        {
            return std::forward<T>(value).c_str();
        }
        else
        {
            return std::forward<T>(value);
        }
    }

    template<typename ... Args>
    std::string StringFormatInternal(const std::string& format, Args&& ... args)
    {
        int size = std::snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args) ...) + 1;

        if (size <= 0)
        {
            THROW_APP_EXCEPTION("Formatting Error");
        }

        std::unique_ptr<char[]> buffer(new char[size]);

        std::snprintf(buffer.get(), size, format.c_str(), args ...);

        return std::string(buffer.get(), buffer.get() + size - 1);
    }
}

template<typename ... Args>
std::string StringFormat(const std::string& format, Args&& ... args)
{
    return detail::StringFormatInternal(format, detail::Convert(std::forward<Args>(args)) ...);
}

#endif

