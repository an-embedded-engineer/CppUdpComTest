#pragma once
#include <string>
#include "ThreadTypes.h"
#include "CancellationPoint.h"

#include <functional>
#include <thread>

/* Cancellable Thread�N���X */
class CancellableThread
{
public:
    /* �R���X�g���N�^ */
    CancellableThread();
    /* �R���X�g���N�^ */
    CancellableThread(const thread_id_t id, const std::string& name, ThreadFuncType thread_func);

    /* �X���b�h�J�n */
    void Start();
    /* �X���b�h��~ */
    void Stop();
    /* �X���b�h���f�B�ʒm */
    void NotifyReady();

    /* �X���b�h�I���܂őҋ@ */
    void Join();
    /* �X���b�h�̊Ǘ������ */
    void Detach();

    /* �X���b�hID�擾 */
    const thread_id_t GetID();
    /* �X���b�h���擾 */
    const std::string& GetName();
    /* �X���b�h���s���m�F */
    bool IsRunning();

private:
    /* �X���b�hID */
    thread_id_t m_ID;
    /* �X���b�h�� */
    std::string m_Name;
    /* Thread�I�u�W�F�N�g */
    std::thread m_Thread;
    /* Cancellation Point�I�u�W�F�N�g */
    CancellationPoint m_CancellationPoint;
    /* �X���b�h���s�֐� */
    ThreadFuncType m_ThreadFunc;
    /* �X���b�h���s���t���O */
    bool m_IsRunning;
};
