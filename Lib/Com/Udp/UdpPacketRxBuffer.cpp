#include "UdpPacketRxBuffer.h"

#include <stdexcept>
#include <iostream>

/* �R���X�g���N�^ */
UdpPacketRxBuffer::UdpPacketRxBuffer()
    : m_MessageID(0)
    , m_Buffer(nullptr)
    , m_TotalSize(0)
    , m_TotalBlockNum(0)
    , m_ReceivedBlockNum(0)
    , m_RecvStatus()
    , m_IsCompleted(false)
    , m_Callback(nullptr)
{
    /* Nothing to do */
}

/* �f�X�g���N�^ */
UdpPacketRxBuffer::~UdpPacketRxBuffer()
{
    /* Nothing to do */
}

/* ��M�R�[���o�b�N�̓o�^ */
void UdpPacketRxBuffer::RegisterCallback(CallbackType& callback)
{
    /* ��M�R�[���o�b�N���o�^ */
    if (this->m_Callback == nullptr)
    {
        /* ��M�R�[���o�b�N��o�^ */
        this->m_Callback = callback;
    }
    else
    {
        throw std::logic_error("Callback is already registered");
    }
}

/* �R�[���o�b�N�Ăяo���̃��N�G�X�g */
void UdpPacketRxBuffer::RequestCallback()
{
    /* �R�[���o�b�N�o�^�m�F */
    if (this->m_Callback != nullptr)
    {
        /* ��M�����m�F */
        if (this->IsCompleted() == true)
        {
            /* ��M�f�[�^&�T�C�Y�m�F */
            if (this->m_Buffer != nullptr && this->m_TotalSize > 0)
            {
                /* ��M�R�[���o�b�N�Ăяo�� */
                this->m_Callback(this->m_Buffer, this->m_TotalSize);
            }
            else
            {
                throw std::logic_error("Rx Buffer & Size are invalid");
            }
        }
        else
        {
            throw std::logic_error("Rx Buffer is not completed");
        }
    }
    else
    {
        std::cerr << "[WARN] Callback is not registered" << std::endl;
    }
}

/* UDP Packet�̒ǉ� */
void UdpPacketRxBuffer::Add(const UdpPacket& udp_packet)
{
    /* 1�Ԗڂ̃p�P�b�g */
    if (udp_packet.Header.CurrentBlockNum == 1)
    {
        /* 1�Ԗڂ̃p�P�b�g��ǉ� */
        this->AddFirstPacket(udp_packet);
    }
    else
    {
        /* 2�Ԗڈȍ~�̃p�P�b�g��ǉ� */
        this->AddContinuousPacket(udp_packet);
    }

    /* ��M�����m�F */
    if (this->CheckCompleted() == true)
    {
        /* ��M������ԍX�V */
        this->m_IsCompleted = true;
    }
}

/* ��M�o�b�t�@�擾 */
void UdpPacketRxBuffer::GetBuffer(byte_ptr& data_ptr, size_t& data_size)
{
    /* ��M�o�b�t�@�Z�b�g */
    data_ptr = this->m_Buffer;

    /* ��M�f�[�^�T�C�Y�Z�b�g */
    data_size = this->m_TotalSize;
}

/* ��M�o�b�t�@�̉�� */
void UdpPacketRxBuffer::Release()
{
    /* ���b�Z�[�WID�N���A */
    this->m_MessageID = 0;

    /* ���v�f�[�^�T�C�Y�N���A */
    this->m_TotalSize = 0;

    /* ���v�u���b�N���N���A */
    this->m_TotalBlockNum = 0;

    /* ��M�u���b�N���N���A */
    this->m_ReceivedBlockNum = 0;

    /* ��M�X�e�[�^�X�N���A */
    this->m_RecvStatus.clear();

    /* ��M�o�b�t�@��� */
    free(this->m_Buffer);

    /* ��M�o�b�t�@�A�h���X�N���A */
    this->m_Buffer = nullptr;

    /* ��M������ԃN���A */
    this->m_IsCompleted = false;
}

/* ��M������Ԏ擾 */
bool UdpPacketRxBuffer::IsCompleted()
{
    return this->m_IsCompleted;
}

