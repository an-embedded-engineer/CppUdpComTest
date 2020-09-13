#pragma once
#include "StringFormat.h"
#include "FileLogger.h"

#include <string>
#include <mutex>

/* Logger�N���X�錾 */
class Logger
{
public:
    /* �����w��Information���O */
    template<typename ... Args>
    static void Info(const std::string& format, Args&& ... args)
    {
        /* �����t�H�[�}�b�g���ăV���O���g���C���X�^���X��Information���O�o�͌Ăяo�� */
        Logger::GetInstance().LogInfo(StringFormat(format, std::forward<Args>(args) ...));
    }

    /* �����w��Warning���O */
    template<typename ... Args>
    static void Warn(const std::string& format, Args&& ... args)
    {
        /* �����t�H�[�}�b�g���ăV���O���g���C���X�^���X��Warning���O�o�͌Ăяo�� */
        Logger::GetInstance().LogWarn(StringFormat(format, std::forward<Args>(args) ...));
    }

    /* �����w��Error���O */
    template<typename ... Args>
    static void Error(const std::string& format, Args&& ... args)
    {
        /* �����t�H�[�}�b�g���ăV���O���g���C���X�^���X��Error���O�o�͌Ăяo�� */
        Logger::GetInstance().LogError(StringFormat(format, std::forward<Args>(args) ...));
    }

private:
    /* �V���O���g���C���X�^���X�擾 */
    static Logger& GetInstance();

private:
    /* �R���X�g���N�^ */
    Logger();
    /* �f�X�g���N�^ */
    ~Logger();

public:
    /* �R�s�[�R���X�g���N�^���폜 */
    Logger(const Logger&) = delete;
    /* ���[�u�R���X�g���N�^���폜 */
    Logger(Logger&&) = delete;
    /* �R�s�[����I�y���[�^���폜 */
    Logger& operator=(const Logger&) = delete;
    /* ���[�u����I�y���[�^���폜 */
    Logger& operator=(Logger&&) = delete;

private:
    /* Information���O�o�� */
    void LogInfo(const std::string& message);
    /* Warning���O�o�� */
    void LogWarn(const std::string& message);
    /* Error���O�o�� */
    void LogError(const std::string& message);

private:
    /* �~���[�e�b�N�X */
    std::mutex m_Mutex;

    /* File Logger�N���X�C���X�^���X */
    FileLogger m_FileLogger;
};
