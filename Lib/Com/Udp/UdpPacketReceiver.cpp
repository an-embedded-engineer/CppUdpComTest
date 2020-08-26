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
    , m_RxBufferMap(UdpPacketRxBufferMap::GetInstance())
{
    /* Nothing to do */
}

/* デストラクタ */
UdpPacketReceiver::~UdpPacketReceiver()
{
    /* Nothing to do */
}

/* データ受信 */
void UdpPacketReceiver::Receive(UdpSocketReceiver& udp_socket_receiver)
{
    /* UDP Packet受信バッファを生成 */
    UdpPacketBuffer udp_packet_buffer;

    /* UDP Packet受信バッファポインタを取得 */
    byte_ptr buffer_ptr = udp_packet_buffer.Binary;

    /* 受信サイズを初期化 */
    size_t rx_size = 0;

    /* UDP Packetを受信 */
    udp_socket_receiver.Receive(buffer_ptr, UDP_PACKET_SIZE, rx_size);

    /* UDP Packetを生成 */
    UdpPacket udp_packet;

    /* 受信UDP Packetをデコード */
    this->m_Encoder.Decode(udp_packet_buffer.Packet, rx_size, udp_packet);

    /* 受信UDP Packetを受信バッファに追加 */
    this->m_RxBufferMap.Add(udp_packet);
}

/* 受信コールバックを登録 */
void UdpPacketReceiver::RegisterCallback(UdpPacketRxBufferMap::CallbackType& callback)
{
    this->m_RxBufferMap.RegisterCallback(callback);
}

/* メッセージIDごとの受信コールバックを登録 */
void UdpPacketReceiver::RegisterCallback(uint16_t message_id, UdpPacketRxBuffer::CallbackType& callback)
{
    this->m_RxBufferMap.RegisterCallback(message_id, callback);
}
