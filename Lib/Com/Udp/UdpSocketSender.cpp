#include "UdpSocketSender.h"

/* �R���X�g���N�^ */
UdpSocketSender::UdpSocketSender()
    : m_Adapter(nullptr)
{
    /* Socket Adapter�N���X�C���X�^���X���� */
    this->m_Adapter = std::make_unique<SocketAdapter>();
}

/* �f�X�g���N�^ */
UdpSocketSender::~UdpSocketSender()
{
    /* Nothing to do */
}

/* ���j�L���X�g�p�\�P�b�g�I�[�v�� */
void UdpSocketSender::OpenUniSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    /* UDP���j�L���X�g���M�p�\�P�b�g�I�[�v�� */
    this->m_Adapter->OpenUdpUniTxSocket(remote_ip, remote_port);
}

/* �}���`�L���X�g�p�\�P�b�g�I�[�v�� */
void UdpSocketSender::OpenMultiSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl)
{
    /* UDP�}���`�L���X�g���M�p�\�P�b�g�I�[�v�� */
    this->m_Adapter->OpenUdpMultiTxSocket(multicast_ip, local_ip, multicast_port, ttl);
}

/* �u���[�h�L���X�g�p�\�P�b�g�I�[�v�� */
void UdpSocketSender::OpenBroadSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    /* UDP�u���[�h�L���X�g���M�p�\�P�b�g�I�[�v�� */
    this->m_Adapter->OpenUdpBroadTxSocket(remote_ip, remote_port);
}

/* �\�P�b�g�N���[�Y */
void UdpSocketSender::CloseSocket()
{
    /* �\�P�b�g�N���[�Y */
    this->m_Adapter->CloseSocket();
}

/* �\�P�b�g�I�[�v���m�F */
bool UdpSocketSender::IsSocketOpened()
{
    /* �\�P�b�g�I�[�v���m�F */
    return this->m_Adapter->IsSocketOpened();
}

/* �p�P�b�g���M */
void UdpSocketSender::Transmit(const any_ptr data_ptr, const size_t tx_size)
{
    this->m_Adapter->Transmit(data_ptr, tx_size);
}

