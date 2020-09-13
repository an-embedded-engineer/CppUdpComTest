#include "FileLogger.h"

/* �R���X�g���N�^ */
FileLogger::FileLogger()
    : m_FilePath("")
    , m_Stream()
{
    /* Nothing to do */
}

/* �R���X�g���N�^ */
FileLogger::FileLogger(const std::string& file_path)
    : m_FilePath(file_path)
    , m_Stream()
{
    /* �t�@�C���I�[�v�� */
    this->Open(file_path);
}

/* �f�X�g���N�^ */
FileLogger::~FileLogger()
{
    /* �t�@�C���N���[�Y */
    this->Close();
}

/* �t�@�C���I�[�v�� */
void FileLogger::Open(const std::string& file_path)
{
    /* �t�@�C�����J���Ă��Ȃ������� */
    if (this->IsOpened() == false)
    {
        this->m_Stream.open(file_path, std::ios::out);
    }
}

/* �t�@�C���N���[�Y */
void FileLogger::Close()
{
    /* �t�@�C�����J���Ă����� */
    if (this->IsOpened() == true)
    {
        this->m_Stream.close();
    }
}

/* �t�@�C���I�[�v���m�F */
bool FileLogger::IsOpened()
{
    return this->m_Stream.is_open();
}

/* �t�@�C���������� */
void FileLogger::Write(const std::string& log)
{
    /* �t�@�C�����J���Ă����� */
    if (this->IsOpened() == true)
    {
        /* �t�@�C���X�g���[���ɏ������� */
        this->m_Stream << log << std::endl;
    }
}
