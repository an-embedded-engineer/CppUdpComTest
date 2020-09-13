#include "ThreadCancelledException.h"
#include "CancellableThread.h"
#include "StringFormat.h"

/* �R���X�g���N�^ */
ThreadCancelledException::ThreadCancelledException(CancellableThread& thread)
    : m_ErrorMessage("")
{
    /* �f�t�H���g������ŃG���[���b�Z�[�W���� */
    this->m_ErrorMessage = this->GenerateErrorMessage(thread, "Thread Cancelled");
}

/* �R���X�g���N�^ */
ThreadCancelledException::ThreadCancelledException(CancellableThread& thread, const std::string& message)
    : m_ErrorMessage("")
{
    /* �w�蕶����ŃG���[���b�Z�[�W���� */
    this->m_ErrorMessage = this->GenerateErrorMessage(thread, message);
}

/* ���b�Z�[�W���e���擾 */
const char* ThreadCancelledException::what() const noexcept
{
    return this->m_ErrorMessage.c_str();
}

/* �G���[���b�Z�[�W���� */
std::string ThreadCancelledException::GenerateErrorMessage(CancellableThread& thread, const std::string& message)
{
    return StringFormat("[Cancelled Exception] %s : ID=%d Name=%s", message, thread.GetID(), thread.GetName());
}
