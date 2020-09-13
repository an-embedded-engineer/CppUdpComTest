#pragma once
#include "SocketDataTypes.h"
#include "UdpPacketTypes.h"
#include "SocketReceiveQueue.h"
#include "UdpPacketRxBufferMap.h"
#include "AsyncTaskManager.h"

class UdpSocketReceiver;
class UdpPacketEncoder;

/* UDP Packet Senderクラス宣言 */
class UdpPacketReceiver final
{
public:
    /* シングルトンインスタンス取得 */
    static UdpPacketReceiver& GetInstance();

private:
    /* コンストラクタ */
    UdpPacketReceiver();
    /* デストラクタ */
    ~UdpPacketReceiver();

public:
    /* コピーコンストラクタ削除 */
    UdpPacketReceiver(const UdpPacketReceiver&) = delete;
    /* ムーブコンストラクタ削除 */
    UdpPacketReceiver(UdpPacketReceiver&&) = delete;
    /* コピー代入演算子削除 */
    UdpPacketReceiver& operator=(const UdpPacketReceiver&) = delete;
    /* ムーブ代入演算子削除 */
    UdpPacketReceiver& operator=(UdpPacketReceiver&&) = delete;

public:
    /* データ同期受信 */
    void ReceiveSync(UdpSocketReceiver& udp_socket_receiver);

    /* データ非同期受信開始 */
    void BeginReceiveAsync(UdpSocketReceiver& udp_socket_receiver);

    /* データ非同期受信停止 */
    void EndReceiveAsync(UdpSocketReceiver& udp_socket_receiver);

    /* 受信コールバックを登録 */
    void RegisterCallback(UdpPacketRxBufferMap::CallbackType callback);

private:
    void UdpPacketHandleTaskFunc(AsyncTask& task, CancellationPoint& cp);

private:
    /* UDP Packet Encoder */
    UdpPacketEncoder& m_Encoder;

    SocketReceiveQueue& m_SocketReceiveQueue;

    /* UDP Packet Rx Buffer Map */
    UdpPacketRxBufferMap& m_RxBufferMap;

    AsyncTaskManager& m_AsyncTaskManager;

    task_id_t m_UdpPacketHandleTaskID;
};
