#include "UdpSender.h"

/* コンストラクタ */
UdpSender::UdpSender()
    : m_SocketSender(nullptr)
    , m_PacketSender(UdpPacketSender::GetInstance())
{
    /* UDP Socket Senderクラスインスタンス生成 */
    this->m_SocketSender = std::make_unique<UdpSocketSender>();
}

/* デストラクタ */
UdpSender::~UdpSender()
{
    /* Nothing to do */
}

/* ユニキャスト用ソケットオープン */
void UdpSender::OpenUniSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    /* ユニキャスト用ソケットオープン */
    this->m_SocketSender->OpenUniSocket(remote_ip, remote_port);
}

/* マルチキャスト用ソケットオープン */
void UdpSender::OpenMultiSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl)
{
    /* マルチキャスト用ソケットオープン */
    this->m_SocketSender->OpenMultiSocket(multicast_ip, local_ip, multicast_port, ttl);
}

/* ブロードキャスト用ソケットオープン */
void UdpSender::OpenBroadSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    /* ブロードキャスト用ソケットオープン */
    this->m_SocketSender->OpenBroadSocket(remote_ip, remote_port);
}

/* ソケットクローズ */
void UdpSender::CloseSocket()
{
    /* ソケットクローズ */
    this->m_SocketSender->CloseSocket();
}

/* ソケットオープン確認 */
bool UdpSender::IsSocketOpened()
{
    /* ソケットオープン確認 */
    return this->m_SocketSender->IsSocketOpened();
}

/* データ送信 */
void UdpSender::Transmit(const uint16_t message_id, const byte_ptr data_ptr, const size_t tx_size)
{
    /* データをUDPパケットに分割して送信 */
    this->m_PacketSender.Transmit(*this->m_SocketSender, message_id, data_ptr, tx_size);
}
