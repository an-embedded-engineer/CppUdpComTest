#pragma once
#include "UdpPacketRxBuffer.h"

#include <map>

/* UDP Packet Rx Buffer Map�N���X�錾 */
class UdpPacketRxBufferMap
{
public:
    /* ��M�R�[���o�b�N */
    using CallbackType = std::function<void(const uint16_t, const byte_ptr, const size_t)>;

public:
    /* �V���O���g���C���X�^���X�擾 */
    static UdpPacketRxBufferMap& GetInstance();

private:
    /* �R���X�g���N�^ */
    UdpPacketRxBufferMap();
    /* �f�X�g���N�^ */
    ~UdpPacketRxBufferMap();

public:
    /* �R�s�[�R���X�g���N�^�폜 */
    UdpPacketRxBufferMap(const UdpPacketRxBufferMap&) = delete;
    /* ���[�u�R���X�g���N�^�폜 */
    UdpPacketRxBufferMap(UdpPacketRxBufferMap&&) = delete;
    /* �R�s�[������Z�q�폜 */
    UdpPacketRxBufferMap& operator=(const UdpPacketRxBufferMap&) = delete;
    /* ���[�u������Z�q�폜 */
    UdpPacketRxBufferMap& operator=(UdpPacketRxBufferMap&&) = delete;

public:
    /* ��M�R�[���o�b�N��o�^ */
    void RegisterCallback(CallbackType& callback);

    /* ���b�Z�[�WID���Ƃ̎�M�R�[���o�b�N��o�^ */
    void RegisterCallback(uint16_t message_id, UdpPacketRxBuffer::CallbackType& callback);

    /* UDP Packet�̒ǉ� */
    void Add(const UdpPacket& udp_packet);

private:
    /* �R�[���o�b�N�Ăяo���̃��N�G�X�g */
    void RequestCallback(uint16_t message_id, const byte_ptr data_ptr, const size_t data_size);

private:
    /* UDP Packet��M�o�b�t�@�}�b�v */
    std::map<uint16_t, UdpPacketRxBuffer> m_Map;

    /* ��M�R�[���o�b�N */
    CallbackType m_Callback;
};
