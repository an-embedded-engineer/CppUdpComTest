#pragma once
#include "UdpSocketReceiver.h"
#include "UdpPacketReceiver.h"

/* UDP Receiverクラス宣言 */
class UdpReceiver
{
public:
    /* コンストラクタ */
    UdpReceiver();
    /* デストラクタ */
    ~UdpReceiver();

    /* ユニキャスト用ソケットオープン */
    void OpenUniSocket(const uint16_t local_port);
    /* マルチキャスト用ソケットオープン */
    void OpenMultiSocket(const std::string& multicast_ip, const uint16_t multicast_port);
    /* ブロードキャスト用ソケットオープン */
    void OpenBroadSocket(const uint16_t local_port);
    /* ソケットクローズ */
    void CloseSocket();
    /* ソケットオープン確認 */
    bool IsSocketOpened();

    /* データ同期受信 */
    void ReceiveSync(uint16_t& message_id, std::string& message);

private:
    /* データ受信コールバック */
    void ReceiveCallback(const uint16_t message_id, const byte_ptr data_ptr, const size_t data_size);

private:
    /* UDP Socket Receiverクラスインスタンス */
    std::unique_ptr<UdpSocketReceiver> m_SocketReceiver;

    /* UDP Packet Receiverクラスインスタンス */
    UdpPacketReceiver& m_PacketReceiver;

    /* 受信メッセージID */
    uint16_t m_MessageID;

    /* 受信メッセージ */
    std::string m_Message;

    /* 受信完了状態 */
    bool m_IsReceived;
};

