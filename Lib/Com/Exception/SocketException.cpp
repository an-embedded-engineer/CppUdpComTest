#include "SocketException.h"
#include "StringFormat.h"

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
        /* エラー情報がある場合は、エラー情報付きメッセージを生成 */
        if (this->m_IsErrorInfoExists == true)
        {
            return Format("[Socket Error] %s : Error Code = %d @ %s[%s:L.%d]", this->m_Message.c_str(), this->m_ErrorCode, this->m_FunctionName.c_str(), this->m_FilePath.c_str(), this->m_LineNumber);
        }
        else
        {
            return Format("[Socket Error] %s : Error Code = %d", this->m_Message.c_str(), this->m_ErrorCode);
        }
    }
}
