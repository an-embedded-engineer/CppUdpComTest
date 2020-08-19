#include "AppException.h"
#include "StringFormat.h"

namespace exception
{
    /* �R���X�g���N�^ */
    AppException::AppException(const std::string& message)
        : ExceptionBase(message)
        , m_ErrorMessage("")
    {
        /* �G���[���b�Z�[�W���� */
        this->m_ErrorMessage = this->GenerateErrorMessage();
    }

    /* �R���X�g���N�^ */
    AppException::AppException(const std::string& message, const std::string& file, const std::string& func, const int line)
        : ExceptionBase(message, file, func, line)
        , m_ErrorMessage("")
    {
        /* �G���[���b�Z�[�W���� */
        this->m_ErrorMessage = this->GenerateErrorMessage();
    }

    /* �f�X�g���N�^ */
    AppException::~AppException()
    {
        /* Nothing to do */
    }

    /* �G���[�v�����擾 */
    char const* AppException::what() const
    {
        /* �G���[��񂪂���ꍇ�́A�G���[���t�����b�Z�[�W���o�� */
        if (this->m_IsErrorInfoExists == true)
        {
            return this->m_ErrorMessage.c_str();
        }
        else
        {
            return this->m_Message.c_str();
        }
    }

    /* �G���[���b�Z�[�W���� */
    const std::string AppException::GenerateErrorMessage()
    {
        /* �G���[��񂪂���ꍇ�́A�G���[���t�����b�Z�[�W�𐶐� */
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
