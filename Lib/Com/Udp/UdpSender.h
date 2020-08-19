#pragma once
#include "UdpSocketSender.h"
#include "UdpPacketSender.h"

class UdpSender
{
public:
    /* コンストラクタ */
    UdpSender();
    /* デストラクタ */
    ~UdpSender();

    /* ユニキャスト用ソケットオープン */
    void OpenUniSocket(const std::string& remote_ip, const uint16_t remote_port);
    /* マルチキャスト用ソケットオープン */
    void OpenMultiSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl);
    /* ブロードキャスト用ソケットオープン */
    void OpenBroadSocket(const std::string& remote_ip, const uint16_t remote_port);
    /* ソケットクローズ */
    void CloseSocket();
    /* ソケットオープン確認 */
    bool IsSocketOpened();

    /* データ送信 */
    void Transmit(const uint16_t message_id, const byte_ptr data_ptr, const size_t tx_size);

private:
    /* UDP Socket Senderクラスインスタンス */
    std::unique_ptr<UdpSocketSender> m_SocketSender;

    /* UDP Packet Senderクラスインスタンス */
    UdpPacketSender& m_PacketSender;
};

