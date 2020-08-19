#include "UdpPacketRxBufferMap.h"
#include "AppException.h"

#include <iostream>

/* �V���O���g���C���X�^���X�擾 */
UdpPacketRxBufferMap& UdpPacketRxBufferMap::GetInstance()
{
    static UdpPacketRxBufferMap instance;
    return instance;
}

/* �R���X�g���N�^ */
UdpPacketRxBufferMap::UdpPacketRxBufferMap()
    : m_Map()
    , m_Callback(nullptr)
{
    /* Nothing to do */
}

/* �f�X�g���N�^ */
UdpPacketRxBufferMap::~UdpPacketRxBufferMap()
{
    /* Nothing to do */
}

/* ��M�R�[���o�b�N��o�^ */
void UdpPacketRxBufferMap::RegisterCallback(CallbackType& callback)
{
    /* ��M�R�[���o�b�N���o�^ */
    if (this->m_Callback == nullptr)
    {
        /* ��M�R�[���o�b�N��o�^ */
        this->m_Callback = callback;
    }
    else
    {
        THROW_APP_EXCEPTION("Callback is already registered");
    }
}

/* ���b�Z�[�WID���Ƃ̎�M�R�[���o�b�N��o�^ */
void UdpPacketRxBufferMap::RegisterCallback(uint16_t message_id, UdpPacketRxBuffer::CallbackType& callback)
{
    /* UDP Packet��M�o�b�t�@���o�^ */
    if (this->m_Map.count(message_id) == 0)
    {
        /* UDP Packet��M�o�b�t�@�o�^ */
        this->m_Map.emplace(std::piecewise_construct, std::forward_as_tuple(message_id), std::forward_as_tuple());
    }

    /* ��M�R�[���o�b�N�o�^ */
    this->m_Map[message_id].RegisterCallback(callback);
}

/* UDP Packet�̒ǉ� */
void UdpPacketRxBufferMap::Add(const UdpPacket& udp_packet)
{
    /* ���b�Z�[�WID�擾 */
    uint16_t message_id = udp_packet.Header.MessageID;

    /* UDP Packet��M�o�b�t�@���o�^ */
    if (this->m_Map.count(message_id) == 0)
    {
        /* UDP Packet��M�o�b�t�@�o�^ */
        this->m_Map.emplace(std::piecewise_construct, std::forward_as_tuple(message_id), std::forward_as_tuple());
    }

    /* UDP Packet��M�o�b�t�@�擾 */
    auto& item = this->m_Map[message_id];

    /* ��M�o�b�t�@��UDP Packet�ǉ� */
    item.Add(udp_packet);

    /* ��M�����m�F */
    if (item.IsCompleted())
    {
        /* ��M�R�[���o�b�N�Ăяo�������N�G�X�g */
        item.RequestCallback();

        /* ��M�f�[�^�o�b�t�@�������� */
        byte_ptr data_ptr = nullptr;

        /* ��M�f�[�^�T�C�Y�������� */
        size_t data_size = 0;

        /* ��M�f�[�^�擾 */
        item.GetBuffer(data_ptr, data_size);

        /* ���b�Z�[�WID���Ƃ̎�M�R�[���o�b�N�Ăяo�������N�G�X�g */
        this->RequestCallback(message_id, data_ptr, data_size);

        /* ��M�o�b�t�@����� */
        item.Release();
    }
}

/* �R�[���o�b�N�Ăяo���̃��N�G�X�g */
void UdpPacketRxBufferMap::RequestCallback(uint16_t message_id, const byte_ptr data_ptr, const size_t data_size)
{
    /* �R�[���o�b�N�o�^�m�F */
    if (this->m_Callback != nullptr)
    {
        /* ��M�f�[�^&�T�C�Y�m�F */
        if (data_ptr != nullptr && data_size > 0)
        {
            /* ��M�R�[���o�b�N�Ăяo�������N�G�X�g */
            this->m_Callback(message_id, data_ptr, data_size);
        }
        else
        {
            THROW_APP_EXCEPTION("Rx Buffer & Size are invalid");
        }
    }
    else
    {
        std::cerr << "[WARN] Callback is not registered" << std::endl;
    }
}
