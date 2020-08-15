#pragma once
#include "SocketDataTypes.h"

#include <memory>

/* Socket Adapter Implクラス前方宣言 */
class SocketAdapterImpl;

/* Socket Adapterクラス宣言 */
class SocketAdapter final
{
public:
    /* Socket全体の初期化 */
    static void Initialize();
    /* Socket全体の後始末 */
    static void Finalize();
    /* エラーコード取得 */
    static int GetErrorCode();

    /* コンストラクタ */
    SocketAdapter();
    /* デストラクタ */
    ~SocketAdapter();

    /* UDPユニキャスト送信用ソケットオープン */
    void OpenUdpUniTxSocket(const std::string& remote_ip, const uint16_t remote_port);
    /* UDPユニキャスト受信用ソケットオープン */
    void OpenUdpUniRxSocket(const uint16_t local_port);
    /* UDPマルチキャスト送信用ソケットオープン */
    void OpenUdpMultiTxSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl);
    /* UDPマルチキャスト受信用ソケットオープン */
    void OpenUdpMultiRxSocket(const std::string& multicast_ip, const uint16_t multicast_port);
    /* ソケットクローズ */
    void CloseSocket();

    /* ソケットオープン確認 */
    bool IsSocketOpened();

    /* パケット送信 */
    void Transmit(const any_ptr data_ptr, size_t tx_size);
    /* パケット同期受信 */
    void ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size);

private:
    /* Socket Adapter Implクラスインスタンス */
    std::unique_ptr<SocketAdapterImpl> m_Impl;
};
