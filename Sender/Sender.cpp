#include "CompileSwitch.h"
#include "SocketAdapter.h"

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

/* 受信メッセージのエンコード */
static void EncodeTxMessage(const std::string& message, byte_ptr& buffer_ptr, size_t& buffer_size);

int main()
{
    try
    {
        /* Socket全体の初期化 */
        SocketAdapter::Initialize();

        std::cout << "Socket Adapter Initialize Success" << std::endl;

        /* Socket Adapterインスタンス生成 */
        SocketAdapter adapter;

        std::cout << "Socket Adapter Instance Creation Success" << std::endl;

#if TEST_MODE == TEST_MODE_UNICAST
        /* UDPユニキャスト送信用ソケットオープン */
        adapter.OpenUdpUniTxSocket("127.0.0.1", 5000);

        std::cout << "UDP Unicast Tx Socket Open Success" << std::endl;
#elif TEST_MODE == TEST_MODE_MULTICAST
        /* UDPマルチキャスト送信用ソケットオープン */
        adapter.OpenUdpMultiTxSocket("239.192.1.2", "127.0.0.1", 5000);

        std::cout << "UDP Multicast Tx Socket Open Success" << std::endl;
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

                std::cout << "Transmit UDP Unicast Message : [" << tx_msg << "]" << std::endl;

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

        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "[ERROR] " << ex.what() << std::endl;

        /* ソケット初期化/終了/オープン/クローズ失敗時はエラー終了 */
        return -1;
    }
}

/* 受信メッセージのエンコード */
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
