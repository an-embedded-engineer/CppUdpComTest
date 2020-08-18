#pragma once
#include "SocketDataTypes.h"
#include "UdpPacketTypes.h"

class UdpSocketSender;
class UdpPacketEncoder;

/* UDP Packet Sender�N���X�錾 */
class UdpPacketSender final
{
public:
    /* �V���O���g���C���X�^���X�擾 */
    static UdpPacketSender& GetInstance();

private:
    /* �R���X�g���N�^ */
    UdpPacketSender();
    /* �f�X�g���N�^ */
    ~UdpPacketSender();

public:
    /* �R�s�[�R���X�g���N�^�폜 */
    UdpPacketSender(const UdpPacketSender&) = delete;
    /* ���[�u�R���X�g���N�^�폜 */
    UdpPacketSender(UdpPacketSender&&) = delete;
    /* �R�s�[������Z�q�폜 */
    UdpPacketSender& operator=(const UdpPacketSender&) = delete;
    /* ���[�u������Z�q�폜 */
    UdpPacketSender& operator=(UdpPacketSender&&) = delete;

public:
    /* �f�[�^���M */
    void Transmit(UdpSocketSender& udp_sockeet_sender, uint16_t message_id, byte_ptr data_ptr, size_t data_size);

private:
    /* �f�[�^��UDP�p�P�b�g�ɕ������đ��M */
    void TransmitPacket(UdpSocketSender& udp_sockeet_sender, uint16_t message_id, byte_ptr data_ptr, size_t total_data_size, size_t total_block_num, size_t current_block_num, size_t start_index, size_t packet_data_size);

private:
    /* UDP Packet Encoder */
    UdpPacketEncoder& m_Encoder;
    /* �V�[�P���X�ԍ� */
    uint16_t m_SequenceNum;
};

