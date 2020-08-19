#pragma once
#include "ExceptionBase.h"

namespace exception
{
    /* Application Exception�N���X�錾 */
    class AppException : public ExceptionBase
    {
    public:
        /* �R���X�g���N�^ */
        AppException(const std::string& message);

        /* �R���X�g���N�^ */
        AppException(const std::string& message, const std::string& file, const std::string& func, const int line);

        /* �f�X�g���N�^ */
        virtual ~AppException();

        /* �G���[�v�����擾 */
        virtual char const* what() const override;

    private:
        /* �G���[���b�Z�[�W���� */
        const std::string GenerateErrorMessage();

    private:
        /* �G���[���b�Z�[�W */
        std::string m_ErrorMessage;
    };
}

#define THROW_APP_EXCEPTION(message) \
    throw exception::AppException(message, __FILE__, __FUNCTION__, __LINE__)

