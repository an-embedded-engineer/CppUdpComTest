#pragma once
#include "CompileSwitch.h"
#include "SocketDataTypes.h"

#include <cstdint>
#include <string>

#if COM_TYPE == COM_WINSOCK
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using SocketType = SOCKET;
using SocketAddressType = struct sockaddr_in;
using IpAddressType = ULONG;
using AddressFamilyType = ADDRESS_FAMILY;
using MulticastRequestType = ip_mreq;
using TTLType = int;

#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
#if SOCKET_INTERFACE_STUB_ENABLED == 0
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#else

using __uint8_t = uint8_t;
using sa_family_t = unsigned char;
using in_port_t = unsigned short;
using in_addr_t = unsigned int;
using socklen_t = unsigned int;
using ssize_t = long;

struct in_addr
{
    in_addr_t s_addr;
};

struct sockaddr
{
    __uint8_t sin_len;
    sa_family_t sin_family;
    char sa_data[14];
};

struct sockaddr_in
{
    __uint8_t sin_len;
    sa_family_t sin_family;
    in_port_t sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
};

struct ip_mreq
{
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
};

inline int socket(int, int, int) { return 0; }
inline in_port_t htons(uint16_t) { return 0; }
inline in_addr_t inet_addr(const char*) { return 0; }
inline int bind(int, const struct sockaddr*, socklen_t) { return 0; }
inline int ioctl(int, unsigned long, ...) { return 0; }
inline int setsockopt(int, int, int, const void*, socklen_t) { return 0; }
inline ssize_t sendto(int, const void*, size_t, int, const struct sockaddr*, socklen_t) { return 0; }
inline ssize_t recv(int, void*, size_t, int) { return 0; }
inline int close(int) { return 0; }

#define AF_INET 0
#define SOCK_DGRAM 0
#define INADDR_ANY 0
#define FIONBIO 0
#define IPPROTO_IP 0
#define IP_MULTICAST_IF 0
#define IP_MULTICAST_TTL 0
#define IP_ADD_MEMBERSHIP 0
#define SOL_SOCKET 0
#define SO_BROADCAST 0
#endif

using SocketType = int;
using SocketAddressType = struct sockaddr_in;
using IpAddressType = in_addr_t;
using AddressFamilyType = sa_family_t;
using MulticastRequestType = ip_mreq;
using TTLType = int;

#else
using SocketType = int;
using SocketAddressType = int;
using IpAddressType = int;
using AddressFamilyType = int;
using MulticastRequestType = int;
using TTLType = int;
#endif


class SocketInterface
{
public:
    static void Initialize();

    static void Finalize();

    static int GetErrorCode();

    static SocketType OpenUdpSocket();

    static void CloseSocket(SocketType socket);

    static SocketAddressType GetTxSocketAddress(const std::string& dst_ip, const uint16_t dst_port);

    static SocketAddressType GetRxSocketAddress(const uint16_t src_port);

    static void BindSocket(SocketType& socket, const SocketAddressType& socket_address);

    static void SetMulticastTxSocketOption(SocketType& socket, const SocketAddressType& socket_address, const std::string& src_ip);

    static void SetMulticastRxSocketOption(SocketType& socket, const SocketAddressType& socket_address, const std::string& dst_ip);

    static void SetBroadcastTxSocketOption(SocketType& socket);

    static void SetTTLSocketOption(SocketType& socket, const int32_t ttl);

    static void SetNonBlockingMode(SocketType& socket);

    static void Transmit(SocketType& socket, const SocketAddressType& socket_address, const any_ptr data_ptr, const size_t tx_size);

    static void ReceiveSync(SocketType& socket, byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size);

    static bool ReceiveAsync(SocketType& socket, byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size);

#if COM_TYPE == COM_WINSOCK
private:
    /* IPアドレスの変換(文字列 -> 数値) */
    static IpAddressType ConvertIpStrToNum(const AddressFamilyType family, const std::string& ip);
#endif
};
