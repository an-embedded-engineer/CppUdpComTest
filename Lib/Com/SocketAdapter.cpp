#include "SocketAdapter.h"
#include "CompileSwitch.h"

#if COM_TYPE == COM_WINSOCK
#include "SocketAdapterImpl_WinSock.h"
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
#include "SocketAdapterImpl_Socket.h"
#else
#include "SocketAdapterImpl_Unknown.h"
#endif

/* Socket全体の初期化 */
void SocketAdapter::Initialize()
{
    SocketAdapterImpl::Initialize();
}

/* Socket全体の後始末 */
void SocketAdapter::Finalize()
{
    SocketAdapterImpl::Finalize();
}

/* エラーコード取得 */
int SocketAdapter::GetErrorCode()
{
    return SocketAdapterImpl::GetErrorCode();
}

/* コンストラクタ */
SocketAdapter::SocketAdapter()
    : m_Impl(nullptr)
{
    /* Socket Adapter Implクラスインスタンス生成 */
    this->m_Impl = std::make_unique<SocketAdapterImpl>();
}

/* デストラクタ */
SocketAdapter::~SocketAdapter()
{
    /* Nothing to do */
}

/* UDPユニキャスト送信用ソケットオープン */
void SocketAdapter::OpenUdpUniTxSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    this->m_Impl->OpenUdpUniTxSocket(remote_ip, remote_port);
}

/* UDPユニキャスト受信用ソケットオープン */
void SocketAdapter::OpenUdpUniRxSocket(const uint16_t local_port)
{
    this->m_Impl->OpenUdpUniRxSocket(local_port);
}

/* UDPマルチキャスト送信用ソケットオープン */
void SocketAdapter::OpenUdpMultiTxSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port)
{
    this->m_Impl->OpenUdpMultiTxSocket(multicast_ip, local_ip, multicast_port);
}

/* UDPマルチキャスト受信用ソケットオープン */
void SocketAdapter::OpenUdpMultiRxSocket(const std::string& multicast_ip, const uint16_t multicast_port)
{
    this->m_Impl->OpenUdpMultiRxSocket(multicast_ip, multicast_port);
}

/* ソケットクローズ */
void SocketAdapter::CloseSocket()
{
    this->m_Impl->CloseSocket();
}

/* ソケットオープン確認 */
bool SocketAdapter::IsSocketOpened()
{
    return this->m_Impl->IsSocketOpened();
}

/* パケット送信 */
void SocketAdapter::Transmit(const any_ptr data_ptr, size_t tx_size)
{
    this->m_Impl->Transmit(data_ptr, tx_size);
}

/* パケット同期受信 */
void SocketAdapter::ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
{
    this->m_Impl->ReceiveSync(buffer_ptr, buffer_size, rx_size);
}
