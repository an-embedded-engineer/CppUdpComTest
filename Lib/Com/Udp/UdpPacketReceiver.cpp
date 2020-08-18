#include "UdpPacketReceiver.h"
#include "UdpSocketReceiver.h"
#include "UdpPacketEncoder.h"

/* �V���O���g���C���X�^���X�擾 */
UdpPacketReceiver& UdpPacketReceiver::GetInstance()
{
    static UdpPacketReceiver instance;
    return instance;
}

/* �R���X�g���N�^ */
UdpPacketReceiver::UdpPacketReceiver()
    : m_Encoder(UdpPacketEncoder::GetInstance())
    , m_RxBufferMap(UdpPacketRxBufferMap::GetInstance())
{
    /* Nothing to do */
}

/* �f�X�g���N�^ */
UdpPacketReceiver::~UdpPacketReceiver()
{
    /* Nothing to do */
}

/* �f�[�^������M */
void UdpPacketReceiver::ReceiveSync(UdpSocketReceiver& udp_socket_receiver)
{
    /* UDP Packet��M�o�b�t�@�𐶐� */
    UdpPacketBuffer udp_packet_buffer;

    /* UDP Packet��M�o�b�t�@�|�C���^���擾 */
    byte_ptr buffer_ptr = udp_packet_buffer.Binary;

    /* ��M�T�C�Y�������� */
    size_t rx_size = 0;

    /* UDP Packet�𓯊���M */
    udp_socket_receiver.ReceiveSync(buffer_ptr, UDP_PACKET_SIZE, rx_size);

    /* UDP Packet�𐶐� */
    UdpPacket udp_packet;

    /* ��MUDP Packet���f�R�[�h */
    this->m_Encoder.Decode(udp_packet_buffer.Packet, rx_size, udp_packet);

    /* ��MUDP Packet����M�o�b�t�@�ɒǉ� */
    this->m_RxBufferMap.Add(udp_packet);
}

/* ��M�R�[���o�b�N��o�^ */
void UdpPacketReceiver::RegisterCallback(UdpPacketRxBufferMap::CallbackType& callback)
{
    this->m_RxBufferMap.RegisterCallback(callback);
}

/* ���b�Z�[�WID���Ƃ̎�M�R�[���o�b�N��o�^ */
void UdpPacketReceiver::RegisterCallback(uint16_t message_id, UdpPacketRxBuffer::CallbackType& callback)
{
    this->m_RxBufferMap.RegisterCallback(message_id, callback);
}
