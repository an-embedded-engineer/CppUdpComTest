#include "SocketInterface.h"
#include "SocketException.h"
#include "AppException.h"

#if COM_TYPE == COM_WINSOCK
static WSADATA s_WsaData;
#endif

static int s_ErrorCode;


void SocketInterface::Initialize()
{
#if COM_TYPE == COM_WINSOCK
    /* WinSock�T�[�r�X�X�^�[�g�A�b�v */
    int startup_result = WSAStartup(MAKEWORD(2, 0), &s_WsaData);

    /* WinSock�T�[�r�X�X�X�^�[�g�A�b�v���s���̃G���[���� */
    if (startup_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = startup_result;

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("WSA Startup Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* Nothing to do */
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::Finalize()
{
#if COM_TYPE == COM_WINSOCK
    /* WinSock�T�[�r�X�N���[���A�b�v */
    int cleanup_result = WSACleanup();

    /* WinSock�T�[�r�X�N���[���A�b�v���s���̃G���[���� */
    if (cleanup_result == SOCKET_ERROR)
    {
        s_ErrorCode = WSAGetLastError();

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("WSA Cleanup Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* Nothing to do */
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

int SocketInterface::GetErrorCode()
{
    return s_ErrorCode;
}

SocketType SocketInterface::OpenUdpSocket()
{
#if COM_TYPE == COM_WINSOCK
    /* UDP�p�\�P�b�g���I�[�v�� */
    SocketType sock = socket(AF_INET, SOCK_DGRAM, 0);

    /* �\�P�b�g�I�[�v�����s���̃G���[���� */
    if (sock == INVALID_SOCKET)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = WSAGetLastError();

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("UDP Socket Open Failed", s_ErrorCode);
    }

    return sock;
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* UDP�p�\�P�b�g���I�[�v�� */
    SocketType sock = socket(AF_INET, SOCK_DGRAM, 0);

    /* �\�P�b�g�I�[�v�����s���̃G���[���� */
    if (sock < 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = errno;

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("UDP Socket Open Failed", s_ErrorCode);
    }

    return sock;
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::CloseSocket(SocketType socket)
{
#if COM_TYPE == COM_WINSOCK
    /* �\�P�b�g�N���[�Y */
    int close_result = closesocket(socket);

    /* �\�P�b�g�N���[�Y���s���̃G���[���� */
    if (close_result == SOCKET_ERROR)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = WSAGetLastError();

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Socket Close Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* �\�P�b�g�N���[�Y */
    int close_result = close(socket);

    /* �\�P�b�g�N���[�Y���s���̃G���[���� */
    if (close_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = errno;

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Socket Close Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

SocketAddressType SocketInterface::GetTxSocketAddress(const std::string& dst_ip, const uint16_t dst_port)
{
#if COM_TYPE == COM_WINSOCK
    SocketAddressType sock_address;

    /* ���M�p�A�h���X���Z�b�g */
    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(dst_port);
    sock_address.sin_addr.S_un.S_addr = SocketInterface::ConvertIpStrToNum(sock_address.sin_family, dst_ip);

    return sock_address;
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    SocketAddressType sock_address;

    /* ���M�p�A�h���X���Z�b�g */
    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(dst_port);
    sock_address.sin_addr.s_addr = inet_addr(dst_ip.c_str());
#if COM_TYPE == COM_MACSOCK
    sock_address.sin_len = sizeof(sock_address);
#endif

    return sock_address;
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

SocketAddressType SocketInterface::GetRxSocketAddress(const uint16_t src_port)
{
#if COM_TYPE == COM_WINSOCK
    SocketAddressType sock_address;

    /* ��M�p�A�h���X���Z�b�g */
    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(src_port);
    sock_address.sin_addr.S_un.S_addr = INADDR_ANY;

    return sock_address;
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    SocketAddressType sock_address;

    /* ��M�p�A�h���X���Z�b�g */
    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(src_port);
    sock_address.sin_addr.s_addr = INADDR_ANY;
#if COM_TYPE == COM_MACSOCK
    sock_address.sin_len = sizeof(sock_address);
#endif

    return sock_address;
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::BindSocket(SocketType& socket, const SocketAddressType& socket_address)
{
#if COM_TYPE == COM_WINSOCK
    /* �\�P�b�g�ɃA�h���X�����o�C���h */
    int bind_result = bind(socket, (struct sockaddr*)&socket_address, sizeof(socket_address));

    /* �\�P�b�g�o�C���h���s���̃G���[���� */
    if (bind_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = WSAGetLastError();

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Socket Bind Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* �\�P�b�g�ɃA�h���X�����o�C���h */
    int bind_result = bind(socket, (struct sockaddr*)&socket_address, sizeof(socket_address));

    /* �\�P�b�g�o�C���h���s���̃G���[���� */
    if (bind_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = errno;

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Socket Bind Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::SetNonBlockingMode(SocketType& socket)
{
#if COM_TYPE == COM_WINSOCK
    /* �m���u���b�L���OI/O���[�h�ɃZ�b�g */
    u_long val = 1;
    int ioctrl_result = ioctlsocket(socket, FIONBIO, &val);

    /* �m���u���b�L���OI/O���[�h�ݒ莸�s���̃G���[���� */
    if (ioctrl_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = WSAGetLastError();

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Socket I/O Control(Set Non Blocking Mode) Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* �m���u���b�L���OI/O���[�h�ɃZ�b�g */
    int val = 1;
    int ioctrl_result = ioctl(socket, FIONBIO, &val);

    /* �m���u���b�L���OI/O���[�h�ݒ莸�s���̃G���[���� */
    if (ioctrl_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = errno;

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Socket I/O Control(Set Non Blocking Mode) Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::SetMulticastTxSocketOption(SocketType& socket, const SocketAddressType& socket_address, const std::string& src_ip)
{
#if COM_TYPE == COM_WINSOCK
    /* ���[�J��IP�A�h���X�Z�b�g */
    IpAddressType local_ip_num = SocketInterface::ConvertIpStrToNum(socket_address.sin_family, src_ip);

    /* UDP�}���`�L���X�g�\�P�b�g�I�v�V�����Z�b�g */
    int set_opt_result = setsockopt(socket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&local_ip_num, sizeof(local_ip_num));

    /* �\�P�b�g�I�v�V�����Z�b�g���s���̃G���[���� */
    if (set_opt_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = WSAGetLastError();

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Multicast Tx Socket Option Set Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* ���[�J��IP�A�h���X�Z�b�g */
    IpAddressType local_ip_num = inet_addr(src_ip.c_str());

    /* UDP�}���`�L���X�g�\�P�b�g�I�v�V�����Z�b�g */
    int set_opt_result = setsockopt(socket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&local_ip_num, sizeof(local_ip_num));

    /* �\�P�b�g�I�v�V�����Z�b�g���s���̃G���[���� */
    if (set_opt_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = errno;

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Multicast Tx Socket Option Set Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::SetMulticastRxSocketOption(SocketType& socket, const SocketAddressType& socket_address, const std::string& dst_ip)
{
#if COM_TYPE == COM_WINSOCK
    MulticastRequestType multicast_request;

    /* �}���`�L���X�g���N�G�X�g�̃Z�b�g */
    memset(&multicast_request, 0, sizeof(multicast_request));
    multicast_request.imr_interface.S_un.S_addr = INADDR_ANY;
    multicast_request.imr_multiaddr.S_un.S_addr = SocketInterface::ConvertIpStrToNum(socket_address.sin_family, dst_ip);

    /* UDP�}���`�L���X�g�\�P�b�g�I�v�V�����Z�b�g */
    int set_opt_result = setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multicast_request, sizeof(multicast_request));

    /* �\�P�b�g�I�v�V�����Z�b�g���s���̃G���[���� */
    if (set_opt_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = WSAGetLastError();

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Multicast Rx Socket Option Set Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    MulticastRequestType multicast_request;

    /* �}���`�L���X�g���N�G�X�g�̃Z�b�g */
    memset(&multicast_request, 0, sizeof(multicast_request));
    multicast_request.imr_interface.s_addr = INADDR_ANY;
    multicast_request.imr_multiaddr.s_addr = inet_addr(dst_ip.c_str());

    /* UDP�}���`�L���X�g�\�P�b�g�I�v�V�����Z�b�g */
    int set_opt_result = setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multicast_request, sizeof(multicast_request));

    /* �\�P�b�g�I�v�V�����Z�b�g���s���̃G���[���� */
    if (set_opt_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = errno;

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Multicast Rx Socket Option Set Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::SetBroadcastTxSocketOption(SocketType& socket)
{
#if COM_TYPE == COM_WINSOCK
    /* UDP�u���[�h�L���X�g���M�p�\�P�b�g�I�v�V�����Z�b�g */
    BOOL yes = 1;
    int set_opt_result = setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));

    /* �\�P�b�g�I�v�V�����Z�b�g���s���̃G���[���� */
    if (set_opt_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = WSAGetLastError();

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Broadcast Tx Socket Option Set Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* UDP�u���[�h�L���X�g���M�p�\�P�b�g�I�v�V�����Z�b�g */
    int yes = 1;
    int set_opt_result = setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));

    /* �\�P�b�g�I�v�V�����Z�b�g���s���̃G���[���� */
    if (set_opt_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = errno;

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Broadcast Tx Socket Option Set Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::SetTTLSocketOption(SocketType& socket, const int32_t ttl)
{
#if COM_TYPE == COM_WINSOCK
    /* TTL�Z�b�g */
    TTLType ttl_value = static_cast<TTLType>(ttl);

    /* TTL�\�P�b�g�I�v�V�����Z�b�g */
    int set_ttl_result = setsockopt(socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl_value, sizeof(ttl_value));

    /* �\�P�b�g�I�v�V�����Z�b�g���s���̃G���[���� */
    if (set_ttl_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = WSAGetLastError();

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("TTL Socket Option Set Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* TTL�Z�b�g */
    TTLType ttl_value = static_cast<TTLType>(ttl);

    /* TTL�\�P�b�g�I�v�V�����Z�b�g */
    int set_ttl_result = setsockopt(socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl_value, sizeof(ttl_value));

    /* �\�P�b�g�I�v�V�����Z�b�g���s���̃G���[���� */
    if (set_ttl_result != 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = errno;

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("TTL Socket Option Set Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::Transmit(SocketType& socket, const SocketAddressType& socket_address, const any_ptr data_ptr, const size_t tx_size)
{
#if COM_TYPE == COM_WINSOCK
    /* �\�P�b�g�Ƀp�P�b�g���M */
    int send_result = sendto(socket, (const char*)data_ptr, (int)tx_size, 0, (struct sockaddr*)&socket_address, sizeof(socket_address));

    /* �p�P�b�g���M���s���̃G���[���� */
    if (send_result == SOCKET_ERROR)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = WSAGetLastError();

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Packet Transmit Failed", s_ErrorCode);
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* �\�P�b�g�Ƀp�P�b�g���M */
    int send_result = sendto(socket, (const char*)data_ptr, (int)tx_size, 0, (struct sockaddr*)&socket_address, sizeof(socket_address));

    /* �p�P�b�g���M���s���̃G���[���� */
    if (send_result < 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = errno;

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Packet Transmit Failed", s_ErrorCode);
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

void SocketInterface::ReceiveSync(SocketType& socket, byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
{
#if COM_TYPE == COM_WINSOCK
    /* �\�P�b�g����p�P�b�g��M(�u���b�L���O) */
    int receive_result = recv(socket, (char*)buffer_ptr, (int)buffer_size, 0);

    /* �p�P�b�g��M���s���̃G���[���� */
    if (receive_result == SOCKET_ERROR)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = WSAGetLastError();

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Packet Receive Failed", s_ErrorCode);
    }
    /* �p�P�b�g��M������ */
    else
    {
        /* ��M�f�[�^�T�C�Y���Z�b�g */
        rx_size = (size_t)receive_result;
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* �\�P�b�g����p�P�b�g��M(�u���b�L���O) */
    int receive_result = recv(socket, (char*)buffer_ptr, (int)buffer_size, 0);

    /* �p�P�b�g��M���s���̃G���[���� */
    if (receive_result < 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = errno;

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Packet Receive Failed", s_ErrorCode);
    }
    /* �p�P�b�g��M������ */
    else
    {
        /* ��M�f�[�^�T�C�Y���Z�b�g */
        rx_size = (size_t)receive_result;
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

bool SocketInterface::ReceiveAsync(SocketType& socket, byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
{
#if COM_TYPE == COM_WINSOCK
    /* �\�P�b�g����p�P�b�g��M(�m���u���b�L���O�u���b�L���O) */
    int receive_result = recv(socket, (char*)buffer_ptr, (int)buffer_size, 0);

    /* �p�P�b�g��M���s���̃G���[���� */
    if (receive_result == SOCKET_ERROR)
    {
        /* �G���[�R�[�h�擾 */
        int error_code = WSAGetLastError();

        /* �f�[�^����M�̏ꍇ */
        if (error_code == WSAEWOULDBLOCK)
        {
            rx_size = 0;

            return false;
        }
        else
        {
            /* �G���[�R�[�h�Z�b�g */
            s_ErrorCode = error_code;

            /* �\�P�b�g��O���o */
            THROW_SOCKET_EXCEPTION("Packet Receive Failed", s_ErrorCode);
        }
    }
    /* �p�P�b�g��M������ */
    else
    {
        /* ��M�f�[�^�T�C�Y���Z�b�g */
        rx_size = (size_t)receive_result;

        return true;
    }
#elif COM_TYPE == COM_SOCKET || COM_TYPE == COM_MACSOCK
    /* �\�P�b�g����p�P�b�g��M(�m���u���b�L���O�u���b�L���O) */
    int receive_result = recv(socket, (char*)buffer_ptr, (int)buffer_size, 0);

    /* �p�P�b�g��M���s���̃G���[���� */
    if (receive_result < 0)
    {
        /* �G���[�R�[�h�擾 */
        int error_code = errno;

        /* �f�[�^����M�̏ꍇ */
        if (error_code == EAGAIN)
        {
            rx_size = 0;

            return false;
        }
        else
        {
            /* �G���[�R�[�h�Z�b�g */
            s_ErrorCode = error_code;

            /* �\�P�b�g��O���o */
            THROW_SOCKET_EXCEPTION("Packet Receive Failed", s_ErrorCode);
        }
    }
    /* �p�P�b�g��M������ */
    else
    {
        /* ��M�f�[�^�T�C�Y���Z�b�g */
        rx_size = (size_t)receive_result;

        return true;
    }
#else
    THROW_APP_EXCEPTION("Invalid Com Type : " + std::to_string(COM_TYPE));
#endif
}

#if COM_TYPE == COM_WINSOCK
/* IP�A�h���X�̕ϊ�(������ -> ���l) */
IpAddressType SocketInterface::ConvertIpStrToNum(const AddressFamilyType family, const std::string& ip)
{
    /* �ϊ���IP�A�h���X */
    ULONG ip_num = 0;

    /* IP�A�h���X�𕶎��񂩂琔�l�ɕϊ� */
    int pton_result = inet_pton(family, ip.c_str(), &ip_num);

    /* IP�A�h���X�ϊ������� */
    if (pton_result == 1)
    {
        return ip_num;
    }
    /* IP�A�h���X�ϊ����s���̃G���[���� */
    else if (pton_result == 0)
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = -1;

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Convert IP Address Failed", s_ErrorCode);
    }
    else
    {
        /* �G���[�R�[�h�Z�b�g */
        s_ErrorCode = WSAGetLastError();

        /* �\�P�b�g��O���o */
        THROW_SOCKET_EXCEPTION("Convert IP Address Failed", s_ErrorCode);
    }
}
#endif
