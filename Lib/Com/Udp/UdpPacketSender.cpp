#include "UdpPacketSender.h"
#include "UdpPacketEncoder.h"
#include "UdpSocketSender.h"

#include <iostream>

/* シングルトンインスタンス取得 */
UdpPacketSender& UdpPacketSender::GetInstance()
{
    static UdpPacketSender instance;
    return instance;
}

/* コンストラクタ */
UdpPacketSender::UdpPacketSender()
    : m_Encoder(UdpPacketEncoder::GetInstance())
    , m_SequenceNum(0)
{
    /* Nothing to do */
}

/* デストラクタ */
UdpPacketSender::~UdpPacketSender()
{
    /* Nothing to do */
}

/* データ送信 */
void UdpPacketSender::Transmit(UdpSocketSender& udp_sockeet_sender, uint16_t message_id, byte_ptr data_ptr, size_t data_size)
{
    /* 合計データサイズ */
    size_t total_data_size = data_size;

    /* データサイズチェック */
    if (total_data_size > 0)
    {
        /* ブロック数 = 合計データサイズ / UDPパケットデータサイズ */
        size_t div_block_num = data_size / UDP_PACKET_DATA_SIZE;

        /* 余りブロックサイズ = 合計データサイズ / UDPパケットデータサイズの剰余 */
        size_t mod_block_size = data_size % UDP_PACKET_DATA_SIZE;

        /* 合計ブロックサイズを算出(余りブロックサイズがある場合は、ブロック数 + 1) */
        size_t total_block_num = (mod_block_size > 0) ? div_block_num + 1 : div_block_num;

        std::cout << "Start Transmit UDP Packet : Size = " << total_data_size << " Blocks = " << total_block_num << std::endl;

        /* 送信データをブロックに分割してUDPパケットとして送信 */
        for (size_t i = 0; i < total_block_num; i++)
        {
            /* 現在のブロック番号 */
            size_t current_block_num = i + 1;

            /* 現在のブロックの開始インデックス */
            size_t start_index = i * UDP_PACKET_DATA_SIZE;

            /* 現在のブロックのデータサイズ */
            size_t packet_data_size = (current_block_num == total_block_num) ? mod_block_size : UDP_PACKET_DATA_SIZE;

            std::cout << "Transmit UDP Packet : Block = " << current_block_num << "/" << total_block_num << " Packet Size = " << packet_data_size << " Start Index = " << start_index << std::endl;

            /* データをUDPパケットに分割して送信 */
            this->TransmitPacket(udp_sockeet_sender, message_id, data_ptr, total_data_size, total_block_num, current_block_num, start_index, packet_data_size);
        }
    }
    else
    {
        std::cerr << "[WARN] Transmit Data Size = 0" << std::endl;

        /* 合計ブロックサイズ */
        size_t total_block_num = 1;

        std::cout << "Start UDP Packet Transmit : Size = " << total_data_size << " Blocks = " << total_block_num << std::endl;

        /* 現在のブロック番号 */
        size_t current_block_num = 1;

        /* 現在のブロックの開始インデックス */
        size_t start_index = 0;

        /* 現在のブロックのデータサイズ */
        size_t packet_data_size = total_data_size;

        std::cout << "Transmit UDP Packet : Block = " << current_block_num << "/" << total_block_num << " Packet Size = " << packet_data_size << " Start Index = " << start_index << std::endl;

        /* データをUDPパケットに分割して送信 */
        this->TransmitPacket(udp_sockeet_sender, message_id, data_ptr, total_data_size, total_block_num, current_block_num, start_index, packet_data_size);
    }
}

/* データをUDPパケットに分割して送信 */
void UdpPacketSender::TransmitPacket(UdpSocketSender& udp_sockeet_sender, uint16_t message_id, byte_ptr data_ptr, size_t total_data_size, size_t total_block_num, size_t current_block_num, size_t start_index, size_t packet_data_size)
{
    /* 現在のブロックのパケットサイズ */
    size_t packet_size = UDP_PACKET_HEADER_SIZE + packet_data_size;

    /* パケットヘッダを作成 */
    UdpPacketHeader src_header;
    src_header.PacketSize = (uint16_t)(packet_size);
    src_header.SequenceNum = this->m_SequenceNum;
    src_header.TotalSize = (uint32_t)total_data_size;
    src_header.TotalBlockNum = (uint32_t)total_block_num;
    src_header.CurrentBlockNum = (uint32_t)current_block_num;
    src_header.MessageID = message_id;
    src_header.Reserved = 0;

    /* UDP Packetインスタンスを生成 */
    UdpPacket dst_packet;

    /* UDP Packetをネットワークバイトオーダーにエンコード */
    this->m_Encoder.Encode(src_header, data_ptr, start_index, packet_data_size, dst_packet);

    /* UDP Packetを送信 */
    udp_sockeet_sender.Transmit((any_ptr)&dst_packet, packet_size);

    /* シーケンス番号をインクリメント */
    this->m_SequenceNum = (this->m_SequenceNum == UINT16_MAX) ? 0 : this->m_SequenceNum + 1;
}
