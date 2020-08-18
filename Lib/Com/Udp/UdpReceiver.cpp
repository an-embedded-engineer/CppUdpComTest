#include "UdpReceiver.h"
#include "UdpSocketReceiver.h"
#include "UdpPacketReceiver.h"

#include <functional>
#include <sstream>

/* コンストラクタ */
UdpReceiver::UdpReceiver()
    : m_SocketReceiver(nullptr)
    , m_PacketReceiver(UdpPacketReceiver::GetInstance())
    , m_MessageID(0)
    , m_Message()
    , m_IsReceived(false)
{
    /* UDP Socket Receiverクラスインスタンス生成 */
    this->m_SocketReceiver = std::make_unique<UdpSocketReceiver>();

    /* 受信コールバックセット */
    //UdpPacketRxBufferMap::CallbackType callback = [this](const uint16_t message_id, const byte_ptr data_ptr, const size_t data_size) { this->ReceiveCallback(message_id, data_ptr, data_size); };
    UdpPacketRxBufferMap::CallbackType callback = std::bind(&UdpReceiver::ReceiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    /* 受信コールバックを登録 */
    this->m_PacketReceiver.RegisterCallback(callback);
}

/* デストラクタ */
UdpReceiver::~UdpReceiver()
{
    /* Nothing to do */
}

/* ユニキャスト用ソケットオープン */
void UdpReceiver::OpenUniSocket(const uint16_t local_port)
{
    /* ユニキャスト用ソケットオープン */
    this->m_SocketReceiver->OpenUniSocket(local_port);
}

/* マルチキャスト用ソケットオープン */
void UdpReceiver::OpenMultiSocket(const std::string& multicast_ip, const uint16_t multicast_port)
{
    /* マルチキャスト用ソケットオープン */
    this->m_SocketReceiver->OpenMultiSocket(multicast_ip, multicast_port);
}

/* ブロードキャスト用ソケットオープン */
void UdpReceiver::OpenBroadSocket(const uint16_t local_port)
{
    /* ブロードキャスト用ソケットオープン */
    this->m_SocketReceiver->OpenBroadSocket(local_port);
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
        /* データ同期受信 */
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

/* データ受信コールバック */
void UdpReceiver::ReceiveCallback(const uint16_t message_id, const byte_ptr data_ptr, const size_t data_size)
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

