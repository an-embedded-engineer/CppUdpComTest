#include "SocketAdapter.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <exception>

/* 受信メッセージのデコード */
static std::string DecodeRxMessage(byte_ptr buffer_ptr, size_t data_size);

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

        /* UDPユニキャスト受信用ソケットオープン */
        adapter.OpenUdpUniRxSocket(5000);

        std::cout << "UDP Unicast Rx Socket Open Success" << std::endl;

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
                /* パケット同期受信 */
                adapter.ReceiveSync(buffer_ptr, buffer_size, rx_size);

                /* 受信メッセージをデコード */
                std::string rx_msg = DecodeRxMessage(buffer_ptr, rx_size);

                std::cout << "UDP Unicast Message Received : [" << rx_msg << "]" << std::endl;

                /* 受信終了判定 */
                if (rx_msg == "exit")
                {
                    break;
                }
            }
            catch(const std::exception& ex)
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

        return 0;
    }
    catch(const std::exception& ex)
    {
        std::cerr << "[ERROR] " << ex.what() << std::endl;

        /* ソケット初期化/終了/オープン/クローズ失敗時はエラー終了 */
        return -1;
    }
}

/* 受信メッセージのデコード */
static std::string DecodeRxMessage(byte_ptr buffer_ptr, size_t data_size)
{
    /* 文字配列として受け取るのでそのまま文字列に変換 */
    std::stringstream ss;

    ss << buffer_ptr;

    std::string rx_msg = ss.str();

    return rx_msg;
}
