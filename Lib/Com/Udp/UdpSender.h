#pragma once
#include "UdpSocketSender.h"
#include "UdpPacketSender.h"

class UdpSender
{
public:
    /* �R���X�g���N�^ */
    UdpSender();
    /* �f�X�g���N�^ */
    ~UdpSender();

    /* ���j�L���X�g�p�\�P�b�g�I�[�v�� */
    void OpenUniSocket(const std::string& remote_ip, const uint16_t remote_port);
    /* �}���`�L���X�g�p�\�P�b�g�I�[�v�� */
    void OpenMultiSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl);
    /* �u���[�h�L���X�g�p�\�P�b�g�I�[�v�� */
    void OpenBroadSocket(const std::string& remote_ip, const uint16_t remote_port);
    /* �\�P�b�g�N���[�Y */
    void CloseSocket();
    /* �\�P�b�g�I�[�v���m�F */
    bool IsSocketOpened();

    /* �f�[�^���M */
    void Transmit(const uint16_t message_id, const byte_ptr data_ptr, const size_t tx_size);

private:
    /* UDP Socket Sender�N���X�C���X�^���X */
    std::unique_ptr<UdpSocketSender> m_SocketSender;

    /* UDP Packet Sender�N���X�C���X�^���X */
    UdpPacketSender& m_PacketSender;
};

