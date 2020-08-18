#include "UdpPacketSender.h"
#include "UdpPacketEncoder.h"
#include "UdpSocketSender.h"

#include <iostream>

/* �V���O���g���C���X�^���X�擾 */
UdpPacketSender& UdpPacketSender::GetInstance()
{
    static UdpPacketSender instance;
    return instance;
}

/* �R���X�g���N�^ */
UdpPacketSender::UdpPacketSender()
    : m_Encoder(UdpPacketEncoder::GetInstance())
    , m_SequenceNum(0)
{
    /* Nothing to do */
}

/* �f�X�g���N�^ */
UdpPacketSender::~UdpPacketSender()
{
    /* Nothing to do */
}

/* �f�[�^���M */
void UdpPacketSender::Transmit(UdpSocketSender& udp_sockeet_sender, uint16_t message_id, byte_ptr data_ptr, size_t data_size)
{
    /* ���v�f�[�^�T�C�Y */
    size_t total_data_size = data_size;

    /* �f�[�^�T�C�Y�`�F�b�N */
    if (total_data_size > 0)
    {
        /* �u���b�N�� = ���v�f�[�^�T�C�Y / UDP�p�P�b�g�f�[�^�T�C�Y */
        size_t div_block_num = data_size / UDP_PACKET_DATA_SIZE;

        /* �]��u���b�N�T�C�Y = ���v�f�[�^�T�C�Y / UDP�p�P�b�g�f�[�^�T�C�Y�̏�] */
        size_t mod_block_size = data_size % UDP_PACKET_DATA_SIZE;

        /* ���v�u���b�N�T�C�Y���Z�o(�]��u���b�N�T�C�Y������ꍇ�́A�u���b�N�� + 1) */
        size_t total_block_num = (mod_block_size > 0) ? div_block_num + 1 : div_block_num;

        std::cout << "Start Transmit UDP Packet : Size = " << total_data_size << " Blocks = " << total_block_num << std::endl;

        /* ���M�f�[�^���u���b�N�ɕ�������UDP�p�P�b�g�Ƃ��đ��M */
        for (size_t i = 0; i < total_block_num; i++)
        {
            /* ���݂̃u���b�N�ԍ� */
            size_t current_block_num = i + 1;

            /* ���݂̃u���b�N�̊J�n�C���f�b�N�X */
            size_t start_index = i * UDP_PACKET_DATA_SIZE;

            /* ���݂̃u���b�N�̃f�[�^�T�C�Y */
            size_t packet_data_size = (current_block_num == total_block_num) ? mod_block_size : UDP_PACKET_DATA_SIZE;

            std::cout << "Transmit UDP Packet : Block = " << current_block_num << "/" << total_block_num << " Packet Size = " << packet_data_size << " Start Index = " << start_index << std::endl;

            /* �f�[�^��UDP�p�P�b�g�ɕ������đ��M */
            this->TransmitPacket(udp_sockeet_sender, message_id, data_ptr, total_data_size, total_block_num, current_block_num, start_index, packet_data_size);
        }
    }
    else
    {
        std::cerr << "[WARN] Transmit Data Size = 0" << std::endl;

        /* ���v�u���b�N�T�C�Y */
        size_t total_block_num = 1;

        std::cout << "Start UDP Packet Transmit : Size = " << total_data_size << " Blocks = " << total_block_num << std::endl;

        /* ���݂̃u���b�N�ԍ� */
        size_t current_block_num = 1;

        /* ���݂̃u���b�N�̊J�n�C���f�b�N�X */
        size_t start_index = 0;

        /* ���݂̃u���b�N�̃f�[�^�T�C�Y */
        size_t packet_data_size = total_data_size;

        std::cout << "Transmit UDP Packet : Block = " << current_block_num << "/" << total_block_num << " Packet Size = " << packet_data_size << " Start Index = " << start_index << std::endl;

        /* �f�[�^��UDP�p�P�b�g�ɕ������đ��M */
        this->TransmitPacket(udp_sockeet_sender, message_id, data_ptr, total_data_size, total_block_num, current_block_num, start_index, packet_data_size);
    }
}

/* �f�[�^��UDP�p�P�b�g�ɕ������đ��M */
void UdpPacketSender::TransmitPacket(UdpSocketSender& udp_sockeet_sender, uint16_t message_id, byte_ptr data_ptr, size_t total_data_size, size_t total_block_num, size_t current_block_num, size_t start_index, size_t packet_data_size)
{
    /* ���݂̃u���b�N�̃p�P�b�g�T�C�Y */
    size_t packet_size = UDP_PACKET_HEADER_SIZE + packet_data_size;

    /* �p�P�b�g�w�b�_���쐬 */
    UdpPacketHeader src_header;
    src_header.PacketSize = (uint16_t)(packet_size);
    src_header.SequenceNum = this->m_SequenceNum;
    src_header.TotalSize = (uint32_t)total_data_size;
    src_header.TotalBlockNum = (uint32_t)total_block_num;
    src_header.CurrentBlockNum = (uint32_t)current_block_num;
    src_header.MessageID = message_id;
    src_header.Reserved = 0;

    /* UDP Packet�C���X�^���X�𐶐� */
    UdpPacket dst_packet;

    /* UDP Packet���l�b�g���[�N�o�C�g�I�[�_�[�ɃG���R�[�h */
    this->m_Encoder.Encode(src_header, data_ptr, start_index, packet_data_size, dst_packet);

    /* UDP Packet�𑗐M */
    udp_sockeet_sender.Transmit((any_ptr)&dst_packet, packet_size);

    /* �V�[�P���X�ԍ����C���N�������g */
    this->m_SequenceNum = (this->m_SequenceNum == UINT16_MAX) ? 0 : this->m_SequenceNum + 1;
}
