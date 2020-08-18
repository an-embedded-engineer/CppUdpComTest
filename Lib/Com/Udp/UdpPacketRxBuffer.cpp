#include "UdpPacketRxBuffer.h"

#include <stdexcept>
#include <iostream>

/* コンストラクタ */
UdpPacketRxBuffer::UdpPacketRxBuffer()
    : m_MessageID(0)
    , m_Buffer(nullptr)
    , m_TotalSize(0)
    , m_TotalBlockNum(0)
    , m_ReceivedBlockNum(0)
    , m_RecvStatus()
    , m_IsCompleted(false)
    , m_Callback(nullptr)
{
    /* Nothing to do */
}

/* デストラクタ */
UdpPacketRxBuffer::~UdpPacketRxBuffer()
{
    /* Nothing to do */
}

/* 受信コールバックの登録 */
void UdpPacketRxBuffer::RegisterCallback(CallbackType& callback)
{
    /* 受信コールバック未登録 */
    if (this->m_Callback == nullptr)
    {
        /* 受信コールバックを登録 */
        this->m_Callback = callback;
    }
    else
    {
        throw std::logic_error("Callback is already registered");
    }
}

/* コールバック呼び出しのリクエスト */
void UdpPacketRxBuffer::RequestCallback()
{
    /* コールバック登録確認 */
    if (this->m_Callback != nullptr)
    {
        /* 受信完了確認 */
        if (this->IsCompleted() == true)
        {
            /* 受信データ&サイズ確認 */
            if (this->m_Buffer != nullptr && this->m_TotalSize > 0)
            {
                /* 受信コールバック呼び出し */
                this->m_Callback(this->m_Buffer, this->m_TotalSize);
            }
            else
            {
                throw std::logic_error("Rx Buffer & Size are invalid");
            }
        }
        else
        {
            throw std::logic_error("Rx Buffer is not completed");
        }
    }
    else
    {
        std::cerr << "[WARN] Callback is not registered" << std::endl;
    }
}

/* UDP Packetの追加 */
void UdpPacketRxBuffer::Add(const UdpPacket& udp_packet)
{
    /* 1番目のパケット */
    if (udp_packet.Header.CurrentBlockNum == 1)
    {
        /* 1番目のパケットを追加 */
        this->AddFirstPacket(udp_packet);
    }
    else
    {
        /* 2番目以降のパケットを追加 */
        this->AddContinuousPacket(udp_packet);
    }

    /* 受信完了確認 */
    if (this->CheckCompleted() == true)
    {
        /* 受信完了状態更新 */
        this->m_IsCompleted = true;
    }
}

/* 受信バッファ取得 */
void UdpPacketRxBuffer::GetBuffer(byte_ptr& data_ptr, size_t& data_size)
{
    /* 受信バッファセット */
    data_ptr = this->m_Buffer;

    /* 受信データサイズセット */
    data_size = this->m_TotalSize;
}

/* 受信バッファの解放 */
void UdpPacketRxBuffer::Release()
{
    /* メッセージIDクリア */
    this->m_MessageID = 0;

    /* 合計データサイズクリア */
    this->m_TotalSize = 0;

    /* 合計ブロック数クリア */
    this->m_TotalBlockNum = 0;

    /* 受信ブロック数クリア */
    this->m_ReceivedBlockNum = 0;

    /* 受信ステータスクリア */
    this->m_RecvStatus.clear();

    /* 受信バッファ解放 */
    free(this->m_Buffer);

    /* 受信バッファアドレスクリア */
    this->m_Buffer = nullptr;

    /* 受信完了状態クリア */
    this->m_IsCompleted = false;
}

/* 受信完了状態取得 */
bool UdpPacketRxBuffer::IsCompleted()
{
    return this->m_IsCompleted;
}