/* 1�Ԗڂ̃p�P�b�g��ǉ� */
void UdpPacketRxBuffer::AddFirstPacket(const UdpPacket& udp_packet)
{
    /* UDP Packet Header���擾 */
    const UdpPacketHeader& header = udp_packet.Header;

    /* ���݂̃u���b�N�C���f�b�N�X���Z�o */
    size_t current_block_index = header.CurrentBlockNum - 1;

    /* ��M������Ԃ��N���A */
    this->m_IsCompleted = false;

    /* ���b�Z�[�WID���Z�b�g */
    this->m_MessageID = header.MessageID;

    /* ���v�f�[�^�T�C�Y���Z�b�g */
    this->m_TotalSize = header.TotalSize;

    /* ���v�u���b�N�����Z�b�g */
    this->m_TotalBlockNum = header.TotalBlockNum;

    /* ��M�u���b�N�����Z�b�g */
    this->m_ReceivedBlockNum = 1;

    /* ��M�X�e�[�^�X�������� */
    this->m_RecvStatus.resize(this->m_TotalBlockNum);

    /* ��M�u���b�N�̎�M�X�e�[�^�X���Z�b�g */
    this->m_RecvStatus[current_block_index] = true;

    /* ��M�o�b�t�@�̃������m�� */
    this->m_Buffer = (byte_ptr)malloc(this->m_TotalSize);

    /* ��M�o�b�t�@�m�ې��� */
    if (this->m_Buffer != nullptr)
    {
        /* �J�n�C���f�b�N�X�Z�b�g */
        size_t start_index = 0;

        /* �u���b�N�T�C�Y�Z�o */
        size_t block_size = header.PacketSize - UDP_PACKET_HEADER_SIZE;

        /* UDP Packet Data����M�o�b�t�@�ɃR�s�[ */
        memcpy(&this->m_Buffer[start_index], udp_packet.Data, block_size);
    }
    else
    {
        throw std::bad_alloc();
    }
}

/* 2�Ԗڈȍ~�̃p�P�b�g��ǉ� */
void UdpPacketRxBuffer::AddContinuousPacket(const UdpPacket& udp_packet)
{
    /* UDP Packet Header���擾 */
    const UdpPacketHeader& header = udp_packet.Header;

    /* ���݂̃u���b�N�C���f�b�N�X���Z�o */
    size_t current_block_index = header.CurrentBlockNum - 1;

    /* ���b�Z�[�WID�m�F */
    if (this->m_MessageID == header.MessageID)
    {
        /* ���v�f�[�^�T�C�Y�m�F */
        if (this->m_TotalSize == header.TotalSize)
        {
            /* ���v�u���b�N���m�F */
            if (this->m_TotalBlockNum == header.TotalBlockNum)
            {
                /* ��M�X�e�[�^�X�m�F */
                if (this->m_RecvStatus.at(current_block_index) == false)
                {
                    /* ��M�u���b�N���C���N�������g */
                    this->m_ReceivedBlockNum++;

                    /* ��M�u���b�N�̎�M�X�e�[�^�X���Z�b�g */
                    this->m_RecvStatus[current_block_index] = true;

                    /* �J�n�C���f�b�N�X���Z�o */
                    size_t start_index = (size_t)current_block_index * (size_t)UDP_PACKET_DATA_SIZE;

                    /* �u���b�N�T�C�Y�Z�o */
                    size_t block_size = header.PacketSize - UDP_PACKET_HEADER_SIZE;

                    /* UDP Packet Data����M�o�b�t�@�ɃR�s�[ */
                    memcpy(&this->m_Buffer[start_index], udp_packet.Data, block_size);
                }
                else
                {
                    throw std::logic_error("Current Block is already received");
                }
            }
            else
            {
                throw std::logic_error("Different Total Block Num");
            }
        }
        else
        {
            throw std::logic_error("Different Total Data Size");
        }
    }
    else
    {
        throw std::logic_error("Different Message ID");
    }
}

/* ��M�����m�F */
bool UdpPacketRxBuffer::CheckCompleted()
{
    /* ��M�u���b�N���m�F */
    if (this->m_ReceivedBlockNum == this->m_TotalBlockNum)
    {
        /* ��M�X�e�[�^�X���m�F */
        for (const auto& flag : this->m_RecvStatus)
        {
            /* ����M�u���b�N������ꍇ�͎�M������ */
            if (flag == false)
            {
                return false;
            }
        }

        /* ��M���� */
        return true;
    }
    else
    {
        /* ��M������ */
        return false;
    }
}
