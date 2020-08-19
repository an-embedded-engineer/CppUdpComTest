#include "AppException.h"
#include "StringFormat.h"

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
        /* エラー情報がある場合は、エラー情報付きメッセージを生成 */
        if (this->m_IsErrorInfoExists == true)
        {
            return Format("[Application Error] %s @ %s[%s:L.%d]", this->m_Message.c_str(), this->m_FunctionName.c_str(), this->m_FilePath.c_str(), this->m_LineNumber);
        }
        else
        {
            return Format("[Application Error] %s", this->m_Message.c_str());
        }
    }
}
