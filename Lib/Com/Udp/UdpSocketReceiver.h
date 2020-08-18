#pragma once
#include "SocketAdapter.h"

#include <memory>

/* UDP Receiver�N���X�錾 */
class UdpSocketReceiver
{
public:
    /* �R���X�g���N�^ */
    UdpSocketReceiver();
    /* �f�X�g���N�^ */
    ~UdpSocketReceiver();

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

    /* �p�P�b�g������M */
    void ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size);

private:
    /* Socket Adapter�N���X�C���X�^���X */
    std::unique_ptr<SocketAdapter> m_Adapter;
};

