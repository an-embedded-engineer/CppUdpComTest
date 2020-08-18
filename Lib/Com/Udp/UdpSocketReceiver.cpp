#include "UdpSocketReceiver.h"

/* �R���X�g���N�^ */
UdpSocketReceiver::UdpSocketReceiver()
    : m_Adapter(nullptr)
{
    /* Socket Adapter�N���X�C���X�^���X���� */
    this->m_Adapter = std::make_unique<SocketAdapter>();
}

/* �f�X�g���N�^ */
UdpSocketReceiver::~UdpSocketReceiver()
{
    /* Nothing to do */
}


/* ���j�L���X�g�p�\�P�b�g�I�[�v�� */
void UdpSocketReceiver::OpenUniSocket(const uint16_t local_port)
{
    /* UDP���j�L���X�g��M�p�\�P�b�g�I�[�v�� */
    this->m_Adapter->OpenUdpUniRxSocket(local_port);
}

/* �}���`�L���X�g�p�\�P�b�g�I�[�v�� */
void UdpSocketReceiver::OpenMultiSocket(const std::string& multicast_ip, const uint16_t multicast_port)
{
    /* UDP�}���`�L���X�g��M�p�\�P�b�g�I�[�v�� */
    this->m_Adapter->OpenUdpMultiRxSocket(multicast_ip, multicast_port);
}

/* �u���[�h�L���X�g�p�\�P�b�g�I�[�v�� */
void UdpSocketReceiver::OpenBroadSocket(const uint16_t local_port)
{
    /* UDP�u���[�h�L���X�g��M�p�\�P�b�g�I�[�v�� */
    this->m_Adapter->OpenUdpBroadRxSocket(local_port);
}

/* �\�P�b�g�N���[�Y */
void UdpSocketReceiver::CloseSocket()
{
    /* �\�P�b�g�N���[�Y */
    this->m_Adapter->CloseSocket();
}

/* �\�P�b�g�I�[�v���m�F */
bool UdpSocketReceiver::IsSocketOpened()
{
    /* �\�P�b�g�I�[�v���m�F */
    return this->m_Adapter->IsSocketOpened();
}

/* �p�P�b�g������M */
void UdpSocketReceiver::ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
{
    /* �p�P�b�g������M */
    this->m_Adapter->ReceiveSync(buffer_ptr, buffer_size, rx_size);
}
