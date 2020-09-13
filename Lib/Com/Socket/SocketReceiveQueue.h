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
    /* �V���O���g���C���X�^���X�擾 */
    static SocketReceiveQueue& GetInstance();

private:
    /* �R���X�g���N�^ */
    SocketReceiveQueue();
    /* �f�X�g���N�^ */
    ~SocketReceiveQueue();

public:
    /* �R�s�[�R���X�g���N�^�폜 */
    SocketReceiveQueue(const SocketReceiveQueue&) = delete;
    /* ���[�u�R���X�g���N�^�폜 */
    SocketReceiveQueue(SocketReceiveQueue&&) = delete;
    /* �R�s�[������Z�q�폜 */
    SocketReceiveQueue& operator=(const SocketReceiveQueue&) = delete;
    /* ���[�u������Z�q�폜 */
    SocketReceiveQueue& operator=(SocketReceiveQueue&&) = delete;

public:
    void Enqueue(SocketReceiveData&& data);
    void Dequeue(SocketReceiveData& data, CancellationPoint& cp);

private:
    std::mutex m_Mutex;
    std::condition_variable m_ConditionVariable;
    std::deque<SocketReceiveData> m_Queue;
};

