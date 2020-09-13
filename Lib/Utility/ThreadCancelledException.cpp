#include "ThreadCancelledException.h"
#include "CancellableThread.h"
#include "StringFormat.h"

/* コンストラクタ */
ThreadCancelledException::ThreadCancelledException(CancellableThread& thread)
    : m_ErrorMessage("")
{
    /* デフォルト文字列でエラーメッセージ生成 */
    this->m_ErrorMessage = this->GenerateErrorMessage(thread, "Thread Cancelled");
}

/* コンストラクタ */
ThreadCancelledException::ThreadCancelledException(CancellableThread& thread, const std::string& message)
    : m_ErrorMessage("")
{
    /* 指定文字列でエラーメッセージ生成 */
    this->m_ErrorMessage = this->GenerateErrorMessage(thread, message);
}

/* メッセージ内容を取得 */
const char* ThreadCancelledException::what() const noexcept
{
    return this->m_ErrorMessage.c_str();
}

/* エラーメッセージ生成 */
std::string ThreadCancelledException::GenerateErrorMessage(CancellableThread& thread, const std::string& message)
{
    return StringFormat("[Cancelled Exception] %s : ID=%d Name=%s", message, thread.GetID(), thread.GetName());
}
