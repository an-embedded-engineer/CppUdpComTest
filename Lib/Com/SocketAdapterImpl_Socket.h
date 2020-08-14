#pragma once
#include "CompileSwitch.h"

#if COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
#include "SocketDataTypes.h"
#include "SocketException.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include <sstream>

/* Socket Adapter Implクラス定義 */
class SocketAdapterImpl final
{
public:
    /* Socket全体の初期化 */
    static void Initialize()
    {
        /* エラーコードクリア */
        SocketAdapterImpl::s_ErrorCode = 0;
    }

    /* Socket全体の後始末 */
    static void Finalize()
    {
        /* Nothing to do */
    }

    /* エラーコード取得 */
    static int GetErrorCode()
    {
        return SocketAdapterImpl::s_ErrorCode;
    }

    /* コンストラクタ */
    SocketAdapterImpl()
        : m_Socket(0)
        , m_Address()
        , m_IsSocketOpened(false)
    {
        /* Nothing to do */
    }

    /* デストラクタ */
    ~SocketAdapterImpl()
    {
        /* ソケットオープン確認 */
        if (this->IsSocketOpened() == true)
        {
            /* ソケットがオープンしていたらクローズする */
            this->CloseSocket();
        }
    }

    /* UDPユニキャスト送信用ソケットオープン */
    void OpenUdpUniTxSocket(const std::string& remote_ip, const uint16_t remote_port)
    {
        /* UDP用ソケットをオープン */
        this->m_Socket = socket(AF_INET, SOCK_DGRAM, 0);

        /* ソケットオープン失敗時のエラー処理 */
        if (this->m_Socket < 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = errno;

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Unicast Tx Socket Open Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* UDPユニキャスト送信用アドレス情報セット */
        this->m_Address.sin_family = AF_INET;
        this->m_Address.sin_port = htons(remote_port);
        this->m_Address.sin_addr.s_addr = inet_addr(remote_ip.c_str());
#if COM_TYPE == COM_MACSOCK
        this->m_Address.sin_len = sizeof(this->m_Address);
#endif

        /* ソケットオープン状態更新 */
        this->m_IsSocketOpened = true;
    }

    /* UDPユニキャスト受信用ソケットオープン */
    void OpenUdpUniRxSocket(const uint16_t local_port)
    {
        /* UDP用ソケットをオープン */
        this->m_Socket = socket(AF_INET, SOCK_DGRAM, 0);

        /* ソケットオープン失敗時のエラー処理 */
        if (this->m_Socket < 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = errno;

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Unicast Rx Socket Open Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* UDPユニキャスト受信用アドレス情報セット */
        this->m_Address.sin_family = AF_INET;
        this->m_Address.sin_port = htons(local_port);
        this->m_Address.sin_addr.s_addr = INADDR_ANY;
#if COM_TYPE == COM_MACSOCK
        this->m_Address.sin_len = sizeof(this->m_Address);
#endif

        /* ソケットにアドレス情報をバインド */
        int bind_result = bind(this->m_Socket, (struct sockaddr*)&this->m_Address, sizeof(this->m_Address));

        /* ソケットバインド失敗時のエラー処理 */
        if (bind_result != 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = errno;

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Unicast Rx Socket Bind Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* ソケットオープン状態更新 */
        this->m_IsSocketOpened = true;
    }

    /* UDPマルチキャスト送信用ソケットオープン */
    void OpenUdpMultiTxSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port)
    {
        /* UDP用ソケットをオープン */
        this->m_Socket = socket(AF_INET, SOCK_DGRAM, 0);

        /* ソケットオープン失敗時のエラー処理 */
        if (this->m_Socket < 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = errno;

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Multicast Tx Socket Open Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* UDPユニキャスト送信用アドレス情報セット */
        this->m_Address.sin_family = AF_INET;
        this->m_Address.sin_port = htons(multicast_port);
        this->m_Address.sin_addr.s_addr = inet_addr(multicast_ip.c_str());
#if COM_TYPE == COM_MACSOCK
        this->m_Address.sin_len = sizeof(this->m_Address);
#endif

        /* ローカルIPアドレスセット */
        this->m_LocalIpAddress = inet_addr(local_ip.c_str());

        /* UDPマルチキャストソケットオプションセット */
        int set_opt_result = setsockopt(this->m_Socket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&this->m_LocalIpAddress, sizeof(this->m_LocalIpAddress));

        /* ソケットオプションセット失敗時のエラー処理 */
        if (set_opt_result != 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = errno;

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Multicast Tx Socket Option Set Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* ソケットオープン状態更新 */
        this->m_IsSocketOpened = true;
    }

    /* UDPマルチキャスト受信用ソケットオープン */
    void OpenUdpMultiRxSocket(const std::string& multicast_ip, const uint16_t multicast_port)
    {
        /* UDP用ソケットをオープン */
        this->m_Socket = socket(AF_INET, SOCK_DGRAM, 0);

        /* ソケットオープン失敗時のエラー処理 */
        if (this->m_Socket < 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = errno;

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Multicast Rx Socket Open Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* UDPユニキャスト受信用アドレス情報セット */
        this->m_Address.sin_family = AF_INET;
        this->m_Address.sin_port = htons(multicast_port);
        this->m_Address.sin_addr.s_addr = INADDR_ANY;
#if COM_TYPE == COM_MACSOCK
        this->m_Address.sin_len = sizeof(this->m_Address);
#endif

        /* ソケットにアドレス情報をバインド */
        int bind_result = bind(this->m_Socket, (struct sockaddr*)&this->m_Address, sizeof(this->m_Address));

        /* ソケットバインド失敗時のエラー処理 */
        if (bind_result != 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = errno;

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Multicast Rx Socket Bind Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* マルチキャストリクエストのセット */
        memset(&this->m_MulticastRequest, 0, sizeof(this->m_MulticastRequest));
        this->m_MulticastRequest.imr_interface.s_addr = INADDR_ANY;
        this->m_MulticastRequest.imr_multiaddr.s_addr = inet_addr(multicast_ip.c_str());

        /* UDPマルチキャストソケットオプションセット */
        int set_opt_result = setsockopt(this->m_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&this->m_MulticastRequest, sizeof(this->m_MulticastRequest));

        /* ソケットオプションセット失敗時のエラー処理 */
        if (set_opt_result != 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = errno;

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Multicast Rx Socket Option Set Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* ソケットオープン状態更新 */
        this->m_IsSocketOpened = true;
    }

    /* ソケットクローズ */
    void CloseSocket()
    {
        /* ソケットオープン確認 */
        if (this->IsSocketOpened() == true)
        {
            /* ソケットがオープンしていたらクローズ */
            int close_result = close(this->m_Socket);

            /* ソケットクローズ成功時 */
            if (close_result == 0)
            {
                /* ソケットオープン状態をクリア */
                this->m_IsSocketOpened = false;
            }
            /* ソケットクローズ失敗時のエラー処理 */
            else
            {
                /* エラーコードセット */
                SocketAdapterImpl::s_ErrorCode = errno;

                /* ソケット例外送出 */
                throw SocketException(SocketAdapterImpl::GetErrorMessage("Socket Close Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
            }
        }
        else
        {
            /* Nothing to do */
        }
    }

    /* ソケットオープン確認 */
    bool IsSocketOpened()
    {
        return this->m_IsSocketOpened;
    }

    /* パケット送信 */
    void Transmit(const any_ptr data_ptr, size_t tx_size)
    {
        /* ソケットにパケット送信 */
        int send_result = sendto(this->m_Socket, (const char*)data_ptr, (int)tx_size, 0, (struct sockaddr*)&this->m_Address, sizeof(this->m_Address));

        /* パケット送信失敗時のエラー処理 */
        if (send_result < 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = errno;

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("Packet Transmit Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }
    }

    /* パケット同期受信 */
    void ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
    {
        /* ソケットからパケット受信(ブロッキング) */
        int receive_result = recv(this->m_Socket, (char*)buffer_ptr, (int)buffer_size, 0);

        /* パケット受信成功時 */
        if (receive_result >= 0)
        {
            /* 受信データサイズをセット */
            rx_size = (size_t)receive_result;
        }
        /* パケット受信失敗時のエラー処理 */
        else
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = errno;

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("Packet Receive Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }
    }

private:
    /* エラーメッセージ生成 */
    static inline std::string GetErrorMessage(const char* message, int error_code)
    {
        std::stringstream ss;
        ss << "[Socket Error] " << message << " : ErrorCode = " << error_code;
        return ss.str();
    }

private:
    /* エラーコード */
    static int s_ErrorCode;

private:
    /* ソケット */
    int m_Socket;
    /* ソケットアドレス情報 */
    struct sockaddr_in m_Address;
    /* ローカルIPアドレス */
    in_addr_t m_LocalIpAddress;
    /* マルチキャストリクエスト */
    ip_mreq m_MulticastRequest;
    /* ソケットオープン状態 */
    bool m_IsSocketOpened;
};

/* エラーコード */
int SocketAdapterImpl::s_ErrorCode = 0;

#else

#endif
