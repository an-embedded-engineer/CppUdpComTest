#include "UdpPacketReceiver.h"
#include "UdpSocketReceiver.h"
#include "UdpPacketEncoder.h"

/* シングルトンインスタンス取得 */
UdpPacketReceiver& UdpPacketReceiver::GetInstance()
{
    static UdpPacketReceiver instance;
    return instance;
}

/* コンストラクタ */
UdpPacketReceiver::UdpPacketReceiver()
    : m_Encoder(UdpPacketEncoder::GetInstance())
    , m_SocketReceiveQueue(SocketReceiveQueue::GetInstance())
    , m_RxBufferMap(UdpPacketRxBufferMap::GetInstance())
    , m_AsyncTaskManager(AsyncTaskManager::GetInstance())
    , m_UdpPacketHandleTaskID(0)
{
    this->m_UdpPacketHandleTaskID = this->m_AsyncTaskManager.AddTask("UDP Packet Handle Task", &UdpPacketReceiver::UdpPacketHandleTaskFunc, this);
}

/* デストラクタ */
UdpPacketReceiver::~UdpPacketReceiver()
{
    /* Nothing to do */
}

/* データ同期受信 */
void UdpPacketReceiver::ReceiveSync(UdpSocketReceiver& udp_socket_receiver)
{
    /* UDP Packet受信バッファを生成 */
    UdpPacketBuffer udp_packet_buffer;

    /* UDP Packet受信バッファポインタを取得 */
    byte_ptr buffer_ptr = udp_packet_buffer.Binary;

    /* 受信サイズを初期化 */
    size_t rx_size = 0;

    /* UDP Packetを同期受信 */
    udp_socket_receiver.ReceiveSync(buffer_ptr, UDP_PACKET_SIZE, rx_size);

    /* UDP Packetを生成 */
    UdpPacket udp_packet;

    /* 受信UDP Packetをデコード */
    this->m_Encoder.Decode(udp_packet_buffer.Packet, rx_size, udp_packet);

    /* 受信UDP Packetを受信バッファに追加 */
    this->m_RxBufferMap.Add(udp_packet);
}

/* 受信コールバックを登録 */
void UdpPacketReceiver::RegisterCallback(UdpPacketRxBufferMap::CallbackType callback)
{
    this->m_RxBufferMap.RegisterCallback(callback);
}

/* データ非同期受信開始 */
void UdpPacketReceiver::BeginReceiveAsync(UdpSocketReceiver& udp_socket_receiver)
{
    this->m_AsyncTaskManager.Start(this->m_UdpPacketHandleTaskID);

    udp_socket_receiver.BeginReceiveAsync();
}

/* データ非同期受信停止 */
void UdpPacketReceiver::EndReceiveAsync(UdpSocketReceiver& udp_socket_receiver)
{
    udp_socket_receiver.EndReceiveAsync();

    this->m_AsyncTaskManager.Stop(this->m_UdpPacketHandleTaskID);
}

void UdpPacketReceiver::UdpPacketHandleTaskFunc(AsyncTask& task, CancellationPoint& cp)
{
    while (true)
    {
        cp.CheckCancelRequested();

        SocketReceiveData receive_data;

        this->m_SocketReceiveQueue.Dequeue(receive_data, cp);

        cp.CheckCancelRequested();

        /* UDP Packet受信バッファを生成 */
        UdpPacketBuffer udp_packet_buffer;

        /* UDP Packet受信バッファポインタを取得 */
        byte_ptr buffer_ptr = udp_packet_buffer.Binary;

        memcpy(buffer_ptr, receive_data.GetDataPtr(), receive_data.GetDataSize());

        size_t rx_size = receive_data.GetDataSize();

        /* UDP Packetを生成 */
        UdpPacket udp_packet;

        /* 受信UDP Packetをデコード */
        this->m_Encoder.Decode(udp_packet_buffer.Packet, rx_size, udp_packet);

        cp.CheckCancelRequested();

        /* 受信UDP Packetを受信バッファに追加 */
        this->m_RxBufferMap.Add(udp_packet);
    }
}
