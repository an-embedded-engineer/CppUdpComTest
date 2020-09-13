#include "ThreadManager.h"
#include "Logger.h"

/* �V���O���g���C���X�^���X�擾 */
ThreadManager& ThreadManager::GetInstance()
{
    static ThreadManager instance;
    return instance;
}

/* �R���X�g���N�^ */
ThreadManager::ThreadManager()
    : m_ThreadCount(0)
    , m_ThreadMap()
    , m_ThreadNameMap()
{
    /* Nothing to do */
}

/* �f�X�g���N�^ */
ThreadManager::~ThreadManager()
{
    /* Nothing to do */
}

/* �X���b�h�ǉ� */
thread_id_t ThreadManager::Add(const std::string& name, ThreadFuncType thread_func)
{
    /* ���̃X���b�hID���擾 */
    thread_id_t thread_id = this->GetNextID();

    /* �X���b�h�}�b�v��ID�ƃX���b�h��ǉ� */
    this->m_ThreadMap.emplace(std::piecewise_construct, std::forward_as_tuple(thread_id), std::forward_as_tuple(thread_id, name, thread_func));

    /* �X���b�h���}�b�v��ID�ƃX���b�h����ǉ� */
    this->m_ThreadNameMap.insert_or_assign(thread_id, name);

    return thread_id;
}

/* �X���b�h���݊m�F */
bool ThreadManager::IsExists(const thread_id_t thread_id)
{
    /* �X���b�hID���o�^����Ă��邩�m�F */
    if (this->m_ThreadMap.count(thread_id) > 0 && this->m_ThreadNameMap.count(thread_id) > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* �X���b�h���擾 */
const std::string ThreadManager::GetName(const thread_id_t thread_id)
{
    /* �X���b�h���o�^����Ă���ꍇ */
    if (this->IsExists(thread_id) == true)
    {
        /* �X���b�h���擾 */
        return this->m_ThreadMap[thread_id].GetName();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);

        return "";
    }
}

/* �X���b�h�J�n */
void ThreadManager::Start(thread_id_t thread_id)
{
    /* �X���b�h���o�^����Ă���ꍇ */
    if (this->IsExists(thread_id) == true)
    {
        /* �X���b�h�J�n */
        this->m_ThreadMap[thread_id].Start();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);
    }
}

/* �X���b�h��~ */
void ThreadManager::Stop(thread_id_t thread_id)
{
    /* �X���b�h���o�^����Ă���ꍇ */
    if (this->IsExists(thread_id) == true)
    {
        /* �X���b�h��~ */
        this->m_ThreadMap[thread_id].Stop();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);
    }
}

/* �X���b�h���f�B�ʒm */
void ThreadManager::NotifyReady(thread_id_t thread_id)
{
    /* �X���b�h���o�^����Ă���ꍇ */
    if (this->IsExists(thread_id) == true)
    {
        /* �X���b�h���f�B�ʒm */
        this->m_ThreadMap[thread_id].NotifyReady();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);
    }
}


/* �X���b�h�I���܂őҋ@ */
void ThreadManager::Join(thread_id_t thread_id)
{
    /* �X���b�h���o�^����Ă���ꍇ */
    if (this->IsExists(thread_id) == true)
    {
        /* �X���b�h�I���܂őҋ@ */
        this->m_ThreadMap[thread_id].Join();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);
    }
}

/* �X���b�h�̊Ǘ������ */
void ThreadManager::Detach(thread_id_t thread_id)
{
    /* �X���b�h���o�^����Ă���ꍇ */
    if (this->IsExists(thread_id) == true)
    {
        /* �X���b�h�̊Ǘ������ */
        this->m_ThreadMap[thread_id].Detach();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);
    }
}

/* ���쒆�m�F */
bool ThreadManager::IsRunning(thread_id_t thread_id)
{
    /* �X���b�h���o�^����Ă���ꍇ */
    if (this->IsExists(thread_id) == true)
    {
        /* �X���b�h���쒆�m�F */
        return this->m_ThreadMap[thread_id].IsRunning();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);

        return false;
    }
}

/* �S�X���b�h�J�n */
void ThreadManager::StartAll()
{
    /* �S�X���b�h�𑖍� */
    for (const auto& item : this->m_ThreadNameMap)
    {
        /* �X���b�hID���擾 */
        thread_id_t thread_id = item.first;

        /* �X���b�h���J�n */
        this->Start(thread_id);
    }
}

/* �S�X���b�h��~ */
void ThreadManager::StopAll()
{
    /* �S�X���b�h�𑖍� */
    for (const auto& item : this->m_ThreadNameMap)
    {
        /* �X���b�hID���擾 */
        thread_id_t thread_id = item.first;

        /* �X���b�h���~ */
        this->Stop(thread_id);
    }
}

/* �S�X���b�h���f�B�ʒm */
void ThreadManager::NotifyReadyAll()
{
    /* �S�X���b�h�𑖍� */
    for (const auto& item : this->m_ThreadNameMap)
    {
        /* �X���b�hID���擾 */
        thread_id_t thread_id = item.first;

        /* �X���b�h���f�B�ʒm */
        this->NotifyReady(thread_id);
    }
}


/* �S�X���b�h�I���܂őҋ@ */
void ThreadManager::JoinAll()
{
    /* �S�X���b�h�𑖍� */
    for (const auto& item : this->m_ThreadNameMap)
    {
        /* �X���b�hID���擾 */
        thread_id_t thread_id = item.first;

        /* ���b�h�I���܂őҋ@ */
        this->Join(thread_id);
    }
}

/* �S�X���b�h�̊Ǘ������ */
void ThreadManager::DetachAll()
{
    /* �S�X���b�h�𑖍� */
    for (const auto& item : this->m_ThreadNameMap)
    {
        /* �X���b�hID���擾 */
        thread_id_t thread_id = item.first;

        /* �X���b�h�̊Ǘ������ */
        this->Join(thread_id);
    }
}

/* ���̃X���b�hID���擾 */
thread_id_t ThreadManager::GetNextID()
{
    while (true)
    {
        /* �X���b�hID�C���N�������g(1�n�܂�) */
        this->m_ThreadCount++;

        /* �X���b�hID���o�^����Ă��Ȃ� */
        if (this->IsExists(this->m_ThreadCount) == false)
        {
            return this->m_ThreadCount;
        }
    }
}
