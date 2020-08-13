#pragma once
#include "CompileSwitch.h"

#if COM_TYPE == COM_WINSOCK
#include "SocketAdapter.h"

#include <WinSock2.h>
#include <ws2tcpip.h>

#include <map>
#include <stdexcept>

#pragma comment(lib, "ws2_32.lib")


class SocketAdapterImpl final
{
public:
    static void Initialize()
    {
        SocketAdapterImpl::s_ErrorCode = 0;

        int wsa_result = WSAStartup(MAKEWORD(2, 0), &SocketAdapterImpl::s_WsaData);

        if (wsa_result != 0)
        {
            SocketAdapterImpl::s_ErrorCode = wsa_result;

            throw std::runtime_error("Socket Adapter Initialize Failed : ErrorCode = " + std::to_string(SocketAdapterImpl::s_ErrorCode));
        }
        else
        {
            /* Nothing to do */
        }
    }

    static void Finalize()
    {
        int wsa_result = WSACleanup();

        if (wsa_result == SOCKET_ERROR)
        {
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            throw std::runtime_error("Socket Adapter Finalize Failed : ErrorCode = " + std::to_string(SocketAdapterImpl::s_ErrorCode));
        }
        else
        {
            /* Nothing to do */
        }
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

        if (this->m_Socket == INVALID_SOCKET)
        {
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            return SocketResult::Fail;
        }

        this->m_Address.sin_family = AF_INET;
        this->m_Address.sin_port = htons(remote_port);
        int wsa_result = inet_pton(this->m_Address.sin_family, remote_ip.c_str(), &this->m_Address.sin_addr.S_un.S_addr);

        if (wsa_result == 1)
        {
            this->m_IsSocketOpened = true;

            return SocketResult::Success;
        }
        else if (wsa_result == 0)
        {
            SocketAdapterImpl::s_ErrorCode = -1;

            return SocketResult::Fail;
        }
        else
        {
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            return SocketResult::Fail;
        }
    }

    SocketResult OpenUdpRxSocket(const uint16_t local_port)
    {
        this->m_Socket = socket(AF_INET, SOCK_DGRAM, 0);

        if (this->m_Socket == INVALID_SOCKET)
        {
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            return SocketResult::Fail;
        }

        this->m_Address.sin_family = AF_INET;
        this->m_Address.sin_port = htons(local_port);
        this->m_Address.sin_addr.S_un.S_addr = INADDR_ANY;

        int wsa_result = bind(this->m_Socket, (struct sockaddr*)&this->m_Address, sizeof(this->m_Address));

        if (wsa_result == 0)
        {
            return SocketResult::Success;
        }
        else
        {
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            return SocketResult::Fail;
        }
    }

    SocketResult CloseUdpSocket()
    {
        if (this->m_IsSocketOpened == true)
        {
            int wsa_result = closesocket(this->m_Socket);

            if (wsa_result == SOCKET_ERROR)
            {
                SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

                return SocketResult::Fail;
            }
            else
            {
                return SocketResult::Success;
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
        int wsa_result = sendto(this->m_Socket, (const char*)data_ptr, (int)tx_size, 0, (struct sockaddr*)&this->m_Address, sizeof(this->m_Address));

        if (wsa_result == SOCKET_ERROR)
        {
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            return SocketResult::Fail;
        }
        else
        {
            return SocketResult::Success;
        }
    }

    SocketResult ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
    {
        int wsa_result = recv(this->m_Socket, (char*)buffer_ptr, (int)buffer_size, 0);

        if (wsa_result == SOCKET_ERROR)
        {
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            return SocketResult::Fail;
        }
        else
        {
            rx_size = (size_t)wsa_result;

            return SocketResult::Success;
        }
    }

private:
    static WSADATA s_WsaData;
    static int s_ErrorCode;

private:
    SOCKET m_Socket;
    struct sockaddr_in m_Address;
    bool m_IsSocketOpened;
};

WSADATA SocketAdapterImpl::s_WsaData;

int SocketAdapterImpl::s_ErrorCode = 0;


#else
#error Invalid Target Type : TARGET_TYPE
#endif
