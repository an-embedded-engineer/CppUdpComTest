#pragma once
#include "SocketAdapter.h"

#include <memory>

/* UDP Senderクラス宣言 */
class UdpSocketSender
{
public:
    /* コンストラクタ */
    UdpSocketSender();
    /* デストラクタ */
    ~UdpSocketSender();

    /* ユニキャスト用ソケットオープン */
    void OpenUniSocket(const std::string& remote_ip, const uint16_t remote_port);
    /* マルチキャスト用ソケットオープン */
    void OpenMultiSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl);
    /* ブロードキャスト用ソケットオープン */
    void OpenBroadSocket(const std::string& remote_ip, const uint16_t remote_port);
    /* ソケットクローズ */
    void CloseSocket();
    /* ソケットオープン確認 */
    bool IsSocketOpened();

    /* パケット送信 */
    void Transmit(const any_ptr data_ptr, const size_t tx_size);

private:
    /* Socket Adapterクラスインスタンス */
    std::unique_ptr<SocketAdapter> m_Adapter;
};

