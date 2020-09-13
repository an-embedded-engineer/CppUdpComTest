#pragma once
#include "UdpSocketReceiver.h"
#include "UdpPacketReceiver.h"
#include "UdpMessageQueueMap.h"

/* UDP Receiverクラス宣言 */
class UdpReceiver
{
public:
    /* コンストラクタ */
    UdpReceiver();
    /* デストラクタ */
    ~UdpReceiver();

    /* ユニキャスト用ソケットオープン */
    void OpenUniSocket(const uint16_t local_port, const ReceiveType receive_type);
    /* マルチキャスト用ソケットオープン */
    void OpenMultiSocket(const std::string& multicast_ip, const uint16_t multicast_port, const ReceiveType receive_type);
    /* ブロードキャスト用ソケットオープン */
    void OpenBroadSocket(const uint16_t local_port, const ReceiveType receive_type);
    /* ソケットクローズ */
    void CloseSocket();
    /* ソケットオープン確認 */
    bool IsSocketOpened();

    /* データ同期受信 */
    void ReceiveSync(uint16_t& message_id, std::string& message);

    /* データ非同期受信開始 */
    void BeginReceiveAsync();

    /* データ非同期受信停止 */
    void EndReceiveAsync();


private:
    /* データ同期受信コールバック */
    void SyncReceiveCallback(const uint16_t message_id, const byte_ptr data_ptr, const size_t data_size);

    /* データ非同期受信コールバック */
    void AsyncReceiveCallback(const uint16_t message_id, const byte_ptr data_ptr, const size_t data_size);


private:
    /* UDP Socket Receiverクラスインスタンス */
    std::unique_ptr<UdpSocketReceiver> m_SocketReceiver;

    /* UDP Packet Receiverクラスインスタンス */
    UdpPacketReceiver& m_PacketReceiver;

    /* UDP Message Queue Map */
    UdpMessageQueueMap& m_MessageQueueMap;

    /* 受信メッセージID */
    uint16_t m_MessageID;

    /* 受信メッセージ */
    std::string m_Message;

    /* 受信完了状態 */
    bool m_IsReceived;
};

