#pragma once
#include "SocketAdapter.h"

#include <memory>

/* UDP Receiverクラス宣言 */
class UdpSocketReceiver
{
public:
    /* コンストラクタ */
    UdpSocketReceiver();
    /* デストラクタ */
    ~UdpSocketReceiver();

    /* ユニキャスト用ソケットオープン */
    void OpenUniSocket(const uint16_t local_port, const ReceiveType receive_type);
    /* マルチキャスト用ソケットオープン */
    void OpenMultiSocket(const std::string& multicast_ip, const uint16_t multicast_port, const ReceiveType receive_type);
    /* ブロードキャスト用ソケットオープン */
    void OpenBroadSocket(const uint16_t local_port, const ReceiveType receive_type);
    /* ソケットクローズ */
    void CloseSocket();
    /* ソケットオープン確認 */
    bool IsSocketOpened();

    /* パケット受信 */
    void Receive(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size);

private:
    /* Socket Adapterクラスインスタンス */
    std::unique_ptr<SocketAdapter> m_Adapter;
};

