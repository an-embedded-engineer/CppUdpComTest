#include "StackTracer.h"

#include <execinfo.h>
#include <string>

/* スタックトレース情報取得 */
const StackTrace StackTracer::GetStackTrace()
{
    /* 最大トレースサイズ */
    constexpr size_t MaxSize = 256;
    /* トレースリスト */
    void* traces[MaxSize] = {};
    /* シンボルリスト */
    char** symbols = {};

    int trace_size = backtrace(traces, MaxSize);
    symbols = backtrace_symbols(traces, trace_size);

    StackTrace stack_trace;
    stack_trace.trace_size = (uint32_t)trace_size;
    stack_trace.traces.reserve(trace_size);
    stack_trace.symbols.reserve(trace_size);
    for (int i = 0; i < trace_size; i++)
    {
        stack_trace.traces.push_back(traces[i]);
        stack_trace.symbols.push_back(std::string(symbols[i]));
    }

    return stack_trace;
}

