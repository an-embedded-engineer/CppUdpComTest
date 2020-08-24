#pragma once
#include "ExceptionBase.h"

namespace exception
{
    /* Socket Exceptionクラス宣言 */
    class SocketException : public ExceptionBase
    {
    public:
        /* コンストラクタ */
        SocketException(const std::string& message, const int error_code);

        /* コンストラクタ */
        SocketException(const std::string& message, const int error_code, const std::string& file, const std::string& func, const int line);

        /* デストラクタ */
        virtual ~SocketException();

        /* エラーコードを取得 */
        int GetErrorCode();

        /* エラー要因を取得 */
        virtual char const* what() const noexcept override;

    private:
        /* エラーメッセージ生成 */
        const std::string GenerateErrorMessage();

    private:
        /* エラーコード */
        int m_ErrorCode;

        /* エラーメッセージ */
        std::string m_ErrorMessage;
    };
}

/* Socket Exceptionのthrow */
#define THROW_SOCKET_EXCEPTION(message, error_code) \
    throw exception::SocketException(message, error_code, __FILE__, __FUNCTION__, __LINE__)

