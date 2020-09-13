#include "SocketInterface.h"
#include "SocketException.h"
#include "AppException.h"

#if COM_TYPE == COM_WINSOCK
static WSADATA s_WsaData;
#endif

static int s_ErrorCode;


void SocketInterface::Initialize()
{
#if COM_TYPE == COM_WINSOCK
    /* WinSockサービススタートアップ */
    int startup_result = WSAStartup(MAKEWORD(2, 0), &s_WsaData);

    /* WinSockサービスススタートアップ失敗時のエラー処理 */
    if (startup_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = startup_result;

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("WSA Startup Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* Nothing to do */
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::Finalize()
{
#if COM_TYPE == COM_WINSOCK
    /* WinSockサービスクリーンアップ */
    int cleanup_result = WSACleanup();

    /* WinSockサービスクリーンアップ失敗時のエラー処理 */
    if (cleanup_result == SOCKET_ERROR)
    {
        s_ErrorCode = WSAGetLastError();

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("WSA Cleanup Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* Nothing to do */
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

int SocketInterface::GetErrorCode()
{
    return s_ErrorCode;
}

SocketType SocketInterface::OpenUdpSocket()
{
#if COM_TYPE == COM_WINSOCK
    /* UDP用ソケットをオープン */
    SocketType sock = socket(AF_INET, SOCK_DGRAM, 0);

    /* ソケットオープン失敗時のエラー処理 */
    if (sock == INVALID_SOCKET)
    {
        /* エラーコードセット */
        s_ErrorCode = WSAGetLastError();

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("UDP Socket Open Failed", s_ErrorCode);
    }

    return sock;
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* UDP用ソケットをオープン */
    SocketType sock = socket(AF_INET, SOCK_DGRAM, 0);

    /* ソケットオープン失敗時のエラー処理 */
    if (sock < 0)
    {
        /* エラーコードセット */
        s_ErrorCode = errno;

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("UDP Socket Open Failed", s_ErrorCode);
    }

    return sock;
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::CloseSocket(SocketType socket)
{
#if COM_TYPE == COM_WINSOCK
    /* ソケットクローズ */
    int close_result = closesocket(socket);

    /* ソケットクローズ失敗時のエラー処理 */
    if (close_result == SOCKET_ERROR)
    {
        /* エラーコードセット */
        s_ErrorCode = WSAGetLastError();

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Socket Close Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* ソケットクローズ */
    int close_result = close(socket);

    /* ソケットクローズ失敗時のエラー処理 */
    if (close_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = errno;

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Socket Close Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

SocketAddressType SocketInterface::GetTxSocketAddress(const std::string& dst_ip, const uint16_t dst_port)
{
#if COM_TYPE == COM_WINSOCK
    SocketAddressType sock_address;

    /* 送信用アドレス情報セット */
    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(dst_port);
    sock_address.sin_addr.S_un.S_addr = SocketInterface::ConvertIpStrToNum(sock_address.sin_family, dst_ip);

    return sock_address;
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    SocketAddressType sock_address;

    /* 送信用アドレス情報セット */
    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(dst_port);
    sock_address.sin_addr.s_addr = inet_addr(dst_ip.c_str());
#if COM_TYPE == COM_MACSOCK
    sock_address.sin_len = sizeof(sock_address);
#endif

    return sock_address;
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

SocketAddressType SocketInterface::GetRxSocketAddress(const uint16_t src_port)
{
#if COM_TYPE == COM_WINSOCK
    SocketAddressType sock_address;

    /* 受信用アドレス情報セット */
    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(src_port);
    sock_address.sin_addr.S_un.S_addr = INADDR_ANY;

    return sock_address;
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    SocketAddressType sock_address;

    /* 受信用アドレス情報セット */
    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(src_port);
    sock_address.sin_addr.s_addr = INADDR_ANY;
#if COM_TYPE == COM_MACSOCK
    sock_address.sin_len = sizeof(sock_address);
#endif

    return sock_address;
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::BindSocket(SocketType& socket, const SocketAddressType& socket_address)
{
#if COM_TYPE == COM_WINSOCK
    /* ソケットにアドレス情報をバインド */
    int bind_result = bind(socket, (struct sockaddr*)&socket_address, sizeof(socket_address));

    /* ソケットバインド失敗時のエラー処理 */
    if (bind_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = WSAGetLastError();

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Socket Bind Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* ソケットにアドレス情報をバインド */
    int bind_result = bind(socket, (struct sockaddr*)&socket_address, sizeof(socket_address));

    /* ソケットバインド失敗時のエラー処理 */
    if (bind_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = errno;

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Socket Bind Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::SetNonBlockingMode(SocketType& socket)
{
#if COM_TYPE == COM_WINSOCK
    /* ノンブロッキングI/Oモードにセット */
    u_long val = 1;
    int ioctrl_result = ioctlsocket(socket, FIONBIO, &val);

    /* ノンブロッキングI/Oモード設定失敗時のエラー処理 */
    if (ioctrl_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = WSAGetLastError();

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Socket I/O Control(Set Non Blocking Mode) Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* ノンブロッキングI/Oモードにセット */
    int val = 1;
    int ioctrl_result = ioctl(socket, FIONBIO, &val);

    /* ノンブロッキングI/Oモード設定失敗時のエラー処理 */
    if (ioctrl_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = errno;

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Socket I/O Control(Set Non Blocking Mode) Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::SetMulticastTxSocketOption(SocketType& socket, const SocketAddressType& socket_address, const std::string& src_ip)
{
#if COM_TYPE == COM_WINSOCK
    /* ローカルIPアドレスセット */
    IpAddressType local_ip_num = SocketInterface::ConvertIpStrToNum(socket_address.sin_family, src_ip);

    /* UDPマルチキャストソケットオプションセット */
    int set_opt_result = setsockopt(socket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&local_ip_num, sizeof(local_ip_num));

    /* ソケットオプションセット失敗時のエラー処理 */
    if (set_opt_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = WSAGetLastError();

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Multicast Tx Socket Option Set Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* ローカルIPアドレスセット */
    IpAddressType local_ip_num = inet_addr(src_ip.c_str());

    /* UDPマルチキャストソケットオプションセット */
    int set_opt_result = setsockopt(socket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&local_ip_num, sizeof(local_ip_num));

    /* ソケットオプションセット失敗時のエラー処理 */
    if (set_opt_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = errno;

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Multicast Tx Socket Option Set Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::SetMulticastRxSocketOption(SocketType& socket, const SocketAddressType& socket_address, const std::string& dst_ip)
{
#if COM_TYPE == COM_WINSOCK
    MulticastRequestType multicast_request;

    /* マルチキャストリクエストのセット */
    memset(&multicast_request, 0, sizeof(multicast_request));
    multicast_request.imr_interface.S_un.S_addr = INADDR_ANY;
    multicast_request.imr_multiaddr.S_un.S_addr = SocketInterface::ConvertIpStrToNum(socket_address.sin_family, dst_ip);

    /* UDPマルチキャストソケットオプションセット */
    int set_opt_result = setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multicast_request, sizeof(multicast_request));

    /* ソケットオプションセット失敗時のエラー処理 */
    if (set_opt_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = WSAGetLastError();

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Multicast Rx Socket Option Set Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    MulticastRequestType multicast_request;

    /* マルチキャストリクエストのセット */
    memset(&multicast_request, 0, sizeof(multicast_request));
    multicast_request.imr_interface.s_addr = INADDR_ANY;
    multicast_request.imr_multiaddr.s_addr = inet_addr(dst_ip.c_str());

    /* UDPマルチキャストソケットオプションセット */
    int set_opt_result = setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multicast_request, sizeof(multicast_request));

    /* ソケットオプションセット失敗時のエラー処理 */
    if (set_opt_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = errno;

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Multicast Rx Socket Option Set Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::SetBroadcastTxSocketOption(SocketType& socket)
{
#if COM_TYPE == COM_WINSOCK
    /* UDPブロードキャスト送信用ソケットオプションセット */
    BOOL yes = 1;
    int set_opt_result = setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));

    /* ソケットオプションセット失敗時のエラー処理 */
    if (set_opt_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = WSAGetLastError();

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Broadcast Tx Socket Option Set Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* UDPブロードキャスト送信用ソケットオプションセット */
    int yes = 1;
    int set_opt_result = setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));

    /* ソケットオプションセット失敗時のエラー処理 */
    if (set_opt_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = errno;

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Broadcast Tx Socket Option Set Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::SetTTLSocketOption(SocketType& socket, const int32_t ttl)
{
#if COM_TYPE == COM_WINSOCK
    /* TTLセット */
    TTLType ttl_value = static_cast<TTLType>(ttl);

    /* TTLソケットオプションセット */
    int set_ttl_result = setsockopt(socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl_value, sizeof(ttl_value));

    /* ソケットオプションセット失敗時のエラー処理 */
    if (set_ttl_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = WSAGetLastError();

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("TTL Socket Option Set Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* TTLセット */
    TTLType ttl_value = static_cast<TTLType>(ttl);

    /* TTLソケットオプションセット */
    int set_ttl_result = setsockopt(socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl_value, sizeof(ttl_value));

    /* ソケットオプションセット失敗時のエラー処理 */
    if (set_ttl_result != 0)
    {
        /* エラーコードセット */
        s_ErrorCode = errno;

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("TTL Socket Option Set Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::Transmit(SocketType& socket, const SocketAddressType& socket_address, const any_ptr data_ptr, const size_t tx_size)
{
#if COM_TYPE == COM_WINSOCK
    /* ソケットにパケット送信 */
    int send_result = sendto(socket, (const char*)data_ptr, (int)tx_size, 0, (struct sockaddr*)&socket_address, sizeof(socket_address));

    /* パケット送信失敗時のエラー処理 */
    if (send_result == SOCKET_ERROR)
    {
        /* エラーコードセット */
        s_ErrorCode = WSAGetLastError();

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Packet Transmit Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* ソケットにパケット送信 */
    int send_result = sendto(socket, (const char*)data_ptr, (int)tx_size, 0, (struct sockaddr*)&socket_address, sizeof(socket_address));

    /* パケット送信失敗時のエラー処理 */
    if (send_result < 0)
    {
        /* エラーコードセット */
        s_ErrorCode = errno;

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Packet Transmit Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::ReceiveSync(SocketType& socket, byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
{
#if COM_TYPE == COM_WINSOCK
    /* ソケットからパケット受信(ブロッキング) */
    int receive_result = recv(socket, (char*)buffer_ptr, (int)buffer_size, 0);

    /* パケット受信失敗時のエラー処理 */
    if (receive_result == SOCKET_ERROR)
    {
        /* エラーコードセット */
        s_ErrorCode = WSAGetLastError();

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Packet Receive Failed", s_ErrorCode);
    }
    /* パケット受信成功時 */
    else
    {
        /* 受信データサイズをセット */
        rx_size = (size_t)receive_result;
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* ソケットからパケット受信(ブロッキング) */
    int receive_result = recv(socket, (char*)buffer_ptr, (int)buffer_size, 0);

    /* パケット受信失敗時のエラー処理 */
    if (receive_result < 0)
    {
        /* エラーコードセット */
        s_ErrorCode = errno;

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Packet Receive Failed", s_ErrorCode);
    }
    /* パケット受信成功時 */
    else
    {
        /* 受信データサイズをセット */
        rx_size = (size_t)receive_result;
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

bool SocketInterface::ReceiveAsync(SocketType& socket, byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
{
#if COM_TYPE == COM_WINSOCK
    /* ソケットからパケット受信(ノンブロッキングブロッキング) */
    int receive_result = recv(socket, (char*)buffer_ptr, (int)buffer_size, 0);

    /* パケット受信失敗時のエラー処理 */
    if (receive_result == SOCKET_ERROR)
    {
        /* エラーコード取得 */
        int error_code = WSAGetLastError();

        /* データ未受信の場合 */
        if (error_code == WSAEWOULDBLOCK)
        {
            rx_size = 0;

            return false;
        }
        else
        {
            /* エラーコードセット */
            s_ErrorCode = error_code;

            /* ソケット例外送出 */
            THROW_SOCKET_EXCEPTION("Packet Receive Failed", s_ErrorCode);
        }
    }
    /* パケット受信成功時 */
    else
    {
        /* 受信データサイズをセット */
        rx_size = (size_t)receive_result;

        return true;
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* ソケットからパケット受信(ノンブロッキングブロッキング) */
    int receive_result = recv(socket, (char*)buffer_ptr, (int)buffer_size, 0);

    /* パケット受信失敗時のエラー処理 */
    if (receive_result < 0)
    {
        /* エラーコード取得 */
        int error_code = errno;

        /* データ未受信の場合 */
        if (error_code == EAGAIN)
        {
            rx_size = 0;

            return false;
        }
        else
        {
            /* エラーコードセット */
            s_ErrorCode = error_code;

            /* ソケット例外送出 */
            THROW_SOCKET_EXCEPTION("Packet Receive Failed", s_ErrorCode);
        }
    }
    /* パケット受信成功時 */
    else
    {
        /* 受信データサイズをセット */
        rx_size = (size_t)receive_result;

        return true;
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

#if COM_TYPE == COM_WINSOCK
/* IPアドレスの変換(文字列 -> 数値) */
IpAddressType SocketInterface::ConvertIpStrToNum(const AddressFamilyType family, const std::string& ip)
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
        s_ErrorCode = -1;

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Convert IP Address Failed", s_ErrorCode);
    }
    else
    {
        /* エラーコードセット */
        s_ErrorCode = WSAGetLastError();

        /* ソケット例外送出 */
        THROW_SOCKET_EXCEPTION("Convert IP Address Failed", s_ErrorCode);
    }
}
#endif
