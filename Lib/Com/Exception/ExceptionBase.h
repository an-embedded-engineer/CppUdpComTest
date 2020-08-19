#pragma once

#include <exception>
#include <string>

namespace exception
{
    /* �G���[��� */
    struct ErrorInfo
    {
        /* �t�@�C���p�X */
        std::string file_path;

        /* �֐��� */
        std::string function_name;
        
        /* �t�@�C���s�ԍ� */
        int line_number;

        /* �R���X�g���N�^ */
        ErrorInfo(const std::string& file, const std::string& func, const int line)
            : file_path(file)
            , function_name(func)
            , line_number(line)
        {
        }
    };

    /* Exception Base�N���X�錾 */
    class ExceptionBase : public std::exception
    {
    public:
        /* �R���X�g���N�^ */
        ExceptionBase(const std::string& message);

        /* �R���X�g���N�^ */
        ExceptionBase(const std::string& message, const std::string& file, const std::string& func, const int line);

        /* �f�X�g���N�^ */
        virtual ~ExceptionBase();

        /* �t�@�C���p�X���擾 */
        const std::string& GetFilePath();

        /* �֐������擾 */
        const std::string& GetFunctionName();

        /* �t�@�C���s�ԍ����擾 */
        int GetLineNumber();

        /* �G���[�����擾 */
        const ErrorInfo& GetErrorInfo();

        /* �G���[�v�����擾 */
        virtual char const* what() const override;

    protected:
        /* �G���[���b�Z�[�W */
        std::string m_Message;
        /* �t�@�C���p�X */
        std::string m_FilePath;
        /* �֐��� */
        std::string m_FunctionName;
        /* �t�@�C���s�ԍ� */
        int m_LineNumber;
        /* �G���[��� */
        ErrorInfo m_ErrorInfo;
        /* �G���[���L�� */
        bool m_IsErrorInfoExists;
    };
}
