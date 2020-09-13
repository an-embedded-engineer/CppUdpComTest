#pragma once
#include <string>

/* Cancellable Threadクラス前方宣言 */
class CancellableThread;

/* Cancelled Exceptionクラス宣言 */
class ThreadCancelledException
{
public:
    /* コンストラクタ */
    ThreadCancelledException(CancellableThread& thread);
    ThreadCancelledException(CancellableThread& thread, const std::string& message);

    /* メッセージ内容を取得 */
    const char* what() const noexcept;

private:
    /* エラーメッセージ生成 */
    std::string GenerateErrorMessage(CancellableThread& thread, const std::string& message);

private:
    /* エラーメッセージ */
    std::string m_ErrorMessage;
};
