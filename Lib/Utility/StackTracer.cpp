#include "StackTracer.h"
#include "CompileSwitch.h"
#include "StringFormat.h"

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

    /* シンボルハンドラの初期化 */
    SymInitialize(process, NULL, TRUE);

    /* スタックトレースの取得 */
    uint16_t trace_size = CaptureStackBackTrace(0, MaxSize, traces, NULL);

    /* シンボル名最大サイズをセット */
    constexpr size_t MaxNameSize = 255;
    /* シンボル情報サイズを算出 */
    constexpr size_t SymbolInfoSize = sizeof(SYMBOL_INFO) + ((MaxNameSize + 1) * sizeof(char));
    
    /* シンボル情報のメモリ確保 */
    SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(SymbolInfoSize, 1);

    /* スタックトレース情報生成 */
    StackTrace stack_trace;

    /* シンボル情報メモリ確保成功 */
    if (symbol != nullptr)
    {
        /* シンボル名最大サイズをセット */
        symbol->MaxNameLen = MaxNameSize;
        /* シンボル情報サイズをセット */
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        /* トレースサイズをセット */
        stack_trace.trace_size = (uint32_t)trace_size;
        /* トレースリストのメモリ確保 */
        stack_trace.traces.reserve((size_t)trace_size);
        /* シンボルリストのメモリ確保 */
        stack_trace.symbols.reserve((size_t)trace_size);

        /* トレースサイズ分ループ */
        for (uint16_t i = 0; i < trace_size; i++)
        {
            /* トレースアドレスからシンボル情報を取得 */
            SymFromAddr(process, (DWORD64)(traces[i]), 0, symbol);

            /* トレースアドレスをトレースリストに追加 */
            stack_trace.traces.push_back(traces[i]);

            /* シンボル名をシンボルリストに追加 */
            stack_trace.symbols.push_back(std::string(symbol->Name));
        }

        /* シンボル情報のメモリ解放 */
        free(symbol);
    }
    else
    {
        /* Nothing to do */
    }

    return stack_trace;
}

#elif TARGET_TYPE == TARGET_MAC || TARGET_TYPE == TARGET_LINUX

#include <execinfo.h>
#include <string>
#include <iostream>
#include <regex>

/* シンボルのデマングル有効化 */
#define DEMANGLE_SYMBOL_ENABLED     (1)

#if DEMANGLE_SYMBOL_ENABLED == 1
#include <cxxabi.h>
#include <typeinfo>

/* シンボル情報 */
struct SymbolInfo
{
    /* シンボル情報有効フラグ */
    bool is_valid;
    /* オブジェクト名 */
    std::string object_name;
    /* アドレス */
    std::string address;
    /* マングルされたシンボル名 */
    std::string mangled_symbol_name;
    /* オフセット */
    std::string offset;

    /* コンストラクタ */
    SymbolInfo()
        : is_valid(false)
        , object_name("")
        , address("")
        , mangled_symbol_name("")
        , offset("")
    {
    }
};

#if TARGET_TYPE == TARGET_MAC
inline SymbolInfo GetSymbolInfo(const std::string& raw_symbol_info)
{
    /* シンボル情報パターン ： <Depth> <ObjectName> <Address> <MangledSymbolName> + <Offset> */
    std::regex pattern("^(\\d+)\\s+([\\w.]+)\\s+(0x[0-9A-Fa-f]+)\\s+(\\w+)\\s+\\+\\s+(\\d+)$");
    std::smatch sm;

    /* シンボル情報生成 */
    SymbolInfo symbol_info;

    /* シンボル情報パターンにマッチ */
    if (std::regex_match(raw_symbol_info, sm, pattern))
    {
        /* 有効なシンボル情報生成 */
        symbol_info.object_name = sm[2].str();
        symbol_info.address = sm[3].str();
        symbol_info.mangled_symbol_name = sm[4].str();
        symbol_info.offset = sm[5].str();
        symbol_info.is_valid = true;
    }
    else
    {
        /* 無効なシンボル情報生成 */
        symbol_info.object_name = "";
        symbol_info.address = "";
        symbol_info.mangled_symbol_name = "";
        symbol_info.offset = "";
        symbol_info.is_valid = false;
    }

    return symbol_info;
}
#elif TARGET_TYPE == TARGET_LINUX
/* シンボル情報取得 */
inline SymbolInfo GetSymbolInfo(const std::string& raw_symbol_info)
{
    /* シンボル情報パターン ： <ObjectName>(<MangledSymbolName>+<Offset>) [<Address>] */
    std::regex pattern("^(.+)\\((\\w*)\\+(0x[0-9a-fA-F]+)\\)\\s+\\[(0x[0-9a-fA-F]+)\\]$");
    std::smatch sm;

    /* シンボル情報生成 */
    SymbolInfo symbol_info;

    /* シンボル情報パターンにマッチ */
    if (std::regex_match(raw_symbol_info, sm, pattern))
    {
        /* 有効なシンボル情報生成 */
        symbol_info.object_name = sm[1].str();
        symbol_info.mangled_symbol_name = sm[2].str();
        symbol_info.offset = sm[3].str();
        symbol_info.address = sm[4].str();
        symbol_info.is_valid = true;
    }
    else
    {
        /* 無効ななシンボル情報生成 */
        symbol_info.object_name = "";
        symbol_info.address = "";
        symbol_info.mangled_symbol_name = "";
        symbol_info.offset = "";
        symbol_info.is_valid = false;
    }

    return symbol_info;
}
#else
inline SymbolInfo GetSymbolInfo(const std::string& raw_symbol_info)
{
    /* 無効なシンボル情報を生成 */
    SymbolInfo symbol_info;
    symbol_info.object_name = "";
    symbol_info.address = "";
    symbol_info.mangled_symbol_name = "";
    symbol_info.offset = "";
    symbol_info.is_valid = false;
    return symbol_info;
}
#endif

