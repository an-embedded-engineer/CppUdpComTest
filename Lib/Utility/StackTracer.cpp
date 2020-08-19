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

        free(symbol);
    }

    return stack_trace;
}

#elif TARGET_TYPE == TARGET_MAC || TARGET_TYPE == TARGET_LINUX

#include <execinfo.h>
#include <string>
#include <iostream>
#include <regex>

#define DEMANGLE_SYMBOL_ENABLED     (1)

#if DEMANGLE_SYMBOL_ENABLED == 1
#include <cxxabi.h>
#include <typeinfo>

inline std::string GetRawFunctionName(const std::string& raw_symbol_info)
{
    std::regex pattern("^(\\d+)\\s+([\\w.]+)\\s+(0x[0-9A-Fa-f]+)\\s(\\w+)\\s(.+)$");
    std::smatch sm;

    if (std::regex_match(raw_symbol_info, sm, pattern))
    {
        std::string raw_function_name = sm[4].str();

        return raw_function_name;
    }
    else
    {
        return "";
    }
}
#endif

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

        std::string raw_symbol_info = symbols[i];

#if DEMANGLE_SYMBOL_ENABLED == 1
        std::string symbol_info = "";

        std::string raw_function_name = GetRawFunctionName(raw_symbol_info);

        if (raw_function_name != "")
        {
            std::cout << raw_function_name << std::endl;

            if (raw_function_name[0] == '_')
            {
                int status = 0;
                char* demangled = abi::__cxa_demangle(raw_function_name.c_str(), 0, 0, &status);

                std::string demangled_function = demangled;

                free(demangled);

                symbol_info = demangled_function;
            }
            else
            {
                symbol_info = raw_function_name;
            }

        }
        else
        {
            symbol_info = raw_symbol_info;
        }

#else
        std::string symbol_info = raw_symbol_info;
#endif

        stack_trace.symbols.push_back(symbol_info);
    }

    free(symbols);

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
