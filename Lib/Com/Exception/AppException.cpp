#include "AppException.h"
#include "StringFormat.h"
#include "StackTracer.h"

#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>

namespace exception
{
    /* コンストラクタ */
    AppException::AppException(const std::string& message)
        : ExceptionBase(message)
        , m_ErrorMessage("")
    {
        /* エラーメッセージ生成 */
        this->m_ErrorMessage = this->GenerateErrorMessage();
    }

    /* コンストラクタ */
    AppException::AppException(const std::string& message, const std::string& file, const std::string& func, const int line)
        : ExceptionBase(message, file, func, line)
        , m_ErrorMessage("")
    {
        /* エラーメッセージ生成 */
        this->m_ErrorMessage = this->GenerateErrorMessage();
    }

    /* デストラクタ */
    AppException::~AppException()
    {
        /* Nothing to do */
    }

    /* エラー要因を取得 */
    char const* AppException::what() const noexcept
    {
        /* エラー情報がある場合は、エラー情報付きメッセージを出力 */
        if (this->m_IsErrorInfoExists == true)
        {
            return this->m_ErrorMessage.c_str();
        }
        else
        {
            return this->m_Message.c_str();
        }
    }

    /* エラーメッセージ生成 */
    const std::string AppException::GenerateErrorMessage()
    {
        /* スタックトレース情報を取得 */
        StackTrace stack_trace = StackTracer::GetStackTrace();

        std::stringstream ss;

        /* エラー情報がある場合は、エラー情報付きメッセージを生成 */
        if (this->m_IsErrorInfoExists == true)
        {
            ss << StringFormat("[Application Error] %s @ %s[%s:L.%d]", this->m_Message, this->m_FunctionName, this->m_FilePath, this->m_LineNumber);
        }
        else
        {
            ss << StringFormat("[Application Error] %s", this->m_Message);
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
            ss << std::setw(16) << std::setfill('0') << std::hex << stack_trace.traces[i];
            ss << " | ";
            ss << stack_trace.symbols[i];
        }
        ss << std::endl;

        return ss.str();
    }
}
