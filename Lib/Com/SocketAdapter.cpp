#include "SocketAdapter.h"
#include "CompileSwitch.h"

#if COM_TYPE == COM_WINSOCK
#include "SocketAdapterImpl_WinSock.h"
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
#include "SocketAdapterImpl_Socket.h"
#else
#include "SocketAdapterImpl_Unknown.h"
#endif

void SocketAdapter::Initialize()
{
    SocketAdapterImpl::Initialize();
}

void SocketAdapter::Finalize()
{
    SocketAdapterImpl::Finalize();
}

int SocketAdapter::GetErrorCode()
{
    return SocketAdapterImpl::GetErrorCode();
}

SocketAdapter::SocketAdapter()
    : m_Impl(nullptr)
{
    this->m_Impl = std::make_unique<SocketAdapterImpl>();
}

SocketAdapter::~SocketAdapter()
{
    /* Nothing to do */
}

void SocketAdapter::OpenUdpUniTxSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    this->m_Impl->OpenUdpUniTxSocket(remote_ip, remote_port);
}

void SocketAdapter::OpenUdpUniRxSocket(const uint16_t local_port)
{
    this->m_Impl->OpenUdpUniRxSocket(local_port);
}

void SocketAdapter::CloseSocket()
{
    this->m_Impl->CloseSocket();
}

bool SocketAdapter::IsSocketOpened()
{
    return this->m_Impl->IsSocketOpened();
}

void SocketAdapter::Transmit(const any_ptr data_ptr, size_t tx_size)
{
    this->m_Impl->Transmit(data_ptr, tx_size);
}

void SocketAdapter::ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
{
    this->m_Impl->ReceiveSync(buffer_ptr, buffer_size, rx_size);
}