/* シンボル情報を整形して取得 */
inline std::string GetSymbolInfoText(const std::string& raw_symbol_info)
{
    /* シンボル情報を取得 */
    const SymbolInfo symbol_info = GetSymbolInfo(raw_symbol_info);

    /* シンボル情報テキストを初期化 */
    std::string symbol_info_text = "";

    /* シンボル情報が有効 */
    if (symbol_info.is_valid == true)
    {
        /* マングルされたシンボル名を取得 */
        std::string mangled_symbol_name = symbol_info.mangled_symbol_name;

        /* マングルされたシンボル名を判定 */
        if (mangled_symbol_name[0] == '_')
        {
            /* デマングルされたシンボル名を取得 */
            int status = 0;
            char* demangled_symbol_name = abi::__cxa_demangle(mangled_symbol_name.c_str(), 0, 0, &status);

            /* デマングルに成功したらデマングルされたシンボル名を関数名にセット、失敗したらマングルされたシンボル名を使用 */
            std::string function_name = (status == 0) ? demangled_symbol_name : mangled_symbol_name;

            /* デマングルされたシンボル名のメモリ解放 */
            free(demangled_symbol_name);

            /* シンボル情報テキストをセット */
            symbol_info_text = StringFormat("%s %s + %s", symbol_info.object_name, function_name, symbol_info.offset);
        }
        else
        {
            /* シンボル情報テキストをセット */
            symbol_info_text = StringFormat("%s %s + %s", symbol_info.object_name, mangled_symbol_name, symbol_info.offset);
        }
    }
    else
    {
        /* シンボル情報テキストをセット */
        symbol_info_text = raw_symbol_info;
    }

    return symbol_info_text;
}

#endif

/* スタックトレース情報取得 */
const StackTrace StackTracer::GetStackTrace()
{
    /* 最大トレースサイズ */
    constexpr size_t MaxSize = 256;
    /* トレースリスト */
    void* traces[MaxSize] = {};

    /* スタックトレース取得 */
    int trace_size = backtrace(traces, MaxSize);
    /* シンボルリスト取得 */
    char** symbols = backtrace_symbols(traces, trace_size);

    /* スタックトレース情報生成 */
    StackTrace stack_trace;

    /* トレースサイズ  */
    stack_trace.trace_size = (uint32_t)trace_size;

    /* トレースリストメモリ確保 */
    stack_trace.traces.reserve(trace_size);
    /* シンボルリストメモリ確保 */
    stack_trace.symbols.reserve(trace_size);

    /* トレースサイズ分ループ */
    for (int i = 0; i < trace_size; i++)
    {
        /* トレースアドレスをリストに追加 */
        stack_trace.traces.push_back(traces[i]);

        /* シンボル情報を取得 */
        std::string raw_symbol_info = symbols[i];

#if DEMANGLE_SYMBOL_ENABLED == 1
        /* シンボル情報を整形して取得 */
        std::string symbol_info = GetSymbolInfoText(raw_symbol_info);
#else
        /* シンボル情報をそのまま取得 */
        std::string symbol_info = raw_symbol_info;
#endif

        /* シンボル情報をシンボルリストに追加 */
        stack_trace.symbols.push_back(symbol_info);
    }

    /* シンボルリスト解放 */
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
