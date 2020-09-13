#pragma once
#include "SocketDataTypes.h"

struct UdpMessage
{
public:
    UdpMessage();

    UdpMessage(const uint16_t message_id, const byte_ptr data_ptr, const size_t data_size);

    ~UdpMessage();

    UdpMessage(const UdpMessage&) = delete;

    UdpMessage& operator=(const UdpMessage&) = delete;

    UdpMessage(UdpMessage&& obj) noexcept;

    UdpMessage& operator=(UdpMessage&& obj) noexcept;

    const uint16_t GetMessageID() const;

    const byte_ptr GetDataPtr() const;

    const size_t GetDataSize() const;

private:
    uint16_t message_id;
    byte_ptr data_ptr;
    size_t data_size;
};

