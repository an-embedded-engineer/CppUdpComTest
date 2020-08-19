#pragma once

#include <cassert>
#include <deque>
#include <mutex>

/* Singleton Finalizerクラス宣言 */
class SingletonFinalizer
{
public:
    /* シングルトンファイナライズ関数型定義 */
    using FinalizerFunc = void(*)();

public:
    /* ファイナライズ関数追加 */
    static void AddFinalizer(FinalizerFunc func);

    /* シングルトンファイナライズ */
    static void Finalize();

private:
    /* ミューテックス */
    static std::mutex s_Mutex;

    /* ファイナライズ関数リスト */
    static std::deque<SingletonFinalizer::FinalizerFunc> s_Finalizers;
};

/* Singletonテンプレートクラス宣言 */
template <typename T>
class Singleton final
{
public:
    /* シングルトンインスタンスゲット */
    static T& GetInstance()
    {
        /* 1度だけインスタンス生成メソッドを呼び出し */
        std::call_once(Singleton<T>::s_InitFlag, Singleton<T>::Create);
        
        /* インスタンス生成確認 */
        assert(Singleton<T>::s_Instance);
        
        /* シングルトンインスタンスを返す */
        return *Singleton<T>::s_Instance;
    }

private:
    /* シングルトンインスタンス生成 */
    static void Create()
    {
        /* シングルトンインスタンス生成 */
        Singleton<T>::s_Instance = new T;
        
        /* Singleton Finalizerにシングルトンインスタンス破壊メソッドを登録 */
        SingletonFinalizer::AddFinalizer(&Singleton<T>::Destroy);
    }

    /* シングルトンインスタンス破壊 */
    static void Destroy()
    {
        /* シングルトンインスタンス削除 */
        delete Singleton<T>::s_Instance;

        /* シングルトンインスタンスアドレスクリア */
        Singleton<T>::s_Instance = nullptr;
    }

private:
    /* 初期化フラグ */
    static std::once_flag s_InitFlag;
    /* シングルトンインスタンス */
    static T* s_Instance;
};

template <typename T> std::once_flag Singleton<T>::s_InitFlag;
template <typename T> T* Singleton<T>::s_Instance = nullptr;

