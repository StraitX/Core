#include <execinfo.h>
#include <cstdio>
#include <cstdlib>
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include "core/os/stacktrace.hpp"

void Stacktrace::Capture(void *(&frames)[MaxFramesCount], size_t &captured_frames){
    captured_frames = backtrace(frames, MaxFramesCount);
}

void Printer<Stacktrace>::Print(const Stacktrace &value, void (*writer)(char, void*), void *writer_data){
    char **symbols = backtrace_symbols(value.m_FramePointers, value.m_CapturedFrames);
    char buf[2048];
    for(int i = 1 + value.m_SkipFrames; i < value.m_CapturedFrames; i++) {
        Printer<char>::Print('\n', writer, writer_data);

        Dl_info info;
        if(dladdr(value.m_FramePointers[i], &info) && info.dli_sname){
            char *demangled = NULL;

            int status = -1;
            if (info.dli_sname[0] == '_')
                demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);

            const char *proc_name = (status == 0 ? demangled : (info.dli_sname == 0 ? symbols[i] : info.dli_sname));
            size_t offset = (u8 *)value.m_FramePointers[i] - (u8 *)info.dli_saddr;
            const char *filename = info.dli_fname;
            void *frame_address = value.m_FramePointers[i];

            snprintf(buf, sizeof(buf), "%-3lu 0x%016lx %s :: %s + %zd", i - value.m_SkipFrames, (size_t)frame_address, filename, proc_name, offset);

            free(demangled);
        }else{
            snprintf(buf, sizeof(buf), "%-3lu 0x%016lx %s", i - value.m_SkipFrames, (size_t)value.m_FramePointers[i], symbols[i]);
        }
        Printer<const char*>::Print(buf, writer, writer_data);
        
    }
    free(symbols);
}
