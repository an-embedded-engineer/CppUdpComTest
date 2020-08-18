#include "SocketAdapter.h"
#include "UdpSocketSender.h"
#include "UdpSender.h"
#include "CompileSwitch.h"

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

/* Socket Adapterによる通信テスト */
static void SocketAdapterTest();

/* UDP Socket Senderによる通信テスト */
static void UdpSocketSenderTest();

/* UDP Senderによる通信テスト */
static void UdpSenderTest();


/* 送信メッセージのエンコード */
static void EncodeTxMessage(const std::string& message, byte_ptr& buffer_ptr, size_t& buffer_size);
/* 巨大送信メッセージのエンコード */
static void EncodeBigTxMessage(const size_t tx_msg_size, byte_ptr& buffer_ptr, size_t& buffer_size);

int main()
{
    try
    {
#if TEST_MODE == TEST_MODE_SCOKET_ADAPTER
        SocketAdapterTest();
#elif TEST_MODE == TEST_MODE_UDP_SINGLE
        UdpSocketSenderTest();
#elif TEST_MODE == TEST_MODE_UDP_MULTI
        UdpSenderTest();
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
    /* UDPユニキャスト送信用ソケットオープン */
    adapter.OpenUdpUniTxSocket(UNICAST_IP, UDP_PORT);

    std::cout << "UDP Unicast Tx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_MULTICAST
    /* UDPマルチキャスト送信用ソケットオープン */
    adapter.OpenUdpMultiTxSocket(MULTICAST_IP, SENDER_IP, UDP_PORT, 1);

    std::cout << "UDP Multicast Tx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_BROADCAST
    /* UDPブロードキャスト送信用ソケットオープン */
    adapter.OpenUdpBroadTxSocket(BROADCAST_IP, UDP_PORT);

    std::cout << "UDP Broadcast Tx Socket Open Success" << std::endl;
#endif

    while (true)
    {
        /* 送信メッセージ */
        std::string tx_msg = "";
        /* 送信データバッファ */
        byte_ptr buffer_ptr = nullptr;
        /* 送信データバッファサイズ */
        size_t buffer_size = 0;

        /* 送信メッセージを標準入力から入力 */
        std::cout << ">";
        std::cin >> tx_msg;

        try
        {
            /* 送信メッセージをエンコード(送信データバッファ動的確保) */
            EncodeTxMessage(tx_msg, buffer_ptr, buffer_size);

            /* パケット送信 */
            adapter.Transmit((any_ptr)buffer_ptr, buffer_size);

            std::cout << "Transmit UDP Message : [" << tx_msg << "]" << std::endl;

            /* 送信データバッファ解放 */
            free(buffer_ptr);

            /* 送信終了判定 */
            if (tx_msg == "exit")
            {
                break;
            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "[ERROR] " << ex.what() << std::endl;

            /* 送信エラーの場合は、一定時間待機して送信を再開する */
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

/* UDP Socket Senderによる通信テスト */
static void UdpSocketSenderTest()
{
    /* Socket全体の初期化 */
    SocketAdapter::Initialize();

    std::cout << "Socket Adapter Initialize Success" << std::endl;

    /* UDP Socket Senderインスタンス生成 */
    UdpSocketSender sender;

    std::cout << "UDP Socket Sender Instance Creation Success" << std::endl;

#if TEST_COM_MODE == TEST_COM_MODE_UNICAST
    /* ユニキャスト用ソケットオープン */
    sender.OpenUniSocket(UNICAST_IP, UDP_PORT);

    std::cout << "UDP Unicast Tx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_MULTICAST
    /* マルチキャスト用ソケットオープン */
    sender.OpenMultiSocket(MULTICAST_IP, SENDER_IP, UDP_PORT, 1);

    std::cout << "UDP Multicast Tx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_BROADCAST
    /* ブロードキャスト用ソケットオープン */
    sender.OpenBroadSocket(BROADCAST_IP, UDP_PORT);

    std::cout << "UDP Broadcast Tx Socket Open Success" << std::endl;
#endif

    while (true)
    {
        /* 送信メッセージ */
        std::string tx_msg = "";
        /* 送信データバッファ */
        byte_ptr buffer_ptr = nullptr;
        /* 送信データバッファサイズ */
        size_t buffer_size = 0;

        /* 送信メッセージを標準入力から入力 */
        std::cout << ">";
        std::cin >> tx_msg;

        try
        {
            /* 送信メッセージをエンコード(送信データバッファ動的確保) */
            EncodeTxMessage(tx_msg, buffer_ptr, buffer_size);

            /* パケット送信 */
            sender.Transmit((any_ptr)buffer_ptr, buffer_size);

            std::cout << "Transmit UDP Message : [" << tx_msg << "]" << std::endl;

            /* 送信データバッファ解放 */
            free(buffer_ptr);

            /* 送信終了判定 */
            if (tx_msg == "exit")
            {
                break;
            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "[ERROR] " << ex.what() << std::endl;

            /* 送信エラーの場合は、一定時間待機して送信を再開する */
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }

    /* ソケットクローズ */
    sender.CloseSocket();
}

/* UDP Senderによる通信テスト */
static void UdpSenderTest()
{
    /* Socket全体の初期化 */
    SocketAdapter::Initialize();

    std::cout << "Socket Adapter Initialize Success" << std::endl;

    /* UDP Senderインスタンス生成 */
    UdpSender sender;

    std::cout << "UDP Sender Instance Creation Success" << std::endl;

#if TEST_COM_MODE == TEST_COM_MODE_UNICAST
    /* ユニキャスト用ソケットオープン */
    sender.OpenUniSocket(UNICAST_IP, UDP_PORT);

    std::cout << "UDP Unicast Tx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_MULTICAST
    /* マルチキャスト用ソケットオープン */
    sender.OpenMultiSocket(MULTICAST_IP, SENDER_IP, UDP_PORT, 1);

    std::cout << "UDP Multicast Tx Socket Open Success" << std::endl;
#elif TEST_COM_MODE == TEST_COM_MODE_BROADCAST
    /* ブロードキャスト用ソケットオープン */
    sender.OpenBroadSocket(BROADCAST_IP, UDP_PORT);

    std::cout << "UDP Broadcast Tx Socket Open Success" << std::endl;
#endif

    while (true)
    {
        /* 送信メッセージ */
        std::string input_text = "";
        /* 送信データバッファ */
        byte_ptr buffer_ptr = nullptr;
        /* 送信データバッファサイズ */
        size_t buffer_size = 0;

        /* 送信メッセージを標準入力から入力 */
        std::cout << ">";
        std::cin >> input_text;

        try
        {
            /* 入力文字列を数値(送信メッセージサイズ)に変換 */
            size_t tx_msg_size = std::stoull(input_text);

            /* 送信メッセージサイズが0 : 終了 */
            if (tx_msg_size == 0)
            {
                std::string tx_msg = "exit";

                /* 送信メッセージをエンコード(送信データバッファ動的確保) */
                EncodeTxMessage(tx_msg, buffer_ptr, buffer_size);

                /* パケット送信 */
                sender.Transmit(0x0000, buffer_ptr, buffer_size);

                std::cout << "Transmit UDP Message : [" << tx_msg << "]" << std::endl;

                /* 送信データバッファ解放 */
                free(buffer_ptr);

                /* 送信終了判定 */
                if (tx_msg == "exit")
                {
                    break;
                }
            }
            else
            {
                /* 送信メッセージをエンコード(送信データバッファ動的確保) */
                EncodeBigTxMessage(tx_msg_size, buffer_ptr, buffer_size);

                /* パケット送信 */
                sender.Transmit(0x0001, buffer_ptr, buffer_size);

                std::cout << "Transmit UDP Message : Size = [" << tx_msg_size << "]" << std::endl;
                std::cout << buffer_ptr << std::endl;

                /* 送信データバッファ解放 */
                free(buffer_ptr);
            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "[ERROR] " << ex.what() << std::endl;

            /* 送信エラーの場合は、一定時間待機して送信を再開する */
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }

    /* ソケットクローズ */
    sender.CloseSocket();
}

/* 送信メッセージのエンコード */
static void EncodeTxMessage(const std::string& message, byte_ptr& buffer_ptr, size_t& buffer_size)
{
    /* 文字列の長さ(null文字を含まない)を取得 */
    size_t tx_msg_len = message.length();

    /* 送信データサイズ(null文字を含む文字数)を算出 */
    size_t tx_data_size = tx_msg_len + 1;

    /* 送信データサイズ文のメモリを動的確保 */
    byte_ptr tx_data_ptr = (byte_ptr)malloc(tx_data_size);

    /* メモリ確保成功 */
    if (tx_data_ptr != nullptr)
    {
        /* 送信メッセージを送信データバッファにコピー */
        memcpy(tx_data_ptr, message.c_str(), tx_msg_len);

        /* 送信データバッファの終端にnull文字をセット */
        tx_data_ptr[tx_msg_len] = '\0';

        /* 送信バッファ/送信バッファサイズをセット */
        buffer_ptr = tx_data_ptr;
        buffer_size = tx_data_size;
    }
    else
    {
        /* 送信バッファ/送信バッファサイズをクリア */
        buffer_ptr = nullptr;
        buffer_size = 0;

        /* Bad Allocation例外送出 */
        throw std::bad_alloc();
    }
}

static void EncodeBigTxMessage(const size_t tx_msg_size, byte_ptr& buffer_ptr, size_t& buffer_size)
{
    /* 文字列の長さ(null文字を含まない)を取得 */
    size_t tx_msg_len = tx_msg_size;

    /* 送信データサイズ(null文字を含む文字数)を算出 */
    size_t tx_data_size = tx_msg_len + 1;

    /* 送信データサイズ文のメモリを動的確保 */
    byte_ptr tx_data_ptr = (byte_ptr)malloc(tx_data_size);

    /* メモリ確保成功 */
    if (tx_data_ptr != nullptr)
    {
        for (size_t i = 0; i < tx_msg_len; i++)
        {
            tx_data_ptr[i] = (byte)('0') + (byte)(i % 10);
        }

        /* 送信データバッファの終端にnull文字をセット */
        tx_data_ptr[tx_msg_len] = '\0';

        /* 送信バッファ/送信バッファサイズをセット */
        buffer_ptr = tx_data_ptr;
        buffer_size = tx_data_size;
    }
    else
    {
        /* 送信バッファ/送信バッファサイズをクリア */
        buffer_ptr = nullptr;
        buffer_size = 0;

        /* Bad Allocation例外送出 */
        throw std::bad_alloc();
    }
}
