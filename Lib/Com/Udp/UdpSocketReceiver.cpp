#include "UdpSocketReceiver.h"

/* コンストラクタ */
UdpSocketReceiver::UdpSocketReceiver()
    : m_Adapter(nullptr)
{
    /* Socket Adapterクラスインスタンス生成 */
    this->m_Adapter = std::make_unique<SocketAdapter>();
}

/* デストラクタ */
UdpSocketReceiver::~UdpSocketReceiver()
{
    /* Nothing to do */
}


/* ユニキャスト用ソケットオープン */
void UdpSocketReceiver::OpenUniSocket(const uint16_t local_port)
{
    /* UDPユニキャスト受信用ソケットオープン */
    this->m_Adapter->OpenUdpUniRxSocket(local_port);
}

/* マルチキャスト用ソケットオープン */
void UdpSocketReceiver::OpenMultiSocket(const std::string& multicast_ip, const uint16_t multicast_port)
{
    /* UDPマルチキャスト受信用ソケットオープン */
    this->m_Adapter->OpenUdpMultiRxSocket(multicast_ip, multicast_port);
}

/* ブロードキャスト用ソケットオープン */
void UdpSocketReceiver::OpenBroadSocket(const uint16_t local_port)
{
    /* UDPブロードキャスト受信用ソケットオープン */
    this->m_Adapter->OpenUdpBroadRxSocket(local_port);
}

/* ソケットクローズ */
void UdpSocketReceiver::CloseSocket()
{
    /* ソケットクローズ */
    this->m_Adapter->CloseSocket();
}

/* ソケットオープン確認 */
bool UdpSocketReceiver::IsSocketOpened()
{
    /* ソケットオープン確認 */
    return this->m_Adapter->IsSocketOpened();
}

/* パケット同期受信 */
void UdpSocketReceiver::ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
{
    /* パケット同期受信 */
    this->m_Adapter->ReceiveSync(buffer_ptr, buffer_size, rx_size);
}
