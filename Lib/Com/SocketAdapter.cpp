#include "SocketAdapter.h"
#include "CompileSwitch.h"

#if COM_TYPE == COM_WINSOCK
#include "SocketAdapterImpl_WinSock.h"

#elif COM_TYPE == COM_MACSOCK
#include "SocketAdapterImpl_MacSock.h"

#elif COM_TYPE == COM_SOCKET
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

SocketResult SocketAdapter::OpenUdpTxSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    return this->m_Impl->OpenUdpTxSocket(remote_ip, remote_port);
}

SocketResult SocketAdapter::OpenUdpRxSocket(const uint16_t local_port)
{
    return this->m_Impl->OpenUdpRxSocket(local_port);
}

SocketResult SocketAdapter::CloseUdpSocket()
{
    return this->m_Impl->CloseUdpSocket();
}

bool SocketAdapter::IsSocketOpened()
{
    return this->m_Impl->IsSocketOpened();
}

SocketResult SocketAdapter::Transmit(const any_ptr data_ptr, size_t tx_size)
{
    return this->m_Impl->Transmit(data_ptr, tx_size);
}

SocketResult SocketAdapter::ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
{
    return this->m_Impl->ReceiveSync(buffer_ptr, buffer_size, rx_size);
}

