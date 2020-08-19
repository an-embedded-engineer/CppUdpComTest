#pragma once
#include "UdpPacketTypes.h"
#include "SocketDataTypes.h"

#include <functional>
#include <vector>

/* UDP Packet Rx Bufferクラス宣言 */
class UdpPacketRxBuffer
{
public:
    /* コールバック */
    using CallbackType = std::function<void(const byte_ptr, const size_t)>;

public:
    /* コンストラクタ */
    UdpPacketRxBuffer();
    /* デストラクタ */
    ~UdpPacketRxBuffer();

    /* 受信コールバックの登録 */
    void RegisterCallback(CallbackType& callback);

    /* コールバック呼び出しのリクエスト */
    void RequestCallback();

    /* UDP Packetの追加 */
    void Add(const UdpPacket& udp_packet);

    /* 受信バッファ取得 */
    void GetBuffer(byte_ptr& data_ptr, size_t& data_size);

    /* 受信バッファの解放 */
    void Release();

    /* 受信完了状態取得 */
    bool IsCompleted();

private:
    /* 1番目のパケットを追加 */
    void AddFirstPacket(const UdpPacket& udp_packet);

    /* 2番目以降のパケットを追加 */
    void AddContinuousPacket(const UdpPacket& udp_packet);

    /* 受信完了確認 */
    bool CheckCompleted();

private:
    /* メッセージID */
    uint16_t m_MessageID;
    /* 受信バッファ */
    byte_ptr m_Buffer;
    /* 合計データサイズ */
    size_t m_TotalSize;
    /* 合計ブロック数 */
    size_t m_TotalBlockNum;
    /* 受信ブロック数 */
    size_t m_ReceivedBlockNum;
    /* 受信ステータス */
    std::vector<bool> m_RecvStatus;
    /* 受信完了状態 */
    bool m_IsCompleted;
    /* 受信コールバック */
    CallbackType m_Callback;
};

