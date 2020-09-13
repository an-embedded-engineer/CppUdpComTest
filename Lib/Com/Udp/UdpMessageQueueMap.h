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
    /* シングルトンインスタンス取得 */
    static UdpMessageQueueMap& GetInstance();

private:
    /* コンストラクタ */
    UdpMessageQueueMap();
    /* デストラクタ */
    ~UdpMessageQueueMap();

public:
    /* コピーコンストラクタ削除 */
    UdpMessageQueueMap(const UdpMessageQueueMap&) = delete;
    /* ムーブコンストラクタ削除 */
    UdpMessageQueueMap(UdpMessageQueueMap&&) = delete;
    /* コピー代入演算子削除 */
    UdpMessageQueueMap& operator=(const UdpMessageQueueMap&) = delete;
    /* ムーブ代入演算子削除 */
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
