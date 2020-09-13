#include "SocketAdapter.h"
#include "UdpSocketReceiver.h"
#include "UdpReceiver.h"
#include "CompileSwitch.h"
#include "ReceiveType.h"
#include "UdpMessageQueueMap.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <exception>

#if TEST_MODE == TEST_MODE_SCOKET_ADAPTER
/* Socket Adapterによる通信テスト */
static void SocketAdapterTest();
#elif TEST_MODE == TEST_MODE_UDP_SINGLE
/* UDP Socket Receiverによる通信テスト */
static void UdpSocketReceiverTest();
#elif TEST_MODE == TEST_MODE_UDP_MULTI
/* UDP Receiverによる通信テスト */
#if 0
static void SyncUdpReceiverTest();
#else
static void AsyncUdpReceiverTest();
#endif
#endif

/* 受信メッセージのデコード */
static std::string DecodeRxMessage(byte_ptr buffer_ptr, size_t data_size);

constexpr ReceiveType RecvType = ReceiveType::Async;

int main()
{
    try
    {
#if TEST_MODE == TEST_MODE_SCOKET_ADAPTER
        SocketAdapterTest();
#elif TEST_MODE == TEST_MODE_UDP_SINGLE
        UdpSocketReceiverTest();
#elif TEST_MODE == TEST_MODE_UDP_MULTI
        //SyncUdpReceiverTest();
        AsyncUdpReceiverTest();
#endif

        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "[ERROR] " << ex.what() << std::endl;

        /* ソケット初期化/終了/オープン/クローズ失敗時はエラー終了 */
        return -1;
    }
}

