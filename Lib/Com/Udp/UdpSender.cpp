#include "UdpSender.h"

/* �R���X�g���N�^ */
UdpSender::UdpSender()
    : m_SocketSender(nullptr)
    , m_PacketSender(UdpPacketSender::GetInstance())
{
    /* UDP Socket Sender�N���X�C���X�^���X���� */
    this->m_SocketSender = std::make_unique<UdpSocketSender>();
}

/* �f�X�g���N�^ */
UdpSender::~UdpSender()
{
    /* Nothing to do */
}

/* ���j�L���X�g�p�\�P�b�g�I�[�v�� */
void UdpSender::OpenUniSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    /* ���j�L���X�g�p�\�P�b�g�I�[�v�� */
    this->m_SocketSender->OpenUniSocket(remote_ip, remote_port);
}

/* �}���`�L���X�g�p�\�P�b�g�I�[�v�� */
void UdpSender::OpenMultiSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl)
{
    /* �}���`�L���X�g�p�\�P�b�g�I�[�v�� */
    this->m_SocketSender->OpenMultiSocket(multicast_ip, local_ip, multicast_port, ttl);
}

/* �u���[�h�L���X�g�p�\�P�b�g�I�[�v�� */
void UdpSender::OpenBroadSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    /* �u���[�h�L���X�g�p�\�P�b�g�I�[�v�� */
    this->m_SocketSender->OpenBroadSocket(remote_ip, remote_port);
}

/* �\�P�b�g�N���[�Y */
void UdpSender::CloseSocket()
{
    /* �\�P�b�g�N���[�Y */
    this->m_SocketSender->CloseSocket();
}

/* �\�P�b�g�I�[�v���m�F */
bool UdpSender::IsSocketOpened()
{
    /* �\�P�b�g�I�[�v���m�F */
    return this->m_SocketSender->IsSocketOpened();
}

/* �f�[�^���M */
void UdpSender::Transmit(const uint16_t message_id, const byte_ptr data_ptr, const size_t tx_size)
{
    /* �f�[�^��UDP�p�P�b�g�ɕ������đ��M */
    this->m_PacketSender.Transmit(*this->m_SocketSender, message_id, data_ptr, tx_size);
}
