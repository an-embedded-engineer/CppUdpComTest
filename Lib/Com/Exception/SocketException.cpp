#include "SocketException.h"
#include "StringFormat.h"

namespace exception
{
    /* �R���X�g���N�^ */
    SocketException::SocketException(const std::string& message, const int error_code)
        : ExceptionBase(message)
        , m_ErrorCode(error_code)
        , m_ErrorMessage("")
    {
        /* �G���[���b�Z�[�W���� */
        this->m_ErrorMessage = this->GenerateErrorMessage();
    }

    /* �R���X�g���N�^ */
    SocketException::SocketException(const std::string& message, const int error_code, const std::string& file, const std::string& func, const int line)
        : ExceptionBase(message, file, func, line)
        , m_ErrorCode(error_code)
        , m_ErrorMessage("")
    {
        /* �G���[���b�Z�[�W���� */
        this->m_ErrorMessage = this->GenerateErrorMessage();
    }

    /* �f�X�g���N�^ */
    SocketException::~SocketException()
    {
        /* Nothing to do */
    }

    /* �G���[�R�[�h���擾 */
    int SocketException::GetErrorCode()
    {
        return this->m_ErrorCode;
    }

    /* �G���[�v�����擾 */
    char const* SocketException::what() const
    {
        return this->m_ErrorMessage.c_str();
    }

    /* �G���[���b�Z�[�W���� */
    const std::string SocketException::GenerateErrorMessage()
    {
        /* �G���[��񂪂���ꍇ�́A�G���[���t�����b�Z�[�W�𐶐� */
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
