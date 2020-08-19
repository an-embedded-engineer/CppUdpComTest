#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>

#define UDP_PACKET_HEADER_SIZE  20
#define UDP_PACKET_DATA_SIZE    980
#define UDP_PACKET_SIZE         UDP_PACKET_HEADER_SIZE + UDP_PACKET_DATA_SIZE

struct UdpPacketHeader
{
    uint16_t PacketSize;
    uint16_t SequenceNum;
    uint32_t TotalSize;
    uint32_t TotalBlockNum;
    uint32_t CurrentBlockNum;
    uint16_t MessageID;
    uint16_t Reserved;

    UdpPacketHeader()
        : PacketSize(0)
        , SequenceNum(0)
        , TotalSize(0)
        , TotalBlockNum(0)
        , CurrentBlockNum(0)
        , MessageID(0)
        , Reserved(0)
    {
    }
};

struct UdpPacket
{
    UdpPacketHeader Header;
    uint8_t Data[UDP_PACKET_DATA_SIZE];

    UdpPacket()
        : Header()
        , Data()
    {
        memset(Data, 0, sizeof(Data));
    }
};

union UdpPacketBuffer
{
    UdpPacket Packet;
    uint8_t Binary[UDP_PACKET_SIZE];

    UdpPacketBuffer()
    {
    }
};


