#include "ExceptionBase.h"

#include <sstream>

namespace exception
{
    /* コンストラクタ */
    ExceptionBase::ExceptionBase(const std::string& message)
        : m_Message(message)
        , m_FilePath("")
        , m_FunctionName("")
        , m_LineNumber(0)
        , m_ErrorInfo("", "", 0)
        , m_IsErrorInfoExists(false)
    {
        /* Nothing to do */
    }

    /* コンストラクタ */
    ExceptionBase::ExceptionBase(const std::string& message, const std::string& file, const std::string& func, const int line)
        : m_Message(message)
        , m_FilePath(file)
        , m_FunctionName(func)
        , m_LineNumber(line)
        , m_ErrorInfo(file, func, line)
        , m_IsErrorInfoExists(true)
    {
        /* Nothing to do */
    }

    /* デストラクタ */
    ExceptionBase::~ExceptionBase()
    {
        /* Nothing to do */
    }

    /* ファイルパスを取得 */
    const std::string& ExceptionBase::GetFilePath()
    {
        return this->m_FilePath;
    }

    /* 関数名を取得 */
    const std::string& ExceptionBase::GetFunctionName()
    {
        return this->m_FunctionName;
    }

    /* ファイル行番号を取得 */
    int ExceptionBase::GetLineNumber()
    {
        return this->m_LineNumber;
    }

    /* エラー情報を取得 */
    const ErrorInfo& ExceptionBase::GetErrorInfo()
    {
        return this->m_ErrorInfo;
    }

    /* エラー要因を取得 */
    char const* ExceptionBase::what() const noexcept
    {
        /* エラー情報が存在する場合 */
        if (this->m_IsErrorInfoExists == true)
        {
            std::stringstream ss;

            /* エラーメッセージに、エラー発生箇所(関数名、ファイルパス、ファイル行数)を付加 */
            ss << this->m_Message << " @ " << this->m_FunctionName << "[" << this->m_FilePath << ": L." << this->m_LineNumber << "]";

            std::string message = ss.str();

            return message.c_str();
        }
        /* エラー情報が存在しない場合 */
        else
        {
            /* エラーメッセージのみ出力 */
            return this->m_Message.c_str();
        }
    }
}
