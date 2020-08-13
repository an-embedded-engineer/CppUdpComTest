#pragma once
#include <memory>
#include <cstdint>
#include <string>

using any_ptr = void*;
using byte = uint8_t;
using byte_ptr = uint8_t*;

enum class SocketResult
{
    Success,
    Fail,
    Unknown,
};

class SocketAdapterImpl;

class SocketAdapter final
{
public:
    static void Initialize();
    static void Finalize();
    static int GetErrorCode();

    SocketAdapter();
    ~SocketAdapter();

    SocketResult OpenUdpTxSocket(const std::string& remote_ip, const uint16_t remote_port);
    SocketResult OpenUdpRxSocket(const uint16_t local_port);
    SocketResult CloseUdpSocket();

    bool IsSocketOpened();

    SocketResult Transmit(const any_ptr data_ptr, size_t tx_size);
    SocketResult ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size);
    
private:
    std::unique_ptr<SocketAdapterImpl> m_Impl;
};

