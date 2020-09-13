#include "AsyncTaskManager.h"

/* シングルトンインスタンス取得 */
AsyncTaskManager& AsyncTaskManager::GetInstance()
{
    static AsyncTaskManager instance;
    return instance;
}

/* コンストラクタ */
AsyncTaskManager::AsyncTaskManager()
    : m_ThreadManager(ThreadManager::GetInstance())
{
    /* Nothin to do */
}

/* デストラクタ */
AsyncTaskManager::~AsyncTaskManager()
{
    /* Nothing to do */
}

void AsyncTaskManager::Start(const task_id_t task_id)
{
    this->m_ThreadManager.Start(task_id);
}

void AsyncTaskManager::Stop(const task_id_t task_id)
{
    this->m_ThreadManager.Stop(task_id);

    while (true)
    {
        if (this->m_ThreadManager.IsRunning(task_id) == true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        else
        {
            break;
        }
    }
}

void AsyncTaskManager::Join(const task_id_t task_id)
{
    this->m_ThreadManager.Join(task_id);
}

void AsyncTaskManager::Detach(const task_id_t task_id)
{
    this->m_ThreadManager.Detach(task_id);
}

bool AsyncTaskManager::IsRunning(const task_id_t task_id)
{
    return this->m_ThreadManager.IsRunning(task_id);
}
