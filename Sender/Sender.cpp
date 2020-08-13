#include "SocketAdapter.h"

#include <iostream>
#include <thread>
#include <chrono>

int main()
{
    SocketAdapter::Initialize();

    SocketAdapter adapter;

    SocketResult socket_open_result = adapter.OpenUdpTxSocket("127.0.0.1", 5000);

    if (socket_open_result == SocketResult::Success)
    {
        std::cout << "Socket Open Success" << std::endl;

        while (true)
        {
            std::string tx_msg =  "";

            std::cout << ">";
            std::cin >> tx_msg;

            size_t tx_msg_len = tx_msg.length();

            size_t tx_data_size = tx_msg_len + 1;

            byte_ptr tx_data = (byte_ptr)malloc(tx_data_size);

            if (tx_data != nullptr)
            {
                memcpy(tx_data, tx_msg.c_str(), tx_msg_len);

                tx_data[tx_msg_len] = '\0';

                std::cout << "Transmit Message : [" << tx_msg << "]" << std::endl;

                SocketResult socket_tx_result = adapter.Transmit(tx_data, tx_data_size);

                if (socket_tx_result == SocketResult::Success)
                {
                    std::cout << "Socket Transmit Success" << std::endl;
                }
                else
                {
                    std::cerr << "Socket Transmit Failed" << std::endl;
                }

                if (tx_msg == "exit")
                {
                    break;
                }
            }
            else
            {
                std::cerr << "Memory Allocation Failed" << std::endl;
            }
        }

        SocketResult socket_close_result = adapter.CloseUdpSocket();

        if (socket_close_result == SocketResult::Success)
        {
            std::cout << "Socket Close Success" << std::endl;
        }
        else
        {
            std::cerr << "Socket Close Failed" << std::endl;
        }
    }
    else
    {
        std::cerr << "Socket Open Failed" << std::endl;
    }

    SocketAdapter::Finalize();

    return 0;
}

