#include "CancellationPoint.h"
#include "CancellableThread.h"

/* �R���X�g���N�^ */
CancellationPoint::CancellationPoint(CancellableThread& thread)
    : m_Thread(thread)
    , m_ThreadName("")
    , m_IsCancelled(false)
    , m_IsReady(false)
    , m_Mutex()
    , m_ConditionVariable()
{
    /* �X���b�h�����Z�b�g */
    this->m_ThreadName = this->m_Thread.GetName();
}

/* �X���b�h�L�����Z�������N�G�X�g*/
void CancellationPoint::RequestCancel()
{
    /* Mutex�ɂ��r������ */
    std::unique_lock<std::mutex> lock(this->m_Mutex);

    Logger::Info("[%s] Cancel Request Accepted", this->m_ThreadName);

    /* �L�����Z���t���O�Z�b�g */
    this->m_IsCancelled = true;

    /* �ҋ@�X���b�h�����ׂċN�������� */
    this->m_ConditionVariable.notify_all();
}

/* �L�����Z�����N�G�X�g�m�F */
void CancellationPoint::CheckCancelRequested()
{
    /* Mutex�ɂ��r������ */
    std::unique_lock<std::mutex> lock(this->m_Mutex);

    //Logger::Info("[%s] Check Cancel Request", this->m_ThreadName);

    /* �L�����Z���t���O�m�F */
    if (this->m_IsCancelled == true)
    {
        Logger::Info("[%s] Cancelled in Thread Operation", this->m_ThreadName);

        /* �L�����Z����ʒm */
        this->NotifyCancel();
    }
}

/* �X���b�h���f�B�����N�G�X�g */
void CancellationPoint::RequestReady()
{
    /* Mutex�ɂ��r������ */
    std::unique_lock<std::mutex> lock(this->m_Mutex);

    Logger::Info("[%s] Read Request Accepted", this->m_ThreadName);

    /* ���f�B�t���O���Z�b�g */
    this->m_IsReady = true;

    /* �ҋ@�X���b�h�����ׂċN�������� */
    this->m_ConditionVariable.notify_all();
}

/* �X���b�h���f�B���N�G�X�g��ҋ@(�L�����Z���m�F�܂�) */
void CancellationPoint::WaitReady()
{
    /* Mutex�ɂ��r������ */
    std::unique_lock<std::mutex> lock(this->m_Mutex);

    /* ���f�B�t���O�����܂őҋ@ */
    this->m_ConditionVariable.wait(lock,
        [this]
        {
            //Logger::Info("[%s] Check Cancel Request in Wait Ready", this->m_ThreadName);

            /* �L�����Z���t���O�m�F */
            if (this->m_IsCancelled == true)
            {
                Logger::Info("[%s] Cancelled in Wait Ready", this->m_ThreadName);

                /* �L�����Z����ʒm */
                this->NotifyCancel();
            }

            return this->m_IsReady;
        });
}

/* �X���b�h�L�����Z���ʒm */
void CancellationPoint::NotifyCancel()
{
    /* �L�����Z���t���O�N���A */
    this->m_IsCancelled = false;

    /* ���f�B�t���O�N���A */
    this->m_IsReady = false;

    Logger::Info("[%s] Throw Cancelld Exception", this->m_ThreadName);

    /* Cancelled Exception��throw */
    throw ThreadCancelledException(this->m_Thread);
}
