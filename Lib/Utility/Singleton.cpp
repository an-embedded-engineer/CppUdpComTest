#include "Singleton.h"

/* �~���[�e�b�N�X */
std::mutex SingletonFinalizer::s_Mutex;

/* �t�@�C�i���C�Y�֐����X�g */
std::deque<SingletonFinalizer::FinalizerFunc> SingletonFinalizer::s_Finalizers;

/* �t�@�C�i���C�Y�֐��ǉ� */
void SingletonFinalizer::AddFinalizer(SingletonFinalizer::FinalizerFunc func)
{
    /* �~���[�e�b�N�X�ɂ��r������ */
    std::lock_guard<std::mutex> lock(SingletonFinalizer::s_Mutex);

    /* ���X�g�̐擪�Ƀt�@�C�i���C�Y�֐���ǉ� */
    SingletonFinalizer::s_Finalizers.push_front(func);
}

/* �V���O���g���t�@�C�i���C�Y */
void SingletonFinalizer::Finalize()
{
    /* �~���[�e�b�N�X�ɂ��r������ */
    std::lock_guard<std::mutex> lock(SingletonFinalizer::s_Mutex);

    /* �t�@�C�i���C�Y�֐������ԂɌĂяo�� */
    for (const auto& func : SingletonFinalizer::s_Finalizers)
    {
        func();
    }

    /* �t�@�C�i���C�Y�֐����X�g���N���A */
    SingletonFinalizer::s_Finalizers.clear();
}
