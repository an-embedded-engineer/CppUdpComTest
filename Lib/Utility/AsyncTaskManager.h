#pragma once
#include "ThreadManager.h"
#include "Logger.h"

#include <functional>

enum class LoopTaskResult
{
    Continue,
    Exit,
};

using task_id_t = thread_id_t;

using AsyncTask = CancellableThread;

using AsyncTaskFuncType = std::function<void(AsyncTask&, CancellationPoint&)>;

using AsyncLoopTaskType = std::function<LoopTaskResult(AsyncTask&, CancellationPoint&)>;

/* Asynchronous Task Manager */
class AsyncTaskManager
{
public:
    /* シングルトンインスタンス取得 */
    static AsyncTaskManager& GetInstance();

private:
    /* コンストラクタ */
    AsyncTaskManager();
    /* デストラクタ */
    ~AsyncTaskManager();

public:
    /* コピーコンストラクタ削除 */
    AsyncTaskManager(const AsyncTaskManager&) = delete;
    /* ムーブコンストラクタを削除 */
    AsyncTaskManager(AsyncTaskManager&&) = delete;
    /* コピー代入オペレータ削除 */
    AsyncTaskManager& operator=(const AsyncTaskManager&) = delete;
    /* ムーブ代入オペレータ削除 */
    AsyncTaskManager& operator=(AsyncTaskManager&&) = delete;

public:
    template<typename T>
    task_id_t AddTask(const std::string& name, void(T::*fp)(AsyncTask& task, CancellationPoint& cp), T* obj)
    {
        return this->AddTask(name, std::bind(fp, obj, std::placeholders::_1, std::placeholders::_2));
    }

    task_id_t AddTask(const std::string& name, void(*fp)(AsyncTask& task, CancellationPoint& cp))
    {
        return this->AddTask(name, std::bind(fp, std::placeholders::_1, std::placeholders::_2));
    }

    template<typename T, typename PeriodType>
    task_id_t AddLoopTask(const std::string& name, LoopTaskResult(T::* fp)(AsyncTask& task, CancellationPoint& cp), T* obj, PeriodType period)
    {
        return this->AddLoopTask(name, std::bind(fp, obj, std::placeholders::_1, std::placeholders::_2), period);
    }

    template<typename PeriodType>
    task_id_t AddLoopTask(const std::string& name, void(*fp)(AsyncTask& task, CancellationPoint& cp), PeriodType period)
    {
        return this->AddLoopTask(name, std::bind(fp, std::placeholders::_1, std::placeholders::_2), period);
    }

    task_id_t AddTask(const std::string& name, AsyncTaskFuncType task_func)
    {
        task_id_t task_id = this->m_ThreadManager.Add(name, [this, task_func](AsyncTask& task, CancellationPoint& cp)
        {
            try
            {
                task_func(task, cp);
            }
            catch (ThreadCancelledException&)
            {
                Logger::Warn("Thread Cancelled : ID=%d Name=%s", task.GetID(), task.GetName());
            }
        });

        return task_id;
    }

    template <typename PeriodType>
    task_id_t AddLoopTask(const std::string& name, AsyncLoopTaskType task_func, PeriodType period)
    {
        task_id_t task_id = this->m_ThreadManager.Add(name, [this, task_func, period](AsyncTask& task, CancellationPoint& cp)
        {
            try
            {
                while (true)
                {
                    try
                    {
                        cp.CheckCancelRequested();

                        LoopTaskResult result = task_func(task, cp);

                        if (result == LoopTaskResult::Exit)
                        {
                            break;
                        }

                        std::this_thread::sleep_for(period);
                    }
                    catch (const std::exception& ex)
                    {
                        Logger::Error("Exception Occurred : %s", ex.what());
                    }
                }
            }
            catch (ThreadCancelledException& ex)
            {
                Logger::Warn("Thread Cancelled : ID=%d Name=%s", task.GetID(), task.GetName());
            }
        });

        return task_id;
    }

    void Start(const task_id_t task_id);

    void Stop(const task_id_t task_id);

    void Join(const task_id_t task_id);

    void Detach(const task_id_t task_id);

    bool IsRunning(const task_id_t task_id);

private:
    ThreadManager& m_ThreadManager;
};

