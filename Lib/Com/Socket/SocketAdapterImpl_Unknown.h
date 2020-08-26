﻿#pragma once
#include "CompileSwitch.h"

#if COM_TYPE != COM_WINSOCK && COM_TYPE != COM_SOCKET && COM_TYPE != COM_MACSOCK
#include "SocketDataTypes.h"
#include "AppException.h"
#include "ReceiveType.h"

/* Socket Adapter Implクラス定義 */
class SocketAdapterImpl final
{
public:
    /* Socket全体の初期化 */
    static void Initialize()
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* Socket全体の後始末 */
    static void Finalize()
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* エラーコード取得 */
    static int GetErrorCode()
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* コンストラクタ */
    SocketAdapterImpl()
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* デストラクタ */
    ~SocketAdapterImpl()
    {
    }

    /* UDPユニキャスト送信用ソケットオープン */
    void OpenUdpUniTxSocket(const std::string& remote_ip, const uint16_t remote_port)
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* UDPユニキャスト受信用ソケットオープン */
    void OpenUdpUniRxSocket(const uint16_t local_port, const ReceiveType receive_type)
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* UDPマルチキャスト送信用ソケットオープン */
    void OpenUdpMultiTxSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl)
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* UDPマルチキャスト受信用ソケットオープン */
    void OpenUdpMultiRxSocket(const std::string& multicast_ip, const uint16_t multicast_port, const ReceiveType receive_type)
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* UDPブロードキャスト送信用ソケットオープン */
    void OpenUdpBroadTxSocket(const std::string& remote_ip, const uint16_t remote_port)
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* UDPブロードキャスト受信用ソケットオープン */
    void OpenUdpBroadRxSocket(const uint16_t local_port, const ReceiveType receive_type)
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* ソケットクローズ */
    void CloseSocket()
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* ソケットオープン確認 */
    bool IsSocketOpened()
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* パケット送信 */
    void Transmit(const any_ptr data_ptr, const size_t tx_size)
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }

    /* パケット受信 */
    void Receive(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
    {
        THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
    }
};

#else

#endif
