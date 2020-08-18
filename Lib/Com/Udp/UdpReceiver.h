#pragma once
#include "UdpSocketReceiver.h"
#include "UdpPacketReceiver.h"

/* UDP Receiver�N���X�錾 */
class UdpReceiver
{
public:
    /* �R���X�g���N�^ */
    UdpReceiver();
    /* �f�X�g���N�^ */
    ~UdpReceiver();

    /* ���j�L���X�g�p�\�P�b�g�I�[�v�� */
    void OpenUniSocket(const uint16_t local_port);
    /* �}���`�L���X�g�p�\�P�b�g�I�[�v�� */
    void OpenMultiSocket(const std::string& multicast_ip, const uint16_t multicast_port);
    /* �u���[�h�L���X�g�p�\�P�b�g�I�[�v�� */
    void OpenBroadSocket(const uint16_t local_port);
    /* �\�P�b�g�N���[�Y */
    void CloseSocket();
    /* �\�P�b�g�I�[�v���m�F */
    bool IsSocketOpened();

    /* �f�[�^������M */
    void ReceiveSync(uint16_t& message_id, std::string& message);

private:
    /* �f�[�^��M�R�[���o�b�N */
    void ReceiveCallback(const uint16_t message_id, const byte_ptr data_ptr, const size_t data_size);

private:
    /* UDP Socket Receiver�N���X�C���X�^���X */
    std::unique_ptr<UdpSocketReceiver> m_SocketReceiver;

    /* UDP Packet Receiver�N���X�C���X�^���X */
    UdpPacketReceiver& m_PacketReceiver;

    /* ��M���b�Z�[�WID */
    uint16_t m_MessageID;

    /* ��M���b�Z�[�W */
    std::string m_Message;

    /* ��M������� */
    bool m_IsReceived;
};

