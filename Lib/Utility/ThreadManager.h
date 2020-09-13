#pragma once
#include "CancellableThread.h"

#include <map>

/* Thread Manager�N���X�錾 */
class ThreadManager
{
public:
    /* �V���O���g���C���X�^���X�擾 */
    static ThreadManager& GetInstance();

private:
    /* �R���X�g���N�^ */
    ThreadManager();
    /* �f�X�g���N�^ */
    ~ThreadManager();

public:
    /* �R�s�[�R���X�g���N�^�폜 */
    ThreadManager(const ThreadManager&) = delete;
    /* ���[�u�R���X�g���N�^���폜 */
    ThreadManager(ThreadManager&&) = delete;
    /* �R�s�[����I�y���[�^�폜 */
    ThreadManager& operator=(const ThreadManager&) = delete;
    /* ���[�u����I�y���[�^�폜 */
    ThreadManager& operator=(ThreadManager&&) = delete;

public:
    /* �X���b�h�ǉ� */
    template<typename C, typename T>
    thread_id_t Add(const std::string& name, void(C::*fp)(T), C* obj)
    {
        return this->Add(name, std::bind(fp, obj, std::placeholders::_1, std::placeholders::_2));
    }

    /* �X���b�h�ǉ� */
    template<typename T>
    thread_id_t Add(const std::string& name, void(*fp)(T))
    {
        return this->Add(name, std::bind(fp, std::placeholders::_1, std::placeholders::_2));
    }

public:
    /* �X���b�h�ǉ� */
    thread_id_t Add(const std::string& name, ThreadFuncType thread_func);

    /* �X���b�h���݊m�F */
    bool IsExists(const thread_id_t thread_id);

    /* �X���b�h���擾 */
    const std::string GetName(const thread_id_t thread_id);

    /* �X���b�h�J�n */
    void Start(thread_id_t thread_id);
    /* �X���b�h��~ */
    void Stop(thread_id_t thread_id);
    /* �X���b�h���f�B�ʒm */
    void NotifyReady(thread_id_t thread_id);

    /* ���쒆�m�F */
    bool IsRunning(thread_id_t thread_id);

    /* �X���b�h�I���܂őҋ@ */
    void Join(thread_id_t thread_id);
    /* �X���b�h�̊Ǘ������ */
    void Detach(thread_id_t thread_id);

    /* �S�X���b�h�J�n */
    void StartAll();
    /* �S�X���b�h��~ */
    void StopAll();
    /* �S�X���b�h���f�B�ʒm */
    void NotifyReadyAll();

    /* �S�X���b�h�I���܂őҋ@ */
    void JoinAll();
    /* �S�X���b�h�̊Ǘ������ */
    void DetachAll();

private:
    /* ���̃X���b�hID���擾 */
    thread_id_t GetNextID();

private:
    /* �X���b�h�� */
    thread_id_t m_ThreadCount;

    /* �X���b�h�}�b�v */
    std::map<thread_id_t, CancellableThread> m_ThreadMap;

    /* �X���b�h���}�b�v */
    std::map<thread_id_t, std::string> m_ThreadNameMap;
};

