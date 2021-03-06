﻿#pragma once
#include "SocketDataTypes.h"
#include "ReceiveType.h"

#include <memory>
#include <functional>

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
    /* Socket全体の初期化確認 */
    static bool IsInitialized();
    /* エラーコード取得 */
    static int GetErrorCode();

    /* コンストラクタ */
    SocketAdapter();
    /* デストラクタ */
    ~SocketAdapter();

    /* UDPユニキャスト送信用ソケットオープン */
    void OpenUdpUniTxSocket(const std::string& remote_ip, const uint16_t remote_port);
    /* UDPユニキャスト受信用ソケットオープン */
    void OpenUdpUniRxSocket(const uint16_t local_port, const ReceiveType receive_type);
    /* UDPマルチキャスト送信用ソケットオープン */
    void OpenUdpMultiTxSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl);
    /* UDPマルチキャスト受信用ソケットオープン */
    void OpenUdpMultiRxSocket(const std::string& multicast_ip, const uint16_t multicast_port, const ReceiveType receive_type);
    /* UDPブロードキャスト送信用ソケットオープン */
    void OpenUdpBroadTxSocket(const std::string& remote_ip, const uint16_t remote_port);
    /* UDPブロードキャスト受信用ソケットオープン */
    void OpenUdpBroadRxSocket(const uint16_t local_port, const ReceiveType receive_type);
    /* ソケットクローズ */
    void CloseSocket();

    /* ソケットオープン確認 */
    bool IsSocketOpened();

    /* パケット送信 */
    void Transmit(const any_ptr data_ptr, const size_t tx_size);
    /* パケット同期受信 */
    void ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size);
    /* パケット非同期受信開始 */
    void BeginReceiveAsync();
    /* パケット非同期受信停止 */
    void EndReceiveAsync();

private:
    /* Socket Adapter Implクラスインスタンス */
    std::unique_ptr<SocketAdapterImpl> m_Impl;
};
