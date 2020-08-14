#pragma once

#include <exception>
#include <stdexcept>
#include <string>

class SocketException : public std::runtime_error
{
public:
    SocketException(const char* message, int error_code)
        : runtime_error(message)
        , m_ErrorCode(error_code)
        {
            /* Nothing to do */
        }
    
    SocketException(const std::string& message, int error_code)
        : runtime_error(message)
        , m_ErrorCode(error_code)
        {
            /* Nothing to do */
        }

    int GetErrorCode()
    {
        return this->m_ErrorCode;
    }

private:
    int m_ErrorCode;
};
