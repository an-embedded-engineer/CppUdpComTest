#pragma once
#include "CompileSwitch.h"

#if COM_TYPE != COM_WINSOCK && COM_TYPE != COM_SOCKET && COM_TYPE != COM_MACSOCK
#include "SocketDataTypes.h"
#include "SocketException.h"

/* Socket Adapter Implクラス定義 */
class SocketAdapterImpl final
{
public:
    /* Socket全体の初期化 */
    static void Initialize()
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    /* Socket全体の後始末 */
    static void Finalize()
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    /* エラーコード取得 */
    static int GetErrorCode()
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    /* コンストラクタ */
    SocketAdapterImpl()
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    /* デストラクタ */
    ~SocketAdapterImpl()
    {
    }

    /* UDPユニキャスト送信用ソケットオープン */
    void OpenUdpUniTxSocket(const std::string& remote_ip, const uint16_t remote_port)
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    /* UDPユニキャスト受信用ソケットオープン */
    void OpenUdpUniRxSocket(const uint16_t local_port)
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    /* UDPマルチキャスト送信用ソケットオープン */
    void OpenUdpMultiTxSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port)
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    /* UDPマルチキャスト受信用ソケットオープン */
    void OpenUdpMultiRxSocket(const std::string& multicast_ip, const uint16_t multicast_port)
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    /* ソケットクローズ */
    void CloseSocket()
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    /* ソケットオープン確認 */
    bool IsSocketOpened()
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    /* パケット送信 */
    void Transmit(const any_ptr data_ptr, size_t tx_size)
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    /* パケット同期受信 */
    void ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }
};

#else
    #error Invalid Target Type : TARGET_TYPE
#endif
