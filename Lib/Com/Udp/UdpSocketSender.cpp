#include "UdpSocketSender.h"

/* コンストラクタ */
UdpSocketSender::UdpSocketSender()
    : m_Adapter(nullptr)
{
    /* Socket Adapterクラスインスタンス生成 */
    this->m_Adapter = std::make_unique<SocketAdapter>();
}

/* デストラクタ */
UdpSocketSender::~UdpSocketSender()
{
    /* Nothing to do */
}

/* ユニキャスト用ソケットオープン */
void UdpSocketSender::OpenUniSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    /* UDPユニキャスト送信用ソケットオープン */
    this->m_Adapter->OpenUdpUniTxSocket(remote_ip, remote_port);
}

/* マルチキャスト用ソケットオープン */
void UdpSocketSender::OpenMultiSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl)
{
    /* UDPマルチキャスト送信用ソケットオープン */
    this->m_Adapter->OpenUdpMultiTxSocket(multicast_ip, local_ip, multicast_port, ttl);
}

/* ブロードキャスト用ソケットオープン */
void UdpSocketSender::OpenBroadSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    /* UDPブロードキャスト送信用ソケットオープン */
    this->m_Adapter->OpenUdpBroadTxSocket(remote_ip, remote_port);
}

/* ソケットクローズ */
void UdpSocketSender::CloseSocket()
{
    /* ソケットクローズ */
    this->m_Adapter->CloseSocket();
}

/* ソケットオープン確認 */
bool UdpSocketSender::IsSocketOpened()
{
    /* ソケットオープン確認 */
    return this->m_Adapter->IsSocketOpened();
}

/* パケット送信 */
void UdpSocketSender::Transmit(const any_ptr data_ptr, const size_t tx_size)
{
    this->m_Adapter->Transmit(data_ptr, tx_size);
}

