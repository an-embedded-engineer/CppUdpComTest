#include "UdpReceiver.h"
#include "UdpSocketReceiver.h"
#include "UdpPacketReceiver.h"

#include <functional>
#include <sstream>

/* �R���X�g���N�^ */
UdpReceiver::UdpReceiver()
    : m_SocketReceiver(nullptr)
    , m_PacketReceiver(UdpPacketReceiver::GetInstance())
    , m_MessageID(0)
    , m_Message()
    , m_IsReceived(false)
{
    /* UDP Socket Receiver�N���X�C���X�^���X���� */
    this->m_SocketReceiver = std::make_unique<UdpSocketReceiver>();

    /* ��M�R�[���o�b�N�Z�b�g */
    //UdpPacketRxBufferMap::CallbackType callback = [this](const uint16_t message_id, const byte_ptr data_ptr, const size_t data_size) { this->ReceiveCallback(message_id, data_ptr, data_size); };
    UdpPacketRxBufferMap::CallbackType callback = std::bind(&UdpReceiver::ReceiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    /* ��M�R�[���o�b�N��o�^ */
    this->m_PacketReceiver.RegisterCallback(callback);
}

/* �f�X�g���N�^ */
UdpReceiver::~UdpReceiver()
{
    /* Nothing to do */
}

/* ���j�L���X�g�p�\�P�b�g�I�[�v�� */
void UdpReceiver::OpenUniSocket(const uint16_t local_port)
{
    /* ���j�L���X�g�p�\�P�b�g�I�[�v�� */
    this->m_SocketReceiver->OpenUniSocket(local_port);
}

/* �}���`�L���X�g�p�\�P�b�g�I�[�v�� */
void UdpReceiver::OpenMultiSocket(const std::string& multicast_ip, const uint16_t multicast_port)
{
    /* �}���`�L���X�g�p�\�P�b�g�I�[�v�� */
    this->m_SocketReceiver->OpenMultiSocket(multicast_ip, multicast_port);
}

/* �u���[�h�L���X�g�p�\�P�b�g�I�[�v�� */
void UdpReceiver::OpenBroadSocket(const uint16_t local_port)
{
    /* �u���[�h�L���X�g�p�\�P�b�g�I�[�v�� */
    this->m_SocketReceiver->OpenBroadSocket(local_port);
}

/* �\�P�b�g�N���[�Y */
void UdpReceiver::CloseSocket()
{
    /* �\�P�b�g�N���[�Y */
    this->m_SocketReceiver->CloseSocket();
}

/* �\�P�b�g�I�[�v���m�F */
bool UdpReceiver::IsSocketOpened()
{
    /* �\�P�b�g�I�[�v���m�F */
    return this->m_SocketReceiver->IsSocketOpened();
}

/* �f�[�^������M */
void UdpReceiver::ReceiveSync(uint16_t& message_id, std::string& message)
{
    /* ��M������ԃN���A */
    this->m_IsReceived = false;

    while (true)
    {
        /* �f�[�^������M */
        this->m_PacketReceiver.ReceiveSync(*this->m_SocketReceiver);

        /* ��M���������烋�[�v�𔲂��� */
        if (this->m_IsReceived == true)
        {
            break;
        }
    }

    /* ���b�Z�[�WID�Z�b�g */
    message_id = this->m_MessageID;

    /* ��M���b�Z�[�W�Z�b�g */
    message = this->m_Message;
}

/* �f�[�^��M�R�[���o�b�N */
void UdpReceiver::ReceiveCallback(const uint16_t message_id, const byte_ptr data_ptr, const size_t data_size)
{
    /* ���b�Z�[�WID�Z�b�g */
    this->m_MessageID = message_id;

    /* ��M���b�Z�[�W�Z�b�g */
    std::stringstream ss;

    ss << data_ptr;

    this->m_Message = ss.str();

    /* ��M������ԃZ�b�g */
    this->m_IsReceived = true;
}

