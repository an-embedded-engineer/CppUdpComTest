#pragma once
#include "SocketDataTypes.h"
#include "UdpPacketTypes.h"
#include "Endian.h"

/* UDP Packet Encoderクラス宣言 */
class UdpPacketEncoder final
{
public:
    /* シングルトンインスタンス取得 */
    static UdpPacketEncoder& GetInstance();

private:
    /* コンストラクタ */
    UdpPacketEncoder();
    /* デストラクタ */
    ~UdpPacketEncoder();

public:
    /* コピーコンストラクタ削除 */
    UdpPacketEncoder(const UdpPacketEncoder&) = delete;
    /* ムーブコンストラクタ削除 */
    UdpPacketEncoder(UdpPacketEncoder&&) = delete;
    /* コピー代入演算子削除 */
    UdpPacketEncoder& operator=(const UdpPacketEncoder&) = delete;
    /* ムーブ代入演算子削除 */
    UdpPacketEncoder& operator=(UdpPacketEncoder&&) = delete;

public:
    /* 入力データをUDP Packetにエンコード */
    void Encode(const UdpPacketHeader& src_header, byte_ptr src_data_ptr, size_t start_index, size_t data_size, UdpPacket& dst_packet);

    /* 受信UDP Packetをデコード */
    void Decode(const UdpPacket& src_packet, const size_t rx_size, UdpPacket& dst_packet);

private:
    /* UDP Packet Headerのエンディアンを変換 */
    void ConvertEndian(const UdpPacketHeader& src, UdpPacketHeader& dst);

private:
    /* 実行環境のエンディアン */
    EndianType m_EnvironmentEndian;
    /* ターゲットエンディアン : ネットワークバイトオーダー */
    EndianType m_TargetEndian;
};
