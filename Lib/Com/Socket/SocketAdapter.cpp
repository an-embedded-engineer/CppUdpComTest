#include "SocketAdapter.h"
#include "CompileSwitch.h"
#include "SocketInterface.h"
#include "SocketException.h"
#include "AsyncTaskManager.h"
#include "SocketReceiveData.h"
#include "SocketReceiveQueue.h"


#include <chrono>
#include <thread>

/* Socket Adapter Implクラス定義 */
class SocketAdapterImpl final
{
public:
    /* Socket全体の初期化 */
    static void Initialize();

    /* Socket全体の後始末 */
    static void Finalize();

    /* Socket全体の初期化確認 */
    static bool IsInitialized();

    /* エラーコード取得 */
    static int GetErrorCode();

    /* コンストラクタ */
    SocketAdapterImpl();

    /* デストラクタ */
    ~SocketAdapterImpl();

    /* UDPユニキャスト送信用ソケットオープン */
    void OpenUdpUniTxSocket(const std::string& remote_ip, const uint16_t remote_port);

    /* UDPユニキャスト受信用ソケットオープン */
    void OpenUdpUniRxSocket(const uint16_t local_port, const ReceiveType receive_type);

    /* UDPマルチキャスト送信用ソケットオープン */
    void OpenUdpMultiTxSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl);

    /* UDPマルチキャスト受信用ソケットオープン */
    void OpenUdpMultiRxSocket(const std::string& multicast_ip, const uint16_t multicast_port, const ReceiveType receive_type);

    /* UDPブロードキャスト送信用ソケットオープン */
    void OpenUdpBroadTxSocket(const std::string& remote_ip, const uint16_t remote_port);

    /* UDPブロードキャスト受信用ソケットオープン */
    void OpenUdpBroadRxSocket(const uint16_t local_port, const ReceiveType receive_type);

    /* ソケットクローズ */
    void CloseSocket();

    /* ソケットオープン確認 */
    bool IsSocketOpened();

    /* パケット送信 */
    void Transmit(const any_ptr data_ptr, const size_t tx_size);

    /* パケット同期受信 */
    void ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size);

    /* パケット非同期受信開始 */
    void BeginReceiveAsync();

    /* パケット非同期受信停止 */
    void EndReceiveAsync();

private:
    /* 受信タスク処理 */
    void ReceiveTaskFunc(AsyncTask& task, CancellationPoint& cp);

private:
    /* Socket全体の初期化フラグ */
    static bool s_IsInitialized;
    /* エラーコード */
    static int s_ErrorCode;

private:
    /* ソケット */
    SocketType m_Socket;
    /* ソケットアドレス情報 */
    SocketAddressType m_Address;
    /* 受信タイプ */
    ReceiveType m_ReceiveType;
    /* ソケットオープン状態 */
    bool m_IsSocketOpened;
    /* Async Task Manager */
    AsyncTaskManager& m_AsyncTaskManager;
    /* 受信タスクID */
    task_id_t m_ReceiveTaskID;
    /* ソケット受信キュー */
    SocketReceiveQueue& m_ReceiveQueue;
};

/* Socket全体の初期化フラグ */
bool SocketAdapterImpl::s_IsInitialized = false;

/* エラーコード */
int SocketAdapterImpl::s_ErrorCode = 0;


/* Socket全体の初期化 */
void SocketAdapter::Initialize()
{
    SocketAdapterImpl::Initialize();
}

/* Socket全体の後始末 */
void SocketAdapter::Finalize()
{
    SocketAdapterImpl::Finalize();
}

/* Socket全体の初期化確認 */
bool SocketAdapter::IsInitialized()
{
    return SocketAdapterImpl::IsInitialized();
}

/* エラーコード取得 */
int SocketAdapter::GetErrorCode()
{
    return SocketAdapterImpl::GetErrorCode();
}

/* コンストラクタ */
SocketAdapter::SocketAdapter()
    : m_Impl(nullptr)
{
    /* Socket Adapter Implクラスインスタンス生成 */
    this->m_Impl = std::make_unique<SocketAdapterImpl>();
}

/* デストラクタ */
SocketAdapter::~SocketAdapter()
{
    /* Nothing to do */
}

/* UDPユニキャスト送信用ソケットオープン */
void SocketAdapter::OpenUdpUniTxSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    this->m_Impl->OpenUdpUniTxSocket(remote_ip, remote_port);
}

/* UDPユニキャスト受信用ソケットオープン */
void SocketAdapter::OpenUdpUniRxSocket(const uint16_t local_port, const ReceiveType receive_type)
{
    this->m_Impl->OpenUdpUniRxSocket(local_port, receive_type);
}

