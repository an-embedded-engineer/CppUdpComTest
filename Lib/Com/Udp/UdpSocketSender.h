#pragma once
#include "SocketAdapter.h"

#include <memory>

/* UDP Sender�N���X�錾 */
class UdpSocketSender
{
public:
    /* �R���X�g���N�^ */
    UdpSocketSender();
    /* �f�X�g���N�^ */
    ~UdpSocketSender();

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

    /* �p�P�b�g���M */
    void Transmit(const any_ptr data_ptr, const size_t tx_size);

private:
    /* Socket Adapter�N���X�C���X�^���X */
    std::unique_ptr<SocketAdapter> m_Adapter;
};

