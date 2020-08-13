#pragma once
#include "CompileSwitch.h"

#if COM_TYPE == COM_SOCKET
#include "SocketAdapter.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include <map>
#include <stdexcept>


class SocketAdapterImpl final
{
public:
    static void Initialize()
    {
        SocketAdapterImpl::s_ErrorCode = 0;
    }

    static void Finalize()
    {
        /* Nothing to do */
    }

    static int GetErrorCode()
    {
        return SocketAdapterImpl::s_ErrorCode;
    }

    SocketAdapterImpl()
        : m_Socket(0)
        , m_Address()
        , m_IsSocketOpened(false)
    {

    }

    ~SocketAdapterImpl()
    {
        if (this->IsSocketOpened() == true)
        {
            this->CloseUdpSocket();
        }
    }

    SocketResult OpenUdpTxSocket(const std::string& remote_ip, const uint16_t remote_port)
    {
        this->m_Socket = socket(AF_INET, SOCK_DGRAM, 0);

        if (this->m_Socket < 0)
        {
            SocketAdapterImpl::s_ErrorCode = errno;

            return SocketResult::Fail;
        }

        this->m_Address.sin_family = AF_INET;
        this->m_Address.sin_port = htons(remote_port);
        this->m_Address.sin_addr.s_addr = inet_addr(remote_ip.c_str());

        this->m_IsSocketOpened = true;

        return SocketResult::Success;
    }

    SocketResult OpenUdpRxSocket(const uint16_t local_port)
    {
        this->m_Socket = socket(AF_INET, SOCK_DGRAM, 0);

        if (this->m_Socket < 0)
        {
            SocketAdapterImpl::s_ErrorCode = errno;

            return SocketResult::Fail;
        }

        this->m_Address.sin_family = AF_INET;
        this->m_Address.sin_port = htons(local_port);
        this->m_Address.sin_addr.s_addr = INADDR_ANY;
        this->m_Address.sin_len = sizeof(this->m_Address);

        int sock_result = bind(this->m_Socket, (struct sockaddr*)&this->m_Address, sizeof(this->m_Address));

        if (sock_result == 0)
        {
            return SocketResult::Success;
        }
        else
        {
            SocketAdapterImpl::s_ErrorCode = errno;

            return SocketResult::Fail;
        }
    }

    SocketResult CloseUdpSocket()
    {
        if (this->m_IsSocketOpened == true)
        {
            int sock_result = close(this->m_Socket);

            if (sock_result == 0)
            {
                return SocketResult::Success;
            }
            else
            {
                SocketAdapterImpl::s_ErrorCode = errno;

                return SocketResult::Fail;
            }
        }
        else
        {

            return SocketResult::Success;
        }
    }

    bool IsSocketOpened()
    {
        return this->m_IsSocketOpened;
    }

    SocketResult Transmit(const any_ptr data_ptr, size_t tx_size)
    {
        int sock_result = sendto(this->m_Socket, (const char*)data_ptr, (int)tx_size, 0, (struct sockaddr*)&this->m_Address, sizeof(this->m_Address));

        if (sock_result >= 0)
        {
            return SocketResult::Success;
        }
        else
        {
            SocketAdapterImpl::s_ErrorCode = errno;

            return SocketResult::Fail;
        }
    }

    SocketResult ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
    {
        int sock_result = recv(this->m_Socket, (char*)buffer_ptr, (int)buffer_size, 0);

        if (sock_result >= 0)
        {
            rx_size = (size_t)sock_result;

            return SocketResult::Success;
        }
        else
        {
            SocketAdapterImpl::s_ErrorCode = errno;

            return SocketResult::Fail;
        }
    }

private:
    static int s_ErrorCode;

private:
    int m_Socket;
    struct sockaddr_in m_Address;
    bool m_IsSocketOpened;
};

int SocketAdapterImpl::s_ErrorCode = 0;

#else
#error Invalid Target Type : TARGET_TYPE
#endif
