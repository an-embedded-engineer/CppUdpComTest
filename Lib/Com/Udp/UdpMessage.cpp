#include "UdpMessage.h"

#include <memory>

UdpMessage::UdpMessage()
    : message_id(0)
    , data_ptr(nullptr)
    , data_size(0)
{

}

UdpMessage::UdpMessage(const uint16_t message_id, const byte_ptr data_ptr, const size_t data_size)
    : message_id(message_id)
    , data_ptr(data_ptr)
    , data_size(data_size)
{
}

UdpMessage::~UdpMessage()
{
    this->message_id = 0;
    if (this->data_ptr != nullptr)
    {
        free(this->data_ptr);
        this->data_ptr = nullptr;
    }
    this->data_size = 0;
}

UdpMessage::UdpMessage(UdpMessage&& obj) noexcept
    : message_id(obj.message_id)
    , data_ptr(obj.data_ptr)
    , data_size(obj.data_size)
{
    obj.message_id = 0;
    obj.data_ptr = nullptr;
    obj.data_size = 0;
}

UdpMessage& UdpMessage::operator=(UdpMessage&& obj) noexcept
{
    if (this->data_ptr != nullptr)
    {
        free(this->data_ptr);
    }
    this->data_ptr = obj.data_ptr;
    this->message_id = obj.message_id;
    this->data_size = obj.data_size;

    obj.message_id = 0;
    obj.data_ptr = nullptr;
    obj.data_size = 0;

    return *this;
}

const uint16_t UdpMessage::GetMessageID() const
{ 
    return this->message_id;
}

const byte_ptr UdpMessage::GetDataPtr() const
{ 
    return this->data_ptr;
}

const size_t UdpMessage::GetDataSize() const
{ 
    return this->data_size;
}
