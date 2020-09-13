#pragma once
#include <mutex>
#include <condition_variable>

#include "ThreadCancelledException.h"
#include "Logger.h"


/* Cancellable Thread�N���X�O���錾 */
class CancellableThread;

/* Cancellation Point�N���X�錾 */
class CancellationPoint
{
public:
    /* �R���X�g���N�^ */
    CancellationPoint(CancellableThread& thread);

    /* �X���b�h�L�����Z�������N�G�X�g*/
    void RequestCancel();

    /* �L�����Z�����N�G�X�g�m�F */
    void CheckCancelRequested();

    /* �X���b�h���f�B�����N�G�X�g */
    void RequestReady();

    /* �X���b�h���f�B���N�G�X�g��ҋ@(�L�����Z���m�F�܂�) */
    void WaitReady();

    /* �X���b�h���f�B���N�G�X�g�𑊑Ύ��ԃ^�C���A�E�g�t���őҋ@(�L�����Z���m�F�܂�) */
    template <typename PeriodType>
    void WaitFor(const PeriodType& period)
    {
        /* Mutex�ɂ��r������ */
        std::unique_lock<std::mutex> lock(this->m_Mutex);

        /* �w�莞��(�Ăяo�����Ԃ���̑��Ύ���)�ɂȂ�܂őҋ@ */
        if (false == this->m_ConditionVariable.wait_for(lock, period, [this]
            {
                //Logger::Info("[%s] Check Cancel Request in Wait For Ready", this->m_ThreadName);

                /* �L�����Z���t���O�m�F */
                if (this->m_IsCancelled == true)
                {
                    Logger::Info("[%s] Cancelled in Wait For Ready", this->m_ThreadName);

                    /* �L�����Z����ʒm */
                    this->NotifyCancel();
                }

                return this->m_IsReady;
            }))
        {
            /* �^�C���A�E�g����܂Ń��f�B���N�G�X�g�Ȃ� */
            Logger::Info("[%s] Cancelled by Timeout in Wait For Ready", this->m_ThreadName);

            /* �L�����Z����ʒm */
            this->NotifyCancel();
        }
        /* �^�C���A�E�g����O�Ƀ��f�B���N�G�X�g���� */
        else
        {
            return;
        }
    }

    /* �X���b�h���f�B���N�G�X�g�𑊑Ύ��ԃ^�C���A�E�g�t���őҋ@(�L�����Z���m�F�܂�) */
    template <typename PeriodType>
    void WaitUntil(const PeriodType& period)
    {
        /* Mutex�ɂ��r������ */
        std::unique_lock<std::mutex> lock(this->m_Mutex);

        /* �w�莞��(�Ăяo�����Ɏw�肵����Ύ���)�ɂȂ�܂őҋ@ */
        if (false == this->m_ConditionVariable.wait_until(lock, period, [this]
            {
                //Logger::Info("[%s] Check Cancel Request in Wait Until Ready", this->m_ThreadName);

                /* �L�����Z���t���O�m�F */
                if (this->m_IsCancelled == true)
                {
                    Logger::Info("[%s] Cancelled in Wait Until Ready", this->m_ThreadName);

                    /* �L�����Z����ʒm */
                    this->NotifyCancel();
                }

                return this->m_IsReady;
            }))
        {
            /* �^�C���A�E�g����܂Ń��f�B���N�G�X�g�Ȃ� */
            Logger::Info("[%s] Cancelled by Timeout in Wait Until Ready", this->m_ThreadName);

            /* �L�����Z����ʒm */
            this->NotifyCancel();
        }
            /* �^�C���A�E�g����O�Ƀ��f�B���N�G�X�g���� */
        else
        {
            return;
        }
    }

private:
    /* �X���b�h�L�����Z���ʒm */
    void NotifyCancel();

private:
    /* Cancellable Thread�N���X�C���X�^���X */
    CancellableThread& m_Thread;
    /* �X���b�h�� */
    std::string m_ThreadName;
    /* �L�����Z���t���O */
    bool m_IsCancelled;
    /* ���f�B�t���O */
    bool m_IsReady;
    /* Mutex */
    std::mutex m_Mutex;
    /* Condition Variable */
    std::condition_variable m_ConditionVariable;
};
