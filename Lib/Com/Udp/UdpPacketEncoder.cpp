#include "UdpPacketEncoder.h"
#include "AppException.h"

/* �V���O���g���C���X�^���X�擾 */
UdpPacketEncoder& UdpPacketEncoder::GetInstance()
{
    static UdpPacketEncoder instance;
    return instance;
}

/* �R���X�g���N�^ */
UdpPacketEncoder::UdpPacketEncoder()
    : m_EnvironmentEndian(endian::GetEnvironmentEndian())
    , m_TargetEndian(EndianType::Big)
{
    /* Nothing to do */
}

/* �f�X�g���N�^ */
UdpPacketEncoder::~UdpPacketEncoder()
{
    /* Nothing to do */
}

/* ���̓f�[�^��UDP Packet�ɃG���R�[�h */
void UdpPacketEncoder::Encode(const UdpPacketHeader& src_header, byte_ptr src_data_ptr, size_t start_index, size_t data_size, UdpPacket& dst_packet)
{
    /* UDP Packet Header�̃G���f�B�A����ϊ� */
    this->ConvertEndian(src_header, dst_packet.Header);

    /* ���̓f�[�^����w�肳�ꂽ�͈͂�UDP Packet Data�ɃR�s�[ */
    memcpy(dst_packet.Data, &src_data_ptr[start_index], data_size);
}

/* ��MUDP Packet���f�R�[�h */
void UdpPacketEncoder::Decode(const UdpPacket& src_packet, const size_t rx_size, UdpPacket& dst_packet)
{
    /* UDP Packet Header�̃G���f�B�A����ϊ� */
    this->ConvertEndian(src_packet.Header, dst_packet.Header);

    /* UDP Packet Data�����N���A */
    memset(dst_packet.Data, 0, UDP_PACKET_DATA_SIZE);

    /* Check Packet Size */
    if (rx_size == dst_packet.Header.PacketSize)
    {
        size_t rx_data_size = rx_size - UDP_PACKET_HEADER_SIZE;

        /* ��M�f�[�^�����M�f�[�^�T�C�Y��UDP Packet Data�ɃR�s�[ */
        memcpy(dst_packet.Data, &src_packet.Data[0], rx_data_size);
    }
    else
    {
        THROW_APP_EXCEPTION("Packet Size Mismatch");
    }
}

/* UDP Packet Header�̃G���f�B�A����ϊ� */
void UdpPacketEncoder::ConvertEndian(const UdpPacketHeader& src, UdpPacketHeader& dst)
{
    /* �G���f�B�A��������̏ꍇ */
    if (this->m_EnvironmentEndian == this->m_TargetEndian)
    {
        /* UDP Packet Header�����̂܂܃R�s�[ */
        dst = src;
    }
    else
    {
        /* UDP Packet Header�̊e�����o�[�̃G���f�B�A����ϊ� */
        dst.PacketSize = endian::ConvertEndian(src.PacketSize);
        dst.SequenceNum = endian::ConvertEndian(src.SequenceNum);
        dst.TotalSize = endian::ConvertEndian(src.TotalSize);
        dst.TotalBlockNum = endian::ConvertEndian(src.TotalBlockNum);
        dst.CurrentBlockNum = endian::ConvertEndian(src.CurrentBlockNum);
        dst.MessageID = endian::ConvertEndian(src.MessageID);
        dst.Reserved = endian::ConvertEndian(src.Reserved);
    }
}

