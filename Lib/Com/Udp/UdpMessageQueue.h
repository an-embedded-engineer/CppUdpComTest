#pragma once
#include "UdpMessage.h"

#include <deque>
#include <mutex>
#include <memory>

class UdpMessageQueue
{
public:
    UdpMessageQueue();

    ~UdpMessageQueue();

    bool Enqueue(UdpMessage&& message);

    bool Dequeue(UdpMessage& message);

    bool IsEmpty();

    size_t Size();

private:
    std::mutex m_Mutex;
    std::deque<UdpMessage> m_Queue;
};