#if TEST_MODE == TEST_MODE_SCOKET_ADAPTER
/* Socket Adapterによる通信テスト */
static void SocketAdapterTest()
{
    /* Socket全体の初期化 */
    SocketAdapter::Initialize();

    std::cout << "Socket Adapter Initialize Success" << std::endl;

    /* Socket Adapterインスタンス生成 */
    SocketAdapter adapter;

    std::cout << "Socket Adapter Instance Creation Success" << std::endl;

#if TEST_COM_MODE == TEST_COM_MODE_UNICAST
    /* UDPユニキャスト受信用ソケットオープン */
    adapter.OpenUdpUniRxSocket(UDP_PORT, RecvType);

    std::cout << "UDP Unicast Rx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_MULTICAST
    /* UDPマルチキャスト受信用ソケットオープン */
    adapter.OpenUdpMultiRxSocket(MULTICAST_IP, UDP_PORT, RecvType);

    std::cout << "UDP Multicast Rx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_BROADCAST
    /* UDPブロードキャスト受信用ソケットオープン */
    adapter.OpenUdpBroadRxSocket(UDP_PORT, RecvType);

    std::cout << "UDP Broadcast Rx Socket Open Success" << std::endl;
#endif

    while (true)
    {
        /* 受信バッファ */
        uint8_t buffer[1024];
        byte_ptr buffer_ptr = buffer;
        /* 受信バッファサイズ */
        size_t buffer_size = sizeof(buffer);
        /* 受信データサイズ */
        size_t rx_size = 0;

        try
        {
            /* パケット受信 */
            adapter.Receive(buffer_ptr, buffer_size, rx_size);

            /* 受信メッセージをデコード */
            std::string rx_msg = DecodeRxMessage(buffer_ptr, rx_size);

            std::cout << "UDP Message Received : [" << rx_msg << "]" << std::endl;

            /* 受信終了判定 */
            if (rx_msg == "exit")
            {
                break;
            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "[ERROR] " << ex.what() << std::endl;

            /* 受信エラーの場合は、一定時間待機して受信を再開する */
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }

    /* ソケットクローズ */
    adapter.CloseSocket();

    std::cout << "Socket Close Success" << std::endl;

    /* Socket全体の後始末 */
    SocketAdapter::Finalize();

    std::cout << "Socket Adapter Finalize Success" << std::endl;
}

#elif TEST_MODE == TEST_MODE_UDP_SINGLE
/* UDP Socket Receiverによる通信テスト */
static void UdpSocketReceiverTest()
{
    /* Socket全体の初期化 */
    SocketAdapter::Initialize();

    std::cout << "Socket Adapter Initialize Success" << std::endl;

    /* UDP Socket Receiverインスタンス生成 */
    UdpSocketReceiver receiver;

    std::cout << "UDP Socket Receiver Instance Creation Success" << std::endl;

#if TEST_COM_MODE == TEST_COM_MODE_UNICAST
    /* ユニキャスト用ソケットオープン */
    receiver.OpenUniSocket(UDP_PORT, RecvType);

    std::cout << "UDP Unicast Rx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_MULTICAST
    /* マルチキャスト用ソケットオープン */
    receiver.OpenMultiSocket(MULTICAST_IP, UDP_PORT, RecvType);

    std::cout << "UDP Multicast Rx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_BROADCAST
    /* ブロードキャスト用ソケットオープン */
    receiver.OpenBroadSocket(UDP_PORT, RecvType);

    std::cout << "UDP Broadcast Rx Socket Open Success" << std::endl;
#endif

    while (true)
    {
        /* 受信バッファ */
        uint8_t buffer[1024];
        byte_ptr buffer_ptr = buffer;
        /* 受信バッファサイズ */
        size_t buffer_size = sizeof(buffer);
        /* 受信データサイズ */
        size_t rx_size = 0;

        try
        {
            /* パケット受信 */
            receiver.Receive(buffer_ptr, buffer_size, rx_size);

            /* 受信メッセージをデコード */
            std::string rx_msg = DecodeRxMessage(buffer_ptr, rx_size);

            std::cout << "UDP Message Received : [" << rx_msg << "]" << std::endl;

            /* 受信終了判定 */
            if (rx_msg == "exit")
            {
                break;
            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "[ERROR] " << ex.what() << std::endl;

            /* 受信エラーの場合は、一定時間待機して受信を再開する */
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }

    /* ソケットクローズ */
    receiver.CloseSocket();

    std::cout << "Socket Close Success" << std::endl;

    /* Socket全体の後始末 */
    SocketAdapter::Finalize();

    std::cout << "Socket Adapter Finalize Success" << std::endl;
}

#elif TEST_MODE == TEST_MODE_UDP_MULTI
/* UDP Receiverによる通信テスト */
#if 0
static void SyncUdpReceiverTest()
{
    /* Socket全体の初期化 */
    SocketAdapter::Initialize();

    std::cout << "Socket Adapter Initialize Success" << std::endl;

    /* UDP Receiverインスタンス生成 */
    UdpReceiver receiver;

    std::cout << "UDP Receiver Instance Creation Success" << std::endl;

#if TEST_COM_MODE == TEST_COM_MODE_UNICAST
    /* ユニキャスト用ソケットオープン */
    receiver.OpenUniSocket(UDP_PORT, RecvType);

    std::cout << "UDP Unicast Rx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_MULTICAST
    /* マルチキャスト用ソケットオープン */
    receiver.OpenMultiSocket(MULTICAST_IP, UDP_PORT, RecvType);

    std::cout << "UDP Multicast Rx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_BROADCAST
    /* ブロードキャスト用ソケットオープン */
    receiver.OpenBroadSocket(UDP_PORT, RecvType);

    std::cout << "UDP Broadcast Rx Socket Open Success" << std::endl;
#endif

    while (true)
    {
        uint16_t message_id = 0;
        std::string message = "";

        try
        {
            /* パケット受信 */
            receiver.ReceiveSync(message_id, message);

            std::cout << "UDP Message Received : [" << message_id << "]" << std::endl;
            std::cout << message << std::endl;

            /* 受信終了判定 */
            if (message == "exit")
            {
                break;
            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "[ERROR] " << ex.what() << std::endl;

            /* 受信エラーの場合は、一定時間待機して受信を再開する */
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }

    /* ソケットクローズ */
    receiver.CloseSocket();

    std::cout << "Socket Close Success" << std::endl;

    /* Socket全体の後始末 */
    SocketAdapter::Finalize();

    std::cout << "Socket Adapter Finalize Success" << std::endl;
}
#else
/* UDP Receiverによる通信テスト */
static void AsyncUdpReceiverTest()
{
    /* Socket全体の初期化 */
    SocketAdapter::Initialize();

    std::cout << "Socket Adapter Initialize Success" << std::endl;

    /* UDP Receiverインスタンス生成 */
    UdpReceiver receiver;

    std::cout << "UDP Receiver Instance Creation Success" << std::endl;

#if TEST_COM_MODE == TEST_COM_MODE_UNICAST
    /* ユニキャスト用ソケットオープン */
    receiver.OpenUniSocket(UDP_PORT, RecvType);

    std::cout << "UDP Unicast Rx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_MULTICAST
    /* マルチキャスト用ソケットオープン */
    receiver.OpenMultiSocket(MULTICAST_IP, UDP_PORT, RecvType);

    std::cout << "UDP Multicast Rx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_BROADCAST
    /* ブロードキャスト用ソケットオープン */
    receiver.OpenBroadSocket(UDP_PORT, RecvType);

    std::cout << "UDP Broadcast Rx Socket Open Success" << std::endl;
#endif

    UdpMessageQueueMap& udp_msg_queue_map = UdpMessageQueueMap::GetInstance();

    try
    {
        /* パケット受信 */
        receiver.BeginReceiveAsync();

        bool is_exit = false;

        while (true)
        {
            const auto& keys = udp_msg_queue_map.GetKeys();
            for (const auto& key : keys)
            {
                uint16_t message_id = key;
                if (udp_msg_queue_map.IsEmpty(message_id) != true)
                {
                    UdpMessage message;

                    bool result = udp_msg_queue_map.Dequeue(message_id, message);

                    if (result == true)
                    {
                        std::string message_text = DecodeRxMessage(message.GetDataPtr(), message.GetDataSize());

                        std::cout << "UDP Message Received : ID=" << message_id << std::endl;
                        std::cout << message_text << std::endl;

                        /* 受信終了判定 */
                        if (message_text == "exit")
                        {
                            is_exit = true;
                            break;
                        }

                    }
                }
            }

            if (is_exit == true)
            {
                receiver.EndReceiveAsync();
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

    }
    catch (const std::exception& ex)
    {
        std::cerr << "[ERROR] " << ex.what() << std::endl;

        /* 受信エラーの場合は、一定時間待機して受信を再開する */
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    /* ソケットクローズ */
    receiver.CloseSocket();

    std::cout << "Socket Close Success" << std::endl;

    /* Socket全体の後始末 */
    SocketAdapter::Finalize();

    std::cout << "Socket Adapter Finalize Success" << std::endl;
}
#endif
#endif

/* 受信メッセージのデコード */
static std::string DecodeRxMessage(byte_ptr buffer_ptr, size_t data_size)
{
    /* 文字配列として受け取るのでそのまま文字列に変換 */
    std::stringstream ss;

    ss << buffer_ptr;

    std::string rx_msg = ss.str();

    return rx_msg;
}
