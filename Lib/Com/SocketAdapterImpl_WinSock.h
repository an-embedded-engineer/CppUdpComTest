#pragma once
#include "CompileSwitch.h"

#if COM_TYPE == COM_WINSOCK
#include "SocketDataTypes.h"
#include "SocketException.h"

#include <WinSock2.h>
#include <ws2tcpip.h>

#include <sstream>

#pragma comment(lib, "ws2_32.lib")

/* Socket Adapter Implクラス定義 */
class SocketAdapterImpl final
{
public:
    /* Socket全体の初期化 */
    static void Initialize()
    {
        /* エラーコードクリア */
        SocketAdapterImpl::s_ErrorCode = 0;

        /* WinSockサービススタートアップ */
        int startup_result = WSAStartup(MAKEWORD(2, 0), &SocketAdapterImpl::s_WsaData);

        /* WinSockサービスススタートアップ失敗時のエラー処理 */
        if (startup_result != 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = startup_result;

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("WSA Startup Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }
    }

    /* Socket全体の後始末 */
    static void Finalize()
    {
        /* WinSockサービスクリーンアップ */
        int cleanup_result = WSACleanup();

        /* WinSockサービスクリーンアップ失敗時のエラー処理 */
        if (cleanup_result == SOCKET_ERROR)
        {
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("WSA Cleanup Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }
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
        if (this->m_Socket == INVALID_SOCKET)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Unicast Tx Socket Open Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* UDPユニキャスト送信用アドレス情報セット */
        this->m_Address.sin_family = AF_INET;
        this->m_Address.sin_port = htons(remote_port);
        this->m_Address.sin_addr.S_un.S_addr = this->ConvertIpStrToNum(this->m_Address.sin_family, remote_ip);

        /* ソケットオープン状態更新 */
        this->m_IsSocketOpened = true;
    }

    /* UDPユニキャスト受信用ソケットオープン */
    void OpenUdpUniRxSocket(const uint16_t local_port)
    {
        /* UDP用ソケットをオープン */
        this->m_Socket = socket(AF_INET, SOCK_DGRAM, 0);

        /* ソケットオープン失敗時のエラー処理 */
        if (this->m_Socket == INVALID_SOCKET)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Unicast Rx Socket Open Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* UDPユニキャスト受信用アドレス情報セット */
        this->m_Address.sin_family = AF_INET;
        this->m_Address.sin_port = htons(local_port);
        this->m_Address.sin_addr.S_un.S_addr = INADDR_ANY;

        /* ソケットにアドレス情報をバインド */
        int bind_result = bind(this->m_Socket, (struct sockaddr*)&this->m_Address, sizeof(this->m_Address));

        /* ソケットバインド失敗時のエラー処理 */
        if (bind_result != 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

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
        if (this->m_Socket == INVALID_SOCKET)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Multicast Tx Socket Open Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* UDPユニキャスト送信用アドレス情報セット */
        this->m_Address.sin_family = AF_INET;
        this->m_Address.sin_port = htons(multicast_port);
        this->m_Address.sin_addr.S_un.S_addr = this->ConvertIpStrToNum(this->m_Address.sin_family, multicast_ip);

        /* ローカルIPアドレスセット */
        this->m_LocalIpAddress = this->ConvertIpStrToNum(this->m_Address.sin_family, local_ip);

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
        if (this->m_Socket == INVALID_SOCKET)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Unicast Rx Socket Open Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* UDPユニキャスト受信用アドレス情報セット */
        this->m_Address.sin_family = AF_INET;
        this->m_Address.sin_port = htons(multicast_port);
        this->m_Address.sin_addr.S_un.S_addr = INADDR_ANY;

        /* ソケットにアドレス情報をバインド */
        int bind_result = bind(this->m_Socket, (struct sockaddr*)&this->m_Address, sizeof(this->m_Address));

        /* ソケットバインド失敗時のエラー処理 */
        if (bind_result != 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("UDP Unicast Rx Socket Bind Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }

        /* マルチキャストリクエストのセット */
        memset(&this->m_MulticastRequest, 0, sizeof(this->m_MulticastRequest));
        this->m_MulticastRequest.imr_interface.S_un.S_addr = INADDR_ANY;
        this->m_MulticastRequest.imr_multiaddr.S_un.S_addr = this->ConvertIpStrToNum(this->m_Address.sin_family, multicast_ip);

        /* UDPマルチキャストソケットオプションセット */
        int set_opt_result = setsockopt(this->m_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&this->m_MulticastRequest, sizeof(this->m_MulticastRequest));

        /* ソケットオプションセット失敗時のエラー処理 */
        if (set_opt_result != 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

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
            int close_result = closesocket(this->m_Socket);

            /* ソケットクローズ失敗時のエラー処理 */
            if (close_result == SOCKET_ERROR)
            {
                /* エラーコードセット */
                SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

                /* ソケット例外送出 */
                throw SocketException(SocketAdapterImpl::GetErrorMessage("Socket Close Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
            }
            /* ソケットクローズ成功時 */
            else
            {
                /* ソケットオープン状態をクリア */
                this->m_IsSocketOpened = false;
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
        if (send_result == SOCKET_ERROR)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("Packet Transmit Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }
    }

    /* パケット同期受信 */
    void ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
    {
        /* ソケットからパケット受信(ブロッキング) */
        int receive_result = recv(this->m_Socket, (char*)buffer_ptr, (int)buffer_size, 0);

        /* パケット受信失敗時のエラー処理 */
        if (receive_result == SOCKET_ERROR)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("Packet Receive Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }
        /* パケット受信成功時 */
        else
        {
            /* 受信データサイズをセット */
            rx_size = (size_t)receive_result;
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

    static ULONG ConvertIpStrToNum(const ADDRESS_FAMILY family, const std::string& ip)
    {
        /* 変換後IPアドレス */
        ULONG ip_num = 0;

        /* IPアドレスを文字列から数値に変換 */
        int pton_result = inet_pton(family, ip.c_str(), &ip_num);

        /* IPアドレス変換成功時 */
        if (pton_result == 1)
        {
            return ip_num;
        }
        /* IPアドレス変換失敗時のエラー処理 */
        else if (pton_result == 0)
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = -1;

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("Convert IP Address Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }
        else
        {
            /* エラーコードセット */
            SocketAdapterImpl::s_ErrorCode = WSAGetLastError();

            /* ソケット例外送出 */
            throw SocketException(SocketAdapterImpl::GetErrorMessage("Convert IP Address Failed", SocketAdapterImpl::s_ErrorCode), SocketAdapterImpl::s_ErrorCode);
        }
    }

private:
    /* WinSockサービス情報 */
    static WSADATA s_WsaData;
    /* エラーコード */
    static int s_ErrorCode;

private:
    /* ソケット */
    SOCKET m_Socket;
    /* ソケットアドレス情報 */
    struct sockaddr_in m_Address;
    /* ローカルIPアドレス */
    ULONG m_LocalIpAddress;
    /* マルチキャストリクエスト */
    ip_mreq m_MulticastRequest;
    /* ソケットオープン状態 */
    bool m_IsSocketOpened;
};

/* WinSockサービス情報 */
WSADATA SocketAdapterImpl::s_WsaData;

/* エラーコード */
int SocketAdapterImpl::s_ErrorCode = 0;

#else

#endif
