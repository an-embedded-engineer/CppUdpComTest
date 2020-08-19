#include "UdpPacketRxBufferMap.h"
#include "AppException.h"

#include <iostream>

/* シングルトンインスタンス取得 */
UdpPacketRxBufferMap& UdpPacketRxBufferMap::GetInstance()
{
    static UdpPacketRxBufferMap instance;
    return instance;
}

/* コンストラクタ */
UdpPacketRxBufferMap::UdpPacketRxBufferMap()
    : m_Map()
    , m_Callback(nullptr)
{
    /* Nothing to do */
}

/* デストラクタ */
UdpPacketRxBufferMap::~UdpPacketRxBufferMap()
{
    /* Nothing to do */
}

/* 受信コールバックを登録 */
void UdpPacketRxBufferMap::RegisterCallback(CallbackType& callback)
{
    /* 受信コールバック未登録 */
    if (this->m_Callback == nullptr)
    {
        /* 受信コールバックを登録 */
        this->m_Callback = callback;
    }
    else
    {
        THROW_APP_EXCEPTION("Callback is already registered");
    }
}

/* メッセージIDごとの受信コールバックを登録 */
void UdpPacketRxBufferMap::RegisterCallback(uint16_t message_id, UdpPacketRxBuffer::CallbackType& callback)
{
    /* UDP Packet受信バッファ未登録 */
    if (this->m_Map.count(message_id) == 0)
    {
        /* UDP Packet受信バッファ登録 */
        this->m_Map.emplace(std::piecewise_construct, std::forward_as_tuple(message_id), std::forward_as_tuple());
    }

    /* 受信コールバック登録 */
    this->m_Map[message_id].RegisterCallback(callback);
}

/* UDP Packetの追加 */
void UdpPacketRxBufferMap::Add(const UdpPacket& udp_packet)
{
    /* メッセージID取得 */
    uint16_t message_id = udp_packet.Header.MessageID;

    /* UDP Packet受信バッファ未登録 */
    if (this->m_Map.count(message_id) == 0)
    {
        /* UDP Packet受信バッファ登録 */
        this->m_Map.emplace(std::piecewise_construct, std::forward_as_tuple(message_id), std::forward_as_tuple());
    }

    /* UDP Packet受信バッファ取得 */
    auto& item = this->m_Map[message_id];

    /* 受信バッファにUDP Packet追加 */
    item.Add(udp_packet);

    /* 受信完了確認 */
    if (item.IsCompleted())
    {
        /* 受信コールバック呼び出しをリクエスト */
        item.RequestCallback();

        /* 受信データバッファを初期化 */
        byte_ptr data_ptr = nullptr;

        /* 受信データサイズを初期化 */
        size_t data_size = 0;

        /* 受信データ取得 */
        item.GetBuffer(data_ptr, data_size);

        /* メッセージIDごとの受信コールバック呼び出しをリクエスト */
        this->RequestCallback(message_id, data_ptr, data_size);

        /* 受信バッファを解放 */
        item.Release();
    }
}

/* コールバック呼び出しのリクエスト */
void UdpPacketRxBufferMap::RequestCallback(uint16_t message_id, const byte_ptr data_ptr, const size_t data_size)
{
    /* コールバック登録確認 */
    if (this->m_Callback != nullptr)
    {
        /* 受信データ&サイズ確認 */
        if (data_ptr != nullptr && data_size > 0)
        {
            /* 受信コールバック呼び出しをリクエスト */
            this->m_Callback(message_id, data_ptr, data_size);
        }
        else
        {
            THROW_APP_EXCEPTION("Rx Buffer & Size are invalid");
        }
    }
    else
    {
        std::cerr << "[WARN] Callback is not registered" << std::endl;
    }
}
