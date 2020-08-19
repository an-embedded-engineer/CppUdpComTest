#include "StackTracer.h"
#include "CompileSwitch.h"

#if TARGET_TYPE == TARGET_MSVC
#include <Windows.h>
#include <ImageHlp.h>

#include <string>

#pragma comment(lib, "imagehlp.lib")

/* スタックトレース情報取得 */
const StackTrace StackTracer::GetStackTrace()
{
    /* 最大トレースサイズ */
    constexpr size_t MaxSize = 256;
    /* トレースリスト */
    void* traces[MaxSize] = {};

    /* 現在のプロセスを取得 */
    HANDLE process = GetCurrentProcess();

    SymInitialize(process, NULL, TRUE);

    uint16_t trace_size = CaptureStackBackTrace(0, MaxSize, traces, NULL);

    constexpr size_t MaxNameSize = 255;
    constexpr size_t SymbolInfoSize = sizeof(SYMBOL_INFO) + ((MaxNameSize + 1) * sizeof(char));

    SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(SymbolInfoSize, 1);

    /* スタックトレース情報生成 */
    StackTrace stack_trace;

    if (symbol != nullptr)
    {
        symbol->MaxNameLen = MaxNameSize;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        stack_trace.trace_size = (uint32_t)trace_size;
        stack_trace.traces.reserve((size_t)trace_size);
        stack_trace.symbols.reserve((size_t)trace_size);

        for (uint16_t i = 0; i < trace_size; i++)
        {
            SymFromAddr(process, (DWORD64)(traces[i]), 0, symbol);

            stack_trace.traces.push_back(traces[i]);
            stack_trace.symbols.push_back(std::string(symbol->Name));
        }
    }

    return stack_trace;
}

#elif TARGET_TYPE == TARGET_MAC || TARGET_TYPE == TARGET_LINUX

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

    /* スタックトレース&シンボル名取得 */
    int trace_size = backtrace(traces, MaxSize);
    symbols = backtrace_symbols(traces, trace_size);

    /* スタックトレース情報生成 */
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

#else

/* スタックトレース情報取得 */
const StackTrace StackTracer::GetStackTrace()
{
    /* 空のスタックトレース情報生成 */
    StackTrace stack_trace;
    stack_trace.trace_size = 0;
    staci_trace.traces.clear();
    stack_trace.symbols.clear();

    return stack_trace;
}
#endif
