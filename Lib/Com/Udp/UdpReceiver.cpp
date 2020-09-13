#include "UdpReceiver.h"
#include "UdpSocketReceiver.h"
#include "UdpPacketReceiver.h"
#include "Logger.h"

#include <functional>
#include <sstream>

/* コンストラクタ */
UdpReceiver::UdpReceiver()
    : m_SocketReceiver(nullptr)
    , m_PacketReceiver(UdpPacketReceiver::GetInstance())
    , m_MessageQueueMap(UdpMessageQueueMap::GetInstance())
    , m_MessageID(0)
    , m_Message()
    , m_IsReceived(false)
{
    /* UDP Socket Receiverクラスインスタンス生成 */
    this->m_SocketReceiver = std::make_unique<UdpSocketReceiver>();
}

/* デストラクタ */
UdpReceiver::~UdpReceiver()
{
    /* Nothing to do */
}

/* ユニキャスト用ソケットオープン */
void UdpReceiver::OpenUniSocket(const uint16_t local_port, const ReceiveType receive_type)
{
    if (receive_type == ReceiveType::Sync)
    {
        /* 同期受信コールバックを登録 */
        this->m_PacketReceiver.RegisterCallback(std::bind(&UdpReceiver::SyncReceiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
    else
    {
        /* 非同期受信コールバックを登録 */
        this->m_PacketReceiver.RegisterCallback(std::bind(&UdpReceiver::AsyncReceiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    /* ユニキャスト用ソケットオープン */
    this->m_SocketReceiver->OpenUniSocket(local_port, receive_type);
}

/* マルチキャスト用ソケットオープン */
void UdpReceiver::OpenMultiSocket(const std::string& multicast_ip, const uint16_t multicast_port, const ReceiveType receive_type)
{
    if (receive_type == ReceiveType::Sync)
    {
        /* 同期受信コールバックを登録 */
        this->m_PacketReceiver.RegisterCallback(std::bind(&UdpReceiver::SyncReceiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
    else
    {
        /* 非同期受信コールバックを登録 */
        this->m_PacketReceiver.RegisterCallback(std::bind(&UdpReceiver::AsyncReceiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    /* マルチキャスト用ソケットオープン */
    this->m_SocketReceiver->OpenMultiSocket(multicast_ip, multicast_port, receive_type);
}

/* ブロードキャスト用ソケットオープン */
void UdpReceiver::OpenBroadSocket(const uint16_t local_port, const ReceiveType receive_type)
{
    if (receive_type == ReceiveType::Sync)
    {
        /* 同期受信コールバックを登録 */
        this->m_PacketReceiver.RegisterCallback(std::bind(&UdpReceiver::SyncReceiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
    else
    {
        /* 非同期受信コールバックを登録 */
        this->m_PacketReceiver.RegisterCallback(std::bind(&UdpReceiver::AsyncReceiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    /* ブロードキャスト用ソケットオープン */
    this->m_SocketReceiver->OpenBroadSocket(local_port, receive_type);
}

/* ソケットクローズ */
void UdpReceiver::CloseSocket()
{
    /* ソケットクローズ */
    this->m_SocketReceiver->CloseSocket();
}

/* ソケットオープン確認 */
bool UdpReceiver::IsSocketOpened()
{
    /* ソケットオープン確認 */
    return this->m_SocketReceiver->IsSocketOpened();
}

/* データ同期受信 */
void UdpReceiver::ReceiveSync(uint16_t& message_id, std::string& message)
{
    /* 受信完了状態クリア */
    this->m_IsReceived = false;

    while (true)
    {
        /* データ受信 */
        this->m_PacketReceiver.ReceiveSync(*this->m_SocketReceiver);

        /* 受信完了したらループを抜ける */
        if (this->m_IsReceived == true)
        {
            break;
        }
    }

    /* メッセージIDセット */
    message_id = this->m_MessageID;

    /* 受信メッセージセット */
    message = this->m_Message;
}


/* データ非同期受信開始 */
void UdpReceiver::BeginReceiveAsync()
{
    this->m_PacketReceiver.BeginReceiveAsync(*this->m_SocketReceiver);
}

/* データ非同期受信停止 */
void UdpReceiver::EndReceiveAsync()
{
    this->m_PacketReceiver.EndReceiveAsync(*this->m_SocketReceiver);
}


/* データ同期受信コールバック */
void UdpReceiver::SyncReceiveCallback(const uint16_t message_id, const byte_ptr data_ptr, const size_t data_size)
{
    /* メッセージIDセット */
    this->m_MessageID = message_id;

    /* 受信メッセージセット */
    std::stringstream ss;

    ss << data_ptr;

    this->m_Message = ss.str();

    /* 受信完了状態セット */
    this->m_IsReceived = true;
}

/* データ非同期受信コールバック */
void UdpReceiver::AsyncReceiveCallback(const uint16_t message_id, const byte_ptr data_ptr, const size_t data_size)
{
    byte_ptr buffer_ptr = (byte_ptr)malloc(data_size);

    if (buffer_ptr != nullptr)
    {
        memcpy(buffer_ptr, data_ptr, data_size);

        UdpMessage message(message_id, buffer_ptr, data_size);

        this->m_MessageQueueMap.Enqueue(message_id, std::move(message));
    }
    else
    {
        Logger::Error("Memory Allocation Failed");
    }
}