/* UDPマルチキャスト送信用ソケットオープン */
void SocketAdapter::OpenUdpMultiTxSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl)
{
    this->m_Impl->OpenUdpMultiTxSocket(multicast_ip, local_ip, multicast_port, ttl);
}

/* UDPマルチキャスト受信用ソケットオープン */
void SocketAdapter::OpenUdpMultiRxSocket(const std::string& multicast_ip, const uint16_t multicast_port, const ReceiveType receive_type)
{
    this->m_Impl->OpenUdpMultiRxSocket(multicast_ip, multicast_port, receive_type);
}

/* UDPブロードキャスト送信用ソケットオープン */
void SocketAdapter::OpenUdpBroadTxSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    this->m_Impl->OpenUdpBroadTxSocket(remote_ip, remote_port);
}

/* UDPブロードキャスト受信用ソケットオープン */
void SocketAdapter::OpenUdpBroadRxSocket(const uint16_t local_port, const ReceiveType receive_type)
{
    this->m_Impl->OpenUdpBroadRxSocket(local_port, receive_type);
}

/* ソケットクローズ */
void SocketAdapter::CloseSocket()
{
    this->m_Impl->CloseSocket();
}

/* ソケットオープン確認 */
bool SocketAdapter::IsSocketOpened()
{
    return this->m_Impl->IsSocketOpened();
}

/* パケット送信 */
void SocketAdapter::Transmit(const any_ptr data_ptr, const size_t tx_size)
{
    this->m_Impl->Transmit(data_ptr, tx_size);
}

/* パケット受信 */
void SocketAdapter::ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
{
    this->m_Impl->ReceiveSync(buffer_ptr, buffer_size, rx_size);
}

/* パケット非同期受信開始 */
void SocketAdapter::BeginReceiveAsync()
{
    this->m_Impl->BeginReceiveAsync();
}

/* パケット非同期受信停止 */
void SocketAdapter::EndReceiveAsync()
{
    this->m_Impl->EndReceiveAsync();
}


/* Socket全体の初期化 */
void SocketAdapterImpl::Initialize()
{
    if (SocketAdapterImpl::IsInitialized() == false)
    {
        /* エラーコードクリア */
        SocketAdapterImpl::s_ErrorCode = 0;

        SocketInterface::Initialize();

        SocketAdapterImpl::s_IsInitialized = true;
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is already initialized", SocketAdapterImpl::s_ErrorCode);
    }
}

/* Socket全体の後始末 */
void SocketAdapterImpl::Finalize()
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        SocketInterface::Finalize();

        SocketAdapterImpl::s_IsInitialized = false;
    }
    else
    {
        /* Nothing to do */
    }
}

/* Socket全体の初期化確認 */
bool SocketAdapterImpl::IsInitialized()
{
    return SocketAdapterImpl::s_IsInitialized;
}

/* エラーコード取得 */
int SocketAdapterImpl::GetErrorCode()
{
    return SocketAdapterImpl::s_ErrorCode;
}

/* コンストラクタ */
SocketAdapterImpl::SocketAdapterImpl()
    : m_Socket(0)
    , m_Address()
    , m_ReceiveType(ReceiveType::Sync)
    , m_IsSocketOpened(false)
    , m_AsyncTaskManager(AsyncTaskManager::GetInstance())
    , m_ReceiveTaskID(0)
    , m_ReceiveQueue(SocketReceiveQueue::GetInstance())
{
    /* Nothing to do */
}

/* デストラクタ */
SocketAdapterImpl::~SocketAdapterImpl()
{
    /* ソケットオープン確認 */
    if (this->IsSocketOpened() == true)
    {
        /* ソケットがオープンしていたらクローズする */
        this->CloseSocket();
    }
}

/* UDPユニキャスト送信用ソケットオープン */
void SocketAdapterImpl::OpenUdpUniTxSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        /* UDP用ソケットをオープン */
        this->m_Socket = SocketInterface::OpenUdpSocket();

        this->m_Address = SocketInterface::GetTxSocketAddress(remote_ip, remote_port);

        /* ソケットオープン状態更新 */
        this->m_IsSocketOpened = true;
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is not initialized", SocketAdapterImpl::s_ErrorCode);
    }
}

/* UDPユニキャスト受信用ソケットオープン */
void SocketAdapterImpl::OpenUdpUniRxSocket(const uint16_t local_port, const ReceiveType receive_type)
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        /* 受信タイプをセット */
        this->m_ReceiveType = receive_type;

        /* UDP用ソケットをオープン */
        this->m_Socket = SocketInterface::OpenUdpSocket();

        this->m_Address = SocketInterface::GetRxSocketAddress(local_port);

        SocketInterface::BindSocket(this->m_Socket, this->m_Address);

        /* 受信タイプが非同期の場合 */
        if (this->m_ReceiveType == ReceiveType::Async)
        {
            SocketInterface::SetNonBlockingMode(this->m_Socket);
        }

        /* ソケットオープン状態更新 */
        this->m_IsSocketOpened = true;
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is not initialized", SocketAdapterImpl::s_ErrorCode);
    }
}

