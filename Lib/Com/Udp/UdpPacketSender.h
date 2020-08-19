#pragma once
#include "SocketDataTypes.h"
#include "UdpPacketTypes.h"

class UdpSocketSender;
class UdpPacketEncoder;

/* UDP Packet Senderクラス宣言 */
class UdpPacketSender final
{
public:
    /* シングルトンインスタンス取得 */
    static UdpPacketSender& GetInstance();

private:
    /* コンストラクタ */
    UdpPacketSender();
    /* デストラクタ */
    ~UdpPacketSender();

public:
    /* コピーコンストラクタ削除 */
    UdpPacketSender(const UdpPacketSender&) = delete;
    /* ムーブコンストラクタ削除 */
    UdpPacketSender(UdpPacketSender&&) = delete;
    /* コピー代入演算子削除 */
    UdpPacketSender& operator=(const UdpPacketSender&) = delete;
    /* ムーブ代入演算子削除 */
    UdpPacketSender& operator=(UdpPacketSender&&) = delete;

public:
    /* データ送信 */
    void Transmit(UdpSocketSender& udp_sockeet_sender, uint16_t message_id, byte_ptr data_ptr, size_t data_size);

private:
    /* データをUDPパケットに分割して送信 */
    void TransmitPacket(UdpSocketSender& udp_sockeet_sender, uint16_t message_id, byte_ptr data_ptr, size_t total_data_size, size_t total_block_num, size_t current_block_num, size_t start_index, size_t packet_data_size);

private:
    /* UDP Packet Encoder */
    UdpPacketEncoder& m_Encoder;
    /* シーケンス番号 */
    uint16_t m_SequenceNum;
};

