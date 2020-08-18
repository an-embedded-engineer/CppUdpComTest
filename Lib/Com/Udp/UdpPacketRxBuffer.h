#pragma once
#include "UdpPacketTypes.h"
#include "SocketDataTypes.h"

#include <functional>
#include <vector>

/* UDP Packet Rx Buffer�N���X�錾 */
class UdpPacketRxBuffer
{
public:
    /* �R�[���o�b�N */
    using CallbackType = std::function<void(const byte_ptr, const size_t)>;

public:
    /* �R���X�g���N�^ */
    UdpPacketRxBuffer();
    /* �f�X�g���N�^ */
    ~UdpPacketRxBuffer();

    /* ��M�R�[���o�b�N�̓o�^ */
    void RegisterCallback(CallbackType& callback);

    /* �R�[���o�b�N�Ăяo���̃��N�G�X�g */
    void RequestCallback();

    /* UDP Packet�̒ǉ� */
    void Add(const UdpPacket& udp_packet);

    /* ��M�o�b�t�@�擾 */
    void GetBuffer(byte_ptr& data_ptr, size_t& data_size);

    /* ��M�o�b�t�@�̉�� */
    void Release();

    /* ��M������Ԏ擾 */
    bool IsCompleted();

private:
    /* 1�Ԗڂ̃p�P�b�g��ǉ� */
    void AddFirstPacket(const UdpPacket& udp_packet);

    /* 2�Ԗڈȍ~�̃p�P�b�g��ǉ� */
    void AddContinuousPacket(const UdpPacket& udp_packet);

    /* ��M�����m�F */
    bool CheckCompleted();

private:
    /* ���b�Z�[�WID */
    uint16_t m_MessageID;
    /* ��M�o�b�t�@ */
    byte_ptr m_Buffer;
    /* ���v�f�[�^�T�C�Y */
    size_t m_TotalSize;
    /* ���v�u���b�N�� */
    size_t m_TotalBlockNum;
    /* ��M�u���b�N�� */
    size_t m_ReceivedBlockNum;
    /* ��M�X�e�[�^�X */
    std::vector<bool> m_RecvStatus;
    /* ��M������� */
    bool m_IsCompleted;
    /* ��M�R�[���o�b�N */
    CallbackType m_Callback;
};

