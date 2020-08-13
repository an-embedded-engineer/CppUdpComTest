#pragma once
#include "SocketAdapter.h"
#include "CompileSwitch.h"

#include <stdexcept>

class SocketAdapterImpl final
{
public:
    static void Initialize()
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    static void Finalize()
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    static int GetErrorCode()
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    SocketAdapterImpl()
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    ~SocketAdapterImpl()
    {
    }

    SocketResult OpenUdpTxSocket(const std::string& remote_ip, const uint16_t remote_port)
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    SocketResult OpenUdpRxSocket(const uint16_t local_port)
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    SocketResult CloseUdpSocket()
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    bool IsSocketOpened()
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    SocketResult Transmit(const any_ptr data_ptr, size_t tx_size)
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }

    SocketResult ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
    {
        throw std::runtime_error("Invalid Target Type : " + std::to_string(TARGET_TYPE));
    }
};

