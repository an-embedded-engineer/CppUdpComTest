#include "SocketReceiveQueue.h"
#include "ThreadCancelledException.h"

SocketReceiveQueue& SocketReceiveQueue::GetInstance()
{
    static SocketReceiveQueue instance;
    return instance;
}

SocketReceiveQueue::SocketReceiveQueue()
    : m_Mutex()
    , m_ConditionVariable()
    , m_Queue()
{
    /* Nothing to do */
}

SocketReceiveQueue::~SocketReceiveQueue()
{
    std::unique_lock<std::mutex> lock(this->m_Mutex);

    this->m_Queue.clear();
}

void SocketReceiveQueue::Enqueue(SocketReceiveData&& data)
{
    std::unique_lock<std::mutex> lock(this->m_Mutex);

    this->m_Queue.emplace_back(std::move(data));

    this->m_ConditionVariable.notify_all();
}

void SocketReceiveQueue::Dequeue(SocketReceiveData& data, CancellationPoint& cp)
{
    while (this->m_Queue.empty())
    {
        cp.CheckCancelRequested();
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    std::unique_lock<std::mutex> lock(this->m_Mutex);

    SocketReceiveData&& item = std::move(this->m_Queue.front());

    data = std::move(item);

    this->m_Queue.pop_front();
}
