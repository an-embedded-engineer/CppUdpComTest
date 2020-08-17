#pragma once
#include <cstdint>

#define UDP_PACKET_HEADER_SIZE  20
#define UDP_PACKET_DATA_SIZE    980

struct UdpPacketHeader
{
    uint16_t PacketSize;
    uint16_t SeqNo;
    uint32_t TotalSize;
    uint32_t TotalBlockNum;
    uint32_t CurrentBlockNum;
    uint16_t MessageID;
    uint16_t Reserved;
};

struct UdpPacket
{
    UdpPacketHeader Header;
    uint8_t Data[UDP_PACKET_DATA_SIZE];
};
