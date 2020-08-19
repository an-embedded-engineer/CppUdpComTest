#include "UdpPacketEncoder.h"
#include "AppException.h"

/* シングルトンインスタンス取得 */
UdpPacketEncoder& UdpPacketEncoder::GetInstance()
{
    static UdpPacketEncoder instance;
    return instance;
}

/* コンストラクタ */
UdpPacketEncoder::UdpPacketEncoder()
    : m_EnvironmentEndian(endian::GetEnvironmentEndian())
    , m_TargetEndian(EndianType::Big)
{
    /* Nothing to do */
}

/* デストラクタ */
UdpPacketEncoder::~UdpPacketEncoder()
{
    /* Nothing to do */
}

/* 入力データをUDP Packetにエンコード */
void UdpPacketEncoder::Encode(const UdpPacketHeader& src_header, byte_ptr src_data_ptr, size_t start_index, size_t data_size, UdpPacket& dst_packet)
{
    /* UDP Packet Headerのエンディアンを変換 */
    this->ConvertEndian(src_header, dst_packet.Header);

    /* 入力データから指定された範囲をUDP Packet Dataにコピー */
    memcpy(dst_packet.Data, &src_data_ptr[start_index], data_size);
}

/* 受信UDP Packetをデコード */
void UdpPacketEncoder::Decode(const UdpPacket& src_packet, const size_t rx_size, UdpPacket& dst_packet)
{
    /* UDP Packet Headerのエンディアンを変換 */
    this->ConvertEndian(src_packet.Header, dst_packet.Header);

    /* UDP Packet Data部をクリア */
    memset(dst_packet.Data, 0, UDP_PACKET_DATA_SIZE);

    /* Check Packet Size */
    if (rx_size == dst_packet.Header.PacketSize)
    {
        size_t rx_data_size = rx_size - UDP_PACKET_HEADER_SIZE;

        /* 受信データから受信データサイズをUDP Packet Dataにコピー */
        memcpy(dst_packet.Data, &src_packet.Data[0], rx_data_size);
    }
    else
    {
        THROW_APP_EXCEPTION("Packet Size Mismatch");
    }
}

/* UDP Packet Headerのエンディアンを変換 */
void UdpPacketEncoder::ConvertEndian(const UdpPacketHeader& src, UdpPacketHeader& dst)
{
    /* エンディアンが同一の場合 */
    if (this->m_EnvironmentEndian == this->m_TargetEndian)
    {
        /* UDP Packet Headerをそのままコピー */
        dst = src;
    }
    else
    {
        /* UDP Packet Headerの各メンバーのエンディアンを変換 */
        dst.PacketSize = endian::ConvertEndian(src.PacketSize);
        dst.SequenceNum = endian::ConvertEndian(src.SequenceNum);
        dst.TotalSize = endian::ConvertEndian(src.TotalSize);
        dst.TotalBlockNum = endian::ConvertEndian(src.TotalBlockNum);
        dst.CurrentBlockNum = endian::ConvertEndian(src.CurrentBlockNum);
        dst.MessageID = endian::ConvertEndian(src.MessageID);
        dst.Reserved = endian::ConvertEndian(src.Reserved);
    }
}

