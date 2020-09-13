#pragma once
#include <fstream>

/* File Logger�N���X�錾 */
class FileLogger
{
public:
    /* �R���X�g���N�^ */
    FileLogger();

    /* �R���X�g���N�^ */
    FileLogger(const std::string& file_path);

    /* �f�X�g���N�^ */
    ~FileLogger();

    /* �t�@�C���I�[�v�� */
    void Open(const std::string& file_path);

    /* �t�@�C���N���[�Y */
    void Close();

    /* �t�@�C���I�[�v���m�F */
    bool IsOpened();

    /* �t�@�C���������� */
    void Write(const std::string& log);

private:
    /* ���O�t�@�C���p�X */
    std::string m_FilePath;

    /* �t�@�C���o�̓X�g���[�� */
    std::ofstream m_Stream;
};

