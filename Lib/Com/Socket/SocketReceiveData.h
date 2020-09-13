#pragma once
#include "SocketDataTypes.h"

class SocketReceiveData
{
public:
    SocketReceiveData();
    SocketReceiveData(const byte_ptr data_ptr, const size_t data_size);
    ~SocketReceiveData();

    SocketReceiveData(const SocketReceiveData&) = delete;
    SocketReceiveData& operator=(const SocketReceiveData&) = delete;

    SocketReceiveData(SocketReceiveData&& obj) noexcept;
    SocketReceiveData& operator=(SocketReceiveData&& obj) noexcept;

    const byte_ptr GetDataPtr() const;
    const size_t GetDataSize() const;

private:
    byte_ptr m_DataPtr;
    size_t m_DataSize;
};
