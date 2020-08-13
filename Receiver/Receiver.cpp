#include "SocketAdapter.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

int main()
{
    SocketAdapter::Initialize();

    SocketAdapter adapter;

    SocketResult socket_open_result = adapter.OpenUdpRxSocket(5000);

    if (socket_open_result == SocketResult::Success)
    {
        std::cout << "Socket Open Success" << std::endl;

        while (true)
        {
            uint8_t buffer[1024];
            byte_ptr buffer_ptr = buffer;
            size_t buffer_size = sizeof(buffer);
            size_t rx_size = 0;

            SocketResult socket_receive_result = adapter.ReceiveSync(buffer_ptr, buffer_size, rx_size);

            if (socket_receive_result == SocketResult::Success)
            {
                std::stringstream ss;
                ss << buffer_ptr;

                std::string rx_msg = ss.str();

                std::cout << "Message Received : [" << rx_msg << "]" << std::endl;

                if (rx_msg == "exit")
                {
                    break;
                }
            }
            else
            {
                std::cerr << "Socket Receive Failed" << std::endl;
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

