#include "Singleton.h"

/* ミューテックス */
std::mutex SingletonFinalizer::s_Mutex;

/* ファイナライズ関数リスト */
std::deque<SingletonFinalizer::FinalizerFunc> SingletonFinalizer::s_Finalizers;

/* ファイナライズ関数追加 */
void SingletonFinalizer::AddFinalizer(SingletonFinalizer::FinalizerFunc func)
{
    /* ミューテックスによる排他処理 */
    std::lock_guard<std::mutex> lock(SingletonFinalizer::s_Mutex);

    /* リストの先頭にファイナライズ関数を追加 */
    SingletonFinalizer::s_Finalizers.push_front(func);
}

/* シングルトンファイナライズ */
void SingletonFinalizer::Finalize()
{
    /* ミューテックスによる排他処理 */
    std::lock_guard<std::mutex> lock(SingletonFinalizer::s_Mutex);

    /* ファイナライズ関数を順番に呼び出し */
    for (const auto& func : SingletonFinalizer::s_Finalizers)
    {
        func();
    }

    /* ファイナライズ関数リストをクリア */
    SingletonFinalizer::s_Finalizers.clear();
}
