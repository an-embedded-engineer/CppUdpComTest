#pragma once
#include <cstddef>
#include <functional>

/* �N���X�O���錾 */
class CancellableThread;
class CancellationPoint;

/* �X���b�hID�^ */
using thread_id_t = size_t;

/* �X���b�h���s�֐��^ */
using ThreadFuncType = std::function<void(CancellableThread& thread, CancellationPoint&)>;

