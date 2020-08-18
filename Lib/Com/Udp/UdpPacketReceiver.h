#pragma once
#include "SocketDataTypes.h"
#include "UdpPacketTypes.h"
#include "UdpPacketRxBufferMap.h"

class UdpSocketReceiver;
class UdpPacketEncoder;

/* UDP Packet Sender�N���X�錾 */
class UdpPacketReceiver final
{
public:
    /* �V���O���g���C���X�^���X�擾 */
    static UdpPacketReceiver& GetInstance();

private:
    /* �R���X�g���N�^ */
    UdpPacketReceiver();
    /* �f�X�g���N�^ */
    ~UdpPacketReceiver();

public:
    /* �R�s�[�R���X�g���N�^�폜 */
    UdpPacketReceiver(const UdpPacketReceiver&) = delete;
    /* ���[�u�R���X�g���N�^�폜 */
    UdpPacketReceiver(UdpPacketReceiver&&) = delete;
    /* �R�s�[������Z�q�폜 */
    UdpPacketReceiver& operator=(const UdpPacketReceiver&) = delete;
    /* ���[�u������Z�q�폜 */
    UdpPacketReceiver& operator=(UdpPacketReceiver&&) = delete;

public:
    /* �f�[�^������M */
    void ReceiveSync(UdpSocketReceiver& udp_socket_receiver);

    /* ��M�R�[���o�b�N��o�^ */
    void RegisterCallback(UdpPacketRxBufferMap::CallbackType& callback);

    /* ���b�Z�[�WID���Ƃ̎�M�R�[���o�b�N��o�^ */
    void RegisterCallback(uint16_t message_id, UdpPacketRxBuffer::CallbackType& callback);

private:
    /* UDP Packet Encoder */
    UdpPacketEncoder& m_Encoder;

    /* UDP Packet Rx Buffer Map */
    UdpPacketRxBufferMap& m_RxBufferMap;
};