/* UDPマルチキャスト送信用ソケットオープン */
void SocketAdapterImpl::OpenUdpMultiTxSocket(const std::string& multicast_ip, const std::string& local_ip, const uint16_t multicast_port, const int32_t ttl)
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        /* UDP用ソケットをオープン */
        this->m_Socket = SocketInterface::OpenUdpSocket();

        this->m_Address = SocketInterface::GetTxSocketAddress(multicast_ip, multicast_port);

        SocketInterface::SetMulticastTxSocketOption(this->m_Socket, this->m_Address, local_ip);

        SocketInterface::SetTTLSocketOption(this->m_Socket, ttl);

        /* ソケットオープン状態更新 */
        this->m_IsSocketOpened = true;
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is not initialized", SocketAdapterImpl::s_ErrorCode);
    }
}

/* UDPマルチキャスト受信用ソケットオープン */
void SocketAdapterImpl::OpenUdpMultiRxSocket(const std::string& multicast_ip, const uint16_t multicast_port, const ReceiveType receive_type)
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        /* 受信タイプをセット */
        this->m_ReceiveType = receive_type;

        /* UDP用ソケットをオープン */
        this->m_Socket = SocketInterface::OpenUdpSocket();

        this->m_Address = SocketInterface::GetRxSocketAddress(multicast_port);

        SocketInterface::BindSocket(this->m_Socket, this->m_Address);

        SocketInterface::SetMulticastRxSocketOption(this->m_Socket, this->m_Address, multicast_ip);

        /* 受信タイプが非同期の場合 */
        if (this->m_ReceiveType == ReceiveType::Async)
        {
            SocketInterface::SetNonBlockingMode(this->m_Socket);
        }

        /* ソケットオープン状態更新 */
        this->m_IsSocketOpened = true;
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is not initialized", SocketAdapterImpl::s_ErrorCode);
    }
}

/* UDPブロードキャスト送信用ソケットオープン */
void SocketAdapterImpl::OpenUdpBroadTxSocket(const std::string& remote_ip, const uint16_t remote_port)
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        /* UDP用ソケットをオープン */
        this->m_Socket = SocketInterface::OpenUdpSocket();

        this->m_Address = SocketInterface::GetTxSocketAddress(remote_ip, remote_port);

        SocketInterface::SetBroadcastTxSocketOption(this->m_Socket);

        /* ソケットオープン状態更新 */
        this->m_IsSocketOpened = true;
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is not initialized", SocketAdapterImpl::s_ErrorCode);
    }
}

/* UDPブロードキャスト受信用ソケットオープン */
void SocketAdapterImpl::OpenUdpBroadRxSocket(const uint16_t local_port, const ReceiveType receive_type)
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        /* 受信タイプをセット */
        this->m_ReceiveType = receive_type;

        /* UDP用ソケットをオープン */
        this->m_Socket = SocketInterface::OpenUdpSocket();

        this->m_Address = SocketInterface::GetRxSocketAddress(local_port);

        SocketInterface::BindSocket(this->m_Socket, this->m_Address);

        /* 受信タイプが非同期の場合 */
        if (this->m_ReceiveType == ReceiveType::Async)
        {
            SocketInterface::SetNonBlockingMode(this->m_Socket);
        }

        /* ソケットオープン状態更新 */
        this->m_IsSocketOpened = true;
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is not initialized", SocketAdapterImpl::s_ErrorCode);
    }
}

/* ソケットクローズ */
void SocketAdapterImpl::CloseSocket()
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        /* ソケットオープン確認 */
        if (this->IsSocketOpened() == true)
        {
            SocketInterface::CloseSocket(this->m_Socket);

            /* ソケットオープン状態をクリア */
            this->m_IsSocketOpened = false;
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is not initialized", SocketAdapterImpl::s_ErrorCode);
    }
}

/* ソケットオープン確認 */
bool SocketAdapterImpl::IsSocketOpened()
{
    return this->m_IsSocketOpened;
}

/* パケット送信 */
void SocketAdapterImpl::Transmit(const any_ptr data_ptr, const size_t tx_size)
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        SocketInterface::Transmit(this->m_Socket, this->m_Address, data_ptr, tx_size);
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is not initialized", SocketAdapterImpl::s_ErrorCode);
    }
}

