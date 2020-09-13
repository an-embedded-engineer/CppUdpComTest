#include "CancellableThread.h"
#include "StringFormat.h"
#include "Logger.h"

#include <stdexcept>
#include <iostream>

/* �R���X�g���N�^ */
CancellableThread::CancellableThread()
    : m_ID(0)
    , m_Name("")
    , m_Thread()
    , m_CancellationPoint(*this)
    , m_ThreadFunc(nullptr)
    , m_IsRunning(false)
{
    /* Nothing to do */
}

/* �R���X�g���N�^ */
CancellableThread::CancellableThread(const thread_id_t id, const std::string& name, ThreadFuncType thread_func)
    : m_ID(id)
    , m_Name(name)
    , m_Thread()
    , m_CancellationPoint(*this)
    , m_ThreadFunc(thread_func)
    , m_IsRunning(false)
{
    /* Nothing to do */
}

/* �X���b�h�J�n */
void CancellableThread::Start()
{
    /* �X���b�h�����s���łȂ� */
    if (this->m_IsRunning == false)
    {
        /* �X���b�h���s�֐���n���ăX���b�h���s�J�n */
        this->m_Thread = std::thread([this]()
            {
                this->m_ThreadFunc(*this, this->m_CancellationPoint);
            });

        /* �X���b�h���s���t���O�Z�b�g */
        this->m_IsRunning = true;
    }
    /* �X���b�h�����Ɏ��s�� */
    else
    {
        Logger::Error("Thread is already running : ID=%d Name=%s", this->m_ID, this->m_Name);
    }
}

/* �X���b�h��~ */
void CancellableThread::Stop()
{
    /* �X���b�h�����s�� */
    if (this->m_IsRunning == true)
    {
        /* �X���b�h�L�����Z�������N�G�X�g */
        this->m_CancellationPoint.RequestCancel();

        /* �X���b�h�I���܂őҋ@ */
        this->Join();
    }
    /* �X���b�h�����ɒ�~ */
    else
    {
        Logger::Warn("Thread is already stopped : ID=%d Name=%s", this->m_ID, this->m_Name);
    }
}

/* �X���b�h���f�B�ʒm */
void CancellableThread::NotifyReady()
{
    /* �X���b�h�����s�� */
    if (this->m_IsRunning == true)
    {
        /* �X���b�h���f�B�����N�G�X�g */
        this->m_CancellationPoint.RequestReady();
    }
    /* �X���b�h����~�� */
    else
    {
        Logger::Warn("Thread is not running : ID=%d Name=%s", this->m_ID, this->m_Name);
    }
}

/* �X���b�h�I���܂őҋ@ */
void CancellableThread::Join()
{
    /* �X���b�h�����s�� */
    if (this->m_IsRunning == true)
    {
        /* �X���b�h��Thread�I�u�W�F�N�g�Ɋ֘A�t�����Ă��邩���m�F */
        if (this->m_Thread.joinable() == true)
        {
            /* �X���b�h�I���܂őҋ@ */
            this->m_Thread.join();

            /* �X���b�h���s���t���O�N���A */
            this->m_IsRunning = false;
        }
    }
    /* �X���b�h����~�� */
    else
    {
        Logger::Warn("Thread is already joined : ID=%d Name=%s", this->m_ID, this->m_Name);
    }
}

/* �X���b�h�̊Ǘ������ */
void CancellableThread::Detach()
{
    /* �X���b�h�����s�� */
    if (this->m_IsRunning == true)
    {
        /* �X���b�h��Thread�I�u�W�F�N�g�Ɋ֘A�t�����Ă��邩���m�F */
        if (this->m_Thread.joinable() == true)
        {
            /* �X���b�h�̊Ǘ������ */
            this->m_Thread.detach();

            /* �X���b�h���s���t���O�N���A */
            this->m_IsRunning = false;
        }
    }
    /* �X���b�h����~�� */
    else
    {
        Logger::Warn("Thread is already joined : ID=%d Name=%s", this->m_ID, this->m_Name);
    }
}

/* �X���b�hID�擾 */
const thread_id_t CancellableThread::GetID()
{
    return this->m_ID;
}

/* �X���b�h���擾 */
const std::string& CancellableThread::GetName()
{
    return this->m_Name;
}

/* �X���b�h���s���m�F */
bool CancellableThread::IsRunning()
{
    return this->m_IsRunning;
}
