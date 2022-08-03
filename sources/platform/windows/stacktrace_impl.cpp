#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdlib>
#include <cstdio>
#pragma warning(push)
#pragma warning(disable : 4091)
#include <DbgHelp.h>
#pragma warning(pop)
#include "core/os/stacktrace.hpp"
#include "core/printer.hpp"


void Stacktrace::Capture(void *(&frames)[MaxFramesCount], size_t &captured_frames){
    captured_frames = CaptureStackBackTrace(1, MaxFramesCount, frames, nullptr);
}

void Printer<Stacktrace>::Print(const Stacktrace &value, StringWriter &writer){

    HANDLE process = GetCurrentProcess();

    SymInitialize(process, NULL, TRUE);

    SYMBOL_INFO *symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    for(size_t i = 1 + value.m_SkipFrames; i < value.m_CapturedFrames; i++) {
        SymFromAddr(process, (DWORD64)(value.m_FramePointers[i]), 0, symbol);

        char buffer[1024];
        size_t offset = (byte*)value.m_FramePointers[i] - (byte*)symbol->Address;
        snprintf(buffer, sizeof(buffer),"%-3zi [0x%0llX] %s + %zi", i - value.m_SkipFrames, symbol->Address, symbol->Name, offset);
        Printer<char>::Print('\n', writer);
        Printer<const char*>::Print(buffer, writer);
    }
    free(symbol);
}
