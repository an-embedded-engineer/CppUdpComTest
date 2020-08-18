#pragma once
#include "SocketDataTypes.h"
#include "UdpPacketTypes.h"
#include "Endian.h"

/* UDP Packet Encoder�N���X�錾 */
class UdpPacketEncoder final
{
public:
    /* �V���O���g���C���X�^���X�擾 */
    static UdpPacketEncoder& GetInstance();

private:
    /* �R���X�g���N�^ */
    UdpPacketEncoder();
    /* �f�X�g���N�^ */
    ~UdpPacketEncoder();

public:
    /* �R�s�[�R���X�g���N�^�폜 */
    UdpPacketEncoder(const UdpPacketEncoder&) = delete;
    /* ���[�u�R���X�g���N�^�폜 */
    UdpPacketEncoder(UdpPacketEncoder&&) = delete;
    /* �R�s�[������Z�q�폜 */
    UdpPacketEncoder& operator=(const UdpPacketEncoder&) = delete;
    /* ���[�u������Z�q�폜 */
    UdpPacketEncoder& operator=(UdpPacketEncoder&&) = delete;

public:
    /* ���̓f�[�^��UDP Packet�ɃG���R�[�h */
    void Encode(const UdpPacketHeader& src_header, byte_ptr src_data_ptr, size_t start_index, size_t data_size, UdpPacket& dst_packet);

    /* ��MUDP Packet���f�R�[�h */
    void Decode(const UdpPacket& src_packet, const size_t rx_size, UdpPacket& dst_packet);

private:
    /* UDP Packet Header�̃G���f�B�A����ϊ� */
    void ConvertEndian(const UdpPacketHeader& src, UdpPacketHeader& dst);

private:
    /* ���s���̃G���f�B�A�� */
    EndianType m_EnvironmentEndian;
    /* �^�[�Q�b�g�G���f�B�A�� : �l�b�g���[�N�o�C�g�I�[�_�[ */
    EndianType m_TargetEndian;
};
