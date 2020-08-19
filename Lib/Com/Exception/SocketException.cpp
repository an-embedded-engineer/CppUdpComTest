#include "SocketException.h"
#include "StringFormat.h"
#include "StackTracer.h"

#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>

namespace exception
{
    /* コンストラクタ */
    SocketException::SocketException(const std::string& message, const int error_code)
        : ExceptionBase(message)
        , m_ErrorCode(error_code)
        , m_ErrorMessage("")
    {
        /* エラーメッセージ生成 */
        this->m_ErrorMessage = this->GenerateErrorMessage();
    }

    /* コンストラクタ */
    SocketException::SocketException(const std::string& message, const int error_code, const std::string& file, const std::string& func, const int line)
        : ExceptionBase(message, file, func, line)
        , m_ErrorCode(error_code)
        , m_ErrorMessage("")
    {
        /* エラーメッセージ生成 */
        this->m_ErrorMessage = this->GenerateErrorMessage();
    }

    /* デストラクタ */
    SocketException::~SocketException()
    {
        /* Nothing to do */
    }

    /* エラーコードを取得 */
    int SocketException::GetErrorCode()
    {
        return this->m_ErrorCode;
    }

    /* エラー要因を取得 */
    char const* SocketException::what() const noexcept
    {
        return this->m_ErrorMessage.c_str();
    }

    /* エラーメッセージ生成 */
    const std::string SocketException::GenerateErrorMessage()
    {
        /* スタックトレース情報を取得 */
        StackTrace stack_trace = StackTracer::GetStackTrace();

        std::stringstream ss;

        /* エラー情報がある場合は、エラー情報付きメッセージを生成 */
        if (this->m_IsErrorInfoExists == true)
        {
            ss << StringFormat("[Socket Error] %s : Error Code = %d @ %s[%s:L.%d]", this->m_Message, this->m_ErrorCode, this->m_FunctionName, this->m_FilePath, this->m_LineNumber);
        }
        else
        {
            ss << StringFormat("[Socket Error] %s : Error Code = %d", this->m_Message, this->m_ErrorCode);
        }
        ss << std::endl;

        /* スタックトレースをダンプ */
        ss << "[Stack Traces] : " << std::endl;
        for (uint32_t i = 0; i < stack_trace.trace_size; i++)
        {
            if (i != 0)
            {
                ss << std::endl;
            }

            ss << "  ";
            ss << std::setw(16) << std::setfill('0') << std::hex << std::showbase << stack_trace.traces[i];
            ss << " | ";
            ss << stack_trace.symbols[i];
        }
        ss << std::endl;

        return ss.str();
    }
}
