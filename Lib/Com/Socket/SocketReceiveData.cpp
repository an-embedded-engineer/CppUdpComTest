#include "SocketReceiveData.h"

SocketReceiveData::SocketReceiveData()
    : m_DataPtr(nullptr)
    , m_DataSize(0)
{

}

SocketReceiveData::SocketReceiveData(const byte_ptr data_ptr, const size_t data_size)
    : m_DataPtr(data_ptr)
    , m_DataSize(data_size)
{

}

SocketReceiveData::~SocketReceiveData()
{
    if (this->m_DataPtr != nullptr)
    {
        free(this->m_DataPtr);
    }
    this->m_DataSize = 0;
}

SocketReceiveData::SocketReceiveData(SocketReceiveData&& obj) noexcept
    : m_DataPtr(obj.m_DataPtr)
    , m_DataSize(obj.m_DataSize)
{
    obj.m_DataPtr = nullptr;
    obj.m_DataSize = 0;
}

SocketReceiveData& SocketReceiveData::operator=(SocketReceiveData&& obj) noexcept
{
    if (this->m_DataPtr != nullptr)
    {
        free(this->m_DataPtr);
        this->m_DataPtr = nullptr;
    }

    this->m_DataPtr = obj.m_DataPtr;
    this->m_DataSize = obj.m_DataSize;

    obj.m_DataPtr = nullptr;
    obj.m_DataSize = 0;

    return *this;
}

const byte_ptr SocketReceiveData::GetDataPtr() const
{
    return this->m_DataPtr;
}

const size_t SocketReceiveData::GetDataSize() const
{
    return this->m_DataSize;
}



