#include "UdpMessageQueueMap.h"
#include "AppException.h"


UdpMessageQueueMap& UdpMessageQueueMap::GetInstance()
{
    static UdpMessageQueueMap instance;
    return instance;
}

/* コンストラクタ */
UdpMessageQueueMap::UdpMessageQueueMap()
    : m_Mutex()
    , m_Map()
{

}

/* デストラクタ */
UdpMessageQueueMap::~UdpMessageQueueMap()
{
    this->m_Map.clear();
}


bool UdpMessageQueueMap::Enqueue(uint16_t message_id, UdpMessage&& message)
{
    std::lock_guard<std::mutex> lock(this->m_Mutex);

    if (this->m_Map.count(message_id) == 0)
    {
        this->m_Map.emplace(std::piecewise_construct, std::forward_as_tuple(message_id), std::forward_as_tuple());

        this->m_Keys.push_back(message_id);
    }

    return this->m_Map[message_id].Enqueue(std::move(message));
}

bool UdpMessageQueueMap::Dequeue(uint16_t message_id, UdpMessage& message)
{
    std::lock_guard<std::mutex> lock(this->m_Mutex);

    if (this->m_Map.count(message_id) > 0)
    {
        return this->m_Map[message_id].Dequeue(message);
    }
    else
    {
        return false;
    }
}

bool UdpMessageQueueMap::IsEmpty(uint16_t message_id)
{
    std::lock_guard<std::mutex> lock(this->m_Mutex);

    if (this->m_Map.count(message_id) > 0)
    {
        return this->m_Map[message_id].IsEmpty();
    }
    else
    {
        return true;
    }
}

size_t UdpMessageQueueMap::Size(uint16_t message_id)
{
    std::lock_guard<std::mutex> lock(this->m_Mutex);

    if (this->m_Map.count(message_id) > 0)
    {
        return this->m_Map[message_id].Size();
    }
    else
    {
        return 0;
    }
}

const std::vector<uint16_t>& UdpMessageQueueMap::GetKeys()
{
    return this->m_Keys;
}