/* 1番目のパケットを追加 */
void UdpPacketRxBuffer::AddFirstPacket(const UdpPacket& udp_packet)
{
    /* UDP Packet Headerを取得 */
    const UdpPacketHeader& header = udp_packet.Header;

    /* 現在のブロックインデックスを算出 */
    size_t current_block_index = header.CurrentBlockNum - 1;

    /* 受信完了状態をクリア */
    this->m_IsCompleted = false;

    /* メッセージIDをセット */
    this->m_MessageID = header.MessageID;

    /* 合計データサイズをセット */
    this->m_TotalSize = header.TotalSize;

    /* 合計ブロック数をセット */
    this->m_TotalBlockNum = header.TotalBlockNum;

    /* 受信ブロック数をセット */
    this->m_ReceivedBlockNum = 1;

    /* 受信ステータスを初期化 */
    this->m_RecvStatus.resize(this->m_TotalBlockNum);

    /* 受信ブロックの受信ステータスをセット */
    this->m_RecvStatus[current_block_index] = true;

    /* 受信バッファのメモリ確保 */
    this->m_Buffer = (byte_ptr)malloc(this->m_TotalSize);

    /* 受信バッファ確保成功 */
    if (this->m_Buffer != nullptr)
    {
        /* 開始インデックスセット */
        size_t start_index = 0;

        /* ブロックサイズ算出 */
        size_t block_size = header.PacketSize - UDP_PACKET_HEADER_SIZE;

        /* UDP Packet Dataを受信バッファにコピー */
        memcpy(&this->m_Buffer[start_index], udp_packet.Data, block_size);
    }
    else
    {
        throw std::bad_alloc();
    }
}

/* 2番目以降のパケットを追加 */
void UdpPacketRxBuffer::AddContinuousPacket(const UdpPacket& udp_packet)
{
    /* UDP Packet Headerを取得 */
    const UdpPacketHeader& header = udp_packet.Header;

    /* 現在のブロックインデックスを算出 */
    size_t current_block_index = header.CurrentBlockNum - 1;

    /* メッセージID確認 */
    if (this->m_MessageID == header.MessageID)
    {
        /* 合計データサイズ確認 */
        if (this->m_TotalSize == header.TotalSize)
        {
            /* 合計ブロック数確認 */
            if (this->m_TotalBlockNum == header.TotalBlockNum)
            {
                /* 受信ステータス確認 */
                if (this->m_RecvStatus.at(current_block_index) == false)
                {
                    /* 受信ブロック数インクリメント */
                    this->m_ReceivedBlockNum++;

                    /* 受信ブロックの受信ステータスをセット */
                    this->m_RecvStatus[current_block_index] = true;

                    /* 開始インデックスを算出 */
                    size_t start_index = (size_t)current_block_index * (size_t)UDP_PACKET_DATA_SIZE;

                    /* ブロックサイズ算出 */
                    size_t block_size = header.PacketSize - UDP_PACKET_HEADER_SIZE;

                    /* UDP Packet Dataを受信バッファにコピー */
                    memcpy(&this->m_Buffer[start_index], udp_packet.Data, block_size);
                }
                else
                {
                    throw std::logic_error("Current Block is already received");
                }
            }
            else
            {
                throw std::logic_error("Different Total Block Num");
            }
        }
        else
        {
            throw std::logic_error("Different Total Data Size");
        }
    }
    else
    {
        throw std::logic_error("Different Message ID");
    }
}

/* 受信完了確認 */
bool UdpPacketRxBuffer::CheckCompleted()
{
    /* 受信ブロック数確認 */
    if (this->m_ReceivedBlockNum == this->m_TotalBlockNum)
    {
        /* 受信ステータスを確認 */
        for (const auto& flag : this->m_RecvStatus)
        {
            /* 未受信ブロックがある場合は受信未完了 */
            if (flag == false)
            {
                return false;
            }
        }

        /* 受信完了 */
        return true;
    }
    else
    {
        /* 受信未完了 */
        return false;
    }
}
