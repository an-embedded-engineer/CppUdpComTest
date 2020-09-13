#pragma once
#include "SocketDataTypes.h"
#include "UdpMessage.h"
#include "UdpMessageQueue.h"

#include <map>
#include <mutex>
#include <vector>


class UdpMessageQueueMap
{
public:
    /* �V���O���g���C���X�^���X�擾 */
    static UdpMessageQueueMap& GetInstance();

private:
    /* �R���X�g���N�^ */
    UdpMessageQueueMap();
    /* �f�X�g���N�^ */
    ~UdpMessageQueueMap();

public:
    /* �R�s�[�R���X�g���N�^�폜 */
    UdpMessageQueueMap(const UdpMessageQueueMap&) = delete;
    /* ���[�u�R���X�g���N�^�폜 */
    UdpMessageQueueMap(UdpMessageQueueMap&&) = delete;
    /* �R�s�[������Z�q�폜 */
    UdpMessageQueueMap& operator=(const UdpMessageQueueMap&) = delete;
    /* ���[�u������Z�q�폜 */
    UdpMessageQueueMap& operator=(UdpMessageQueueMap&&) = delete;

public:
    bool Enqueue(uint16_t message_id, UdpMessage&& message);

    bool Dequeue(uint16_t message_id, UdpMessage& message);

    bool IsEmpty(uint16_t message_id);

    size_t Size(uint16_t message_id);

    const std::vector<uint16_t>& GetKeys();
private:
    std::mutex m_Mutex;
    std::map<uint16_t, UdpMessageQueue> m_Map;
    std::vector<uint16_t> m_Keys;
};
