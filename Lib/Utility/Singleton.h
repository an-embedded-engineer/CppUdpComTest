#pragma once

#include <cassert>
#include <deque>
#include <mutex>

/* Singleton Finalizer�N���X�錾 */
class SingletonFinalizer
{
public:
    /* �V���O���g���t�@�C�i���C�Y�֐��^��` */
    using FinalizerFunc = void(*)();

public:
    /* �t�@�C�i���C�Y�֐��ǉ� */
    static void AddFinalizer(FinalizerFunc func);

    /* �V���O���g���t�@�C�i���C�Y */
    static void Finalize();

private:
    /* �~���[�e�b�N�X */
    static std::mutex s_Mutex;

    /* �t�@�C�i���C�Y�֐����X�g */
    static std::deque<SingletonFinalizer::FinalizerFunc> s_Finalizers;
};

/* Singleton�e���v���[�g�N���X�錾 */
template <typename T>
class Singleton final
{
public:
    /* �V���O���g���C���X�^���X�Q�b�g */
    static T& GetInstance()
    {
        /* 1�x�����C���X�^���X�������\�b�h���Ăяo�� */
        std::call_once(Singleton<T>::s_InitFlag, Singleton<T>::Create);
        
        /* �C���X�^���X�����m�F */
        assert(Singleton<T>::s_Instance);
        
        /* �V���O���g���C���X�^���X��Ԃ� */
        return *Singleton<T>::s_Instance;
    }

private:
    /* �V���O���g���C���X�^���X���� */
    static void Create()
    {
        /* �V���O���g���C���X�^���X���� */
        Singleton<T>::s_Instance = new T;
        
        /* Singleton Finalizer�ɃV���O���g���C���X�^���X�j�󃁃\�b�h��o�^ */
        SingletonFinalizer::AddFinalizer(&Singleton<T>::Destroy);
    }

    /* �V���O���g���C���X�^���X�j�� */
    static void Destroy()
    {
        /* �V���O���g���C���X�^���X�폜 */
        delete Singleton<T>::s_Instance;

        /* �V���O���g���C���X�^���X�A�h���X�N���A */
        Singleton<T>::s_Instance = nullptr;
    }

private:
    /* �������t���O */
    static std::once_flag s_InitFlag;
    /* �V���O���g���C���X�^���X */
    static T* s_Instance;
};

template <typename T> std::once_flag Singleton<T>::s_InitFlag;
template <typename T> T* Singleton<T>::s_Instance = nullptr;

