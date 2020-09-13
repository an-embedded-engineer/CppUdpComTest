#pragma once
#include "SocketDataTypes.h"
#include "SocketReceiveData.h"
#include "CancellationPoint.h"

#include <mutex>
#include <condition_variable>
#include <deque>

class SocketReceiveQueue
{
public:
    /* シングルトンインスタンス取得 */
    static SocketReceiveQueue& GetInstance();

private:
    /* コンストラクタ */
    SocketReceiveQueue();
    /* デストラクタ */
    ~SocketReceiveQueue();

public:
    /* コピーコンストラクタ削除 */
    SocketReceiveQueue(const SocketReceiveQueue&) = delete;
    /* ムーブコンストラクタ削除 */
    SocketReceiveQueue(SocketReceiveQueue&&) = delete;
    /* コピー代入演算子削除 */
    SocketReceiveQueue& operator=(const SocketReceiveQueue&) = delete;
    /* ムーブ代入演算子削除 */
    SocketReceiveQueue& operator=(SocketReceiveQueue&&) = delete;

public:
    void Enqueue(SocketReceiveData&& data);
    void Dequeue(SocketReceiveData& data, CancellationPoint& cp);

private:
    std::mutex m_Mutex;
    std::condition_variable m_ConditionVariable;
    std::deque<SocketReceiveData> m_Queue;
};

