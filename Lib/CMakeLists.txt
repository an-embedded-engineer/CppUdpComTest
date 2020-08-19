# Generate Com Library
add_library(ComLib
  STATIC
    Com/Exception/AppException.cpp
    Com/Exception/ExceptionBase.cpp
    Com/Exception/SocketException.cpp
    Com/Socket/SocketAdapter.cpp
    Com/Udp/UdpPacketEncoder.cpp
    Com/Udp/UdpPacketReceiver.cpp
    Com/Udp/UdpPacketRxBuffer.cpp
    Com/Udp/UdpPacketRxBufferMap.cpp
    Com/Udp/UdpPacketSender.cpp
    Com/Udp/UdpReceiver.cpp
    Com/Udp/UdpSender.cpp
    Com/Udp/UdpSocketReceiver.cpp
    Com/Udp/UdpSocketSender.cpp
    Utility/StackTracer.cpp
)

# Set Include Directories
target_include_directories(ComLib
  PUBLIC
  ${PROJECT_SOURCE_DIR}/Lib/Com
  ${PROJECT_SOURCE_DIR}/Lib/Com/Exception
  ${PROJECT_SOURCE_DIR}/Lib/Com/Socket
  ${PROJECT_SOURCE_DIR}/Lib/Com/Udp
  ${PROJECT_SOURCE_DIR}/Lib/Config
  ${PROJECT_SOURCE_DIR}/Lib/Utility
)
