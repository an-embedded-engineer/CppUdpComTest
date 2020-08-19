#pragma once
#include "ExceptionBase.h"

namespace exception
{
    /* Application Exceptionクラス宣言 */
    class AppException : public ExceptionBase
    {
    public:
        /* コンストラクタ */
        AppException(const std::string& message);

        /* コンストラクタ */
        AppException(const std::string& message, const std::string& file, const std::string& func, const int line);

        /* デストラクタ */
        virtual ~AppException();

        /* エラー要因を取得 */
        virtual char const* what() const override;

    private:
        /* エラーメッセージ生成 */
        const std::string GenerateErrorMessage();

    private:
        /* エラーメッセージ */
        std::string m_ErrorMessage;
    };
}

#define THROW_APP_EXCEPTION(message) \
    throw exception::AppException(message, __FILE__, __FUNCTION__, __LINE__)