/* パケット受信 */
void SocketAdapterImpl::ReceiveSync(byte_ptr& buffer_ptr, const size_t buffer_size, size_t& rx_size)
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        /* 受信タイプが同期の場合 */
        if (this->m_ReceiveType == ReceiveType::Sync)
        {
            SocketInterface::ReceiveSync(this->m_Socket, buffer_ptr, buffer_size, rx_size);
        }
        else
        {
            while (true)
            {
                bool result = SocketInterface::ReceiveAsync(this->m_Socket, buffer_ptr, buffer_size, rx_size);

                if (result == true)
                {
                    /* ループを抜ける */
                    break;
                }
                else
                {
                    /* 10us待機 */
                    std::this_thread::sleep_for(std::chrono::microseconds(10));

                }
            }
        }
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is not initialized", SocketAdapterImpl::s_ErrorCode);
    }
}


/* パケット非同期受信開始 */
void SocketAdapterImpl::BeginReceiveAsync()
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        if (this->m_ReceiveType == ReceiveType::Async)
        {
            if (this->m_ReceiveTaskID == 0)
            {
                this->m_ReceiveTaskID = this->m_AsyncTaskManager.AddTask("Callback Receive Task", &SocketAdapterImpl::ReceiveTaskFunc, this);

                this->m_AsyncTaskManager.Start(this->m_ReceiveTaskID);
            }
            else
            {
                SocketAdapterImpl::s_ErrorCode = -1;

                THROW_SOCKET_EXCEPTION("Receive Task is already running", SocketAdapterImpl::s_ErrorCode);
            }
        }
        else
        {
            SocketAdapterImpl::s_ErrorCode = -1;

            THROW_SOCKET_EXCEPTION("Receive Type is not Async", SocketAdapterImpl::s_ErrorCode);
        }
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is not initialized", SocketAdapterImpl::s_ErrorCode);
    }
}

/* パケット非同期受信停止 */
void SocketAdapterImpl::EndReceiveAsync()
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        if (this->m_ReceiveType == ReceiveType::Async)
        {
            if (this->m_AsyncTaskManager.IsRunning(this->m_ReceiveTaskID) == true)
            {
                this->m_AsyncTaskManager.Stop(this->m_ReceiveTaskID);

                this->m_ReceiveTaskID = 0;
            }
            else
            {
                SocketAdapterImpl::s_ErrorCode = -1;

                THROW_SOCKET_EXCEPTION("Receive Task is not running", SocketAdapterImpl::s_ErrorCode);
            }
        }
        else
        {
            SocketAdapterImpl::s_ErrorCode = -1;

            THROW_SOCKET_EXCEPTION("Receive Type is not Async", SocketAdapterImpl::s_ErrorCode);
        }
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is not initialized", SocketAdapterImpl::s_ErrorCode);
    }
}

/* コールバック受信タスク処理 */
void SocketAdapterImpl::ReceiveTaskFunc(AsyncTask& task, CancellationPoint& cp)
{
    if (SocketAdapterImpl::IsInitialized() == true)
    {
        if (this->m_ReceiveType == ReceiveType::Async)
        {
            while (true)
            {
                cp.CheckCancelRequested();

                byte buffer[2000];
                byte_ptr buffer_ptr = buffer;
                size_t buffer_size = sizeof(buffer);

                size_t rx_size = 0;

                bool result = SocketInterface::ReceiveAsync(this->m_Socket, buffer_ptr, buffer_size, rx_size);

                cp.CheckCancelRequested();

                if (result == true)
                {
                    size_t data_size = rx_size;
                    byte_ptr data_ptr = (byte_ptr)malloc(data_size);
                    if (data_ptr != nullptr)
                    {
                        memcpy(data_ptr, buffer_ptr, data_size);

                        SocketReceiveData recv_data(data_ptr, data_size);

                        this->m_ReceiveQueue.Enqueue(std::move(recv_data));
                    }
                    else
                    {
                        SocketAdapterImpl::s_ErrorCode = -1;

                        THROW_SOCKET_EXCEPTION("Memory Allocation Failed", SocketAdapterImpl::s_ErrorCode);
                    }
                }
                else
                {
                    /* 10us待機 */
                    std::this_thread::sleep_for(std::chrono::microseconds(10));
                }
            }
        }
        else
        {
            SocketAdapterImpl::s_ErrorCode = -1;

            THROW_SOCKET_EXCEPTION("Receive Type is not Async", SocketAdapterImpl::s_ErrorCode);
        }
    }
    else
    {
        SocketAdapterImpl::s_ErrorCode = 1;

        THROW_SOCKET_EXCEPTION("Socket Adapter is not initialized", SocketAdapterImpl::s_ErrorCode);
    }
}
