#include "Logger.h"
#include <iostream>

/* �R���\�[���o�͕����F����R�[�h */
constexpr const char* ConsoleColorReset     = "\x1b[0m";
constexpr const char* ConsoleColorBlack     = "\x1b[30m";
constexpr const char* ConsoleColorRed       = "\x1b[31m";
constexpr const char* ConsoleColorGreen     = "\x1b[32m";
constexpr const char* ConsoleColorYellow    = "\x1b[33m";
constexpr const char* ConsoleColorBlue      = "\x1b[34m";
constexpr const char* ConsoleColorMagenta   = "\x1b[35m";
constexpr const char* ConsoleColorCyan      = "\x1b[36m";
constexpr const char* ConsoleColorWhite     = "\x1b[37m";


/* �V���O���g���C���X�^���X�擾 */
Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

/* �R���X�g���N�^ */
Logger::Logger()
    : m_Mutex()
    , m_FileLogger()
{
    /* ���O�t�@�C���I�[�v�� */
    this->m_FileLogger.Open("log.txt");
}

/* �f�X�g���N�^ */
Logger::~Logger()
{
    /* ���O�t�@�C���N���[�Y */
    this->m_FileLogger.Close();
}

/* Information���O�o�� */
void Logger::LogInfo(const std::string& message)
{
    /* �~���[�e�b�N�X�ɂ��r������ */
    std::lock_guard<std::mutex> lock(this->m_Mutex);

    /* Information���O�𐶐� */
    std::string log_message = StringFormat("[INFO] %s", message);

    /* Information���O��W���o�͂ɏo��(�V�A��) */
    std::cout << ConsoleColorCyan << log_message << ConsoleColorReset << std::endl;

    /* Information���O���t�@�C���ɏo�� */
    this->m_FileLogger.Write(log_message);
}

/* Warning���O�o�� */
void Logger::LogWarn(const std::string& message)
{
    /* �~���[�e�b�N�X�ɂ��r������ */
    std::lock_guard<std::mutex> lock(this->m_Mutex);

    /* Warning���O�𐶐� */
    std::string log_message = StringFormat("[WARN] %s", message);

    /* Warning���O��W���o�͂ɏo��(���F) */
    std::cerr << ConsoleColorYellow << log_message << ConsoleColorReset << std::endl;

    /* Warning���O���t�@�C���ɏo�� */
    this->m_FileLogger.Write(log_message);
}

/* Error���O�o�� */
void Logger::LogError(const std::string& message)
{
    /* �~���[�e�b�N�X�ɂ��r������ */
    std::lock_guard<std::mutex> lock(this->m_Mutex);

    /* Error���O�𐶐� */
    std::string log_message = StringFormat("[ERROR] %s", message);

    /* Error���O��W���o�͂ɏo��(�ԐF) */
    std::cerr << ConsoleColorRed << log_message << ConsoleColorReset << std::endl;

    /* Error���O���t�@�C���ɏo�� */
    this->m_FileLogger.Write(log_message);
}
