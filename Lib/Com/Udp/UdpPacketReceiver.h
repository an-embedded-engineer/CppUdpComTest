#pragma once
#include "SocketDataTypes.h"
#include "UdpPacketTypes.h"
#include "UdpPacketRxBufferMap.h"

class UdpSocketReceiver;
class UdpPacketEncoder;

/* UDP Packet Senderクラス宣言 */
class UdpPacketReceiver final
{
public:
    /* シングルトンインスタンス取得 */
    static UdpPacketReceiver& GetInstance();

private:
    /* コンストラクタ */
    UdpPacketReceiver();
    /* デストラクタ */
    ~UdpPacketReceiver();

public:
    /* コピーコンストラクタ削除 */
    UdpPacketReceiver(const UdpPacketReceiver&) = delete;
    /* ムーブコンストラクタ削除 */
    UdpPacketReceiver(UdpPacketReceiver&&) = delete;
    /* コピー代入演算子削除 */
    UdpPacketReceiver& operator=(const UdpPacketReceiver&) = delete;
    /* ムーブ代入演算子削除 */
    UdpPacketReceiver& operator=(UdpPacketReceiver&&) = delete;

public:
    /* データ同期受信 */
    void ReceiveSync(UdpSocketReceiver& udp_socket_receiver);

    /* 受信コールバックを登録 */
    void RegisterCallback(UdpPacketRxBufferMap::CallbackType& callback);

    /* メッセージIDごとの受信コールバックを登録 */
    void RegisterCallback(uint16_t message_id, UdpPacketRxBuffer::CallbackType& callback);

private:
    /* UDP Packet Encoder */
    UdpPacketEncoder& m_Encoder;

    /* UDP Packet Rx Buffer Map */
    UdpPacketRxBufferMap& m_RxBufferMap;
};
