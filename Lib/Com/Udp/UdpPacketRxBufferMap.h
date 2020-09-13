#pragma once
#include "UdpPacketRxBuffer.h"

#include <map>
#include <mutex>

/* UDP Packet Rx Buffer Mapクラス宣言 */
class UdpPacketRxBufferMap
{
public:
    /* 受信コールバック */
    using CallbackType = std::function<void(const uint16_t, const byte_ptr, const size_t)>;

public:
    /* シングルトンインスタンス取得 */
    static UdpPacketRxBufferMap& GetInstance();

private:
    /* コンストラクタ */
    UdpPacketRxBufferMap();
    /* デストラクタ */
    ~UdpPacketRxBufferMap();

public:
    /* コピーコンストラクタ削除 */
    UdpPacketRxBufferMap(const UdpPacketRxBufferMap&) = delete;
    /* ムーブコンストラクタ削除 */
    UdpPacketRxBufferMap(UdpPacketRxBufferMap&&) = delete;
    /* コピー代入演算子削除 */
    UdpPacketRxBufferMap& operator=(const UdpPacketRxBufferMap&) = delete;
    /* ムーブ代入演算子削除 */
    UdpPacketRxBufferMap& operator=(UdpPacketRxBufferMap&&) = delete;

public:
    /* 受信コールバックを登録 */
    void RegisterCallback(CallbackType& callback);

    /* UDP Packetの追加 */
    void Add(const UdpPacket& udp_packet);

private:
    /* コールバック呼び出しのリクエスト */
    void RequestCallback(uint16_t message_id, const byte_ptr data_ptr, const size_t data_size);

private:
    /* ミューテックス */
    std::mutex m_Mutex;

    /* UDP Packet受信バッファマップ */
    std::map<uint16_t, UdpPacketRxBuffer> m_Map;

    /* 受信コールバック */
    CallbackType m_Callback;
};
