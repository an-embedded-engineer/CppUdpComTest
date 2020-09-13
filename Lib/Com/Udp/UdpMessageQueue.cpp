#include "UdpMessageQueue.h"

UdpMessageQueue::UdpMessageQueue()
    : m_Mutex()
    , m_Queue()
{
    /* Nothing to do */
}

UdpMessageQueue::~UdpMessageQueue()
{
    this->m_Queue.clear();
}

bool UdpMessageQueue::Enqueue(UdpMessage&& message)
{
    std::lock_guard<std::mutex> lock(this->m_Mutex);

    this->m_Queue.emplace_back(std::move(message));

    return true;
}

bool UdpMessageQueue::Dequeue(UdpMessage& message)
{
    std::lock_guard<std::mutex> lock(this->m_Mutex);

    if (this->m_Queue.empty() == true)
    {
        return false;
    }
    else
    {
        UdpMessage&& item = std::move(this->m_Queue.front());

        message = std::move(item);

        this->m_Queue.pop_front();

        return true;
    }
}

bool UdpMessageQueue::IsEmpty()
{
    std::lock_guard<std::mutex> lock(this->m_Mutex);

    return (this->m_Queue.empty());
}

size_t UdpMessageQueue::Size()
{
    std::lock_guard<std::mutex> lock(this->m_Mutex);

    return this->m_Queue.size();
}
