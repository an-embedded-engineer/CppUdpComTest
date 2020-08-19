#pragma once
#include <cstdint>
#include <vector>

/* アドレス型 */
using address_t = void*;

/* スタックトレース情報 */
struct StackTrace
{
    /* トレース数 */
    uint32_t trace_size;
    /* トレースアドレスリスト */
    std::vector<address_t> traces;
    /* トレースシンボルリスト */
    std::vector<std::string> symbols;
};

/* Stack Tracerクラス宣言 */
class StackTracer
{
public:
    /* スタックトレース情報取得 */
    static const StackTrace GetStackTrace();
};
