#include <iostream>
#include "CompileSwitch.h"
#include "StringFormat.h"
#include "AppException.h"

static void StringFormatTest();
static void ExceptionTest();

static void TestFunc1();
static void TestFunc2();

int main()
{
    try
    {
        StringFormatTest();

        std::cout << std::endl;

        ExceptionTest();
    }
    catch (std::exception& ex)
    {
        std::cerr << StringFormat("[ERROR] %s", ex.what()) << std::endl;
    }
}

static void StringFormatTest()
{
    std::string text1 = "Test Message1";

    const char* text2 = "Test Message 2";

    int value = 256;

    std::cout << StringFormat("%s | %s | 0x%x = %d", text1, text2, value, value) << std::endl;
}

static void ExceptionTest()
{
    TestFunc1();
}


static void TestFunc1()
{
    TestFunc2();
}

static void TestFunc2()
{
    THROW_APP_EXCEPTION("Invoke Application Exception");
}
