#ifndef STRAITX_STACKTRACE_HPP
#define STRAITX_STACKTRACE_HPP

#include <core/printer.hpp>
#include <core/span.hpp>

//XXX: Gonna be slow for now
class Stacktrace{
    static constexpr size_t MaxFramesCount = 256;
private:
    void *m_FramePointers[MaxFramesCount];
    size_t m_CapturedFrames = 0;
    size_t m_SkipFrames = 0;

    friend class Printer<Stacktrace>;
public:
    Stacktrace(size_t skip_frames = 0):
        m_SkipFrames(skip_frames)
    {
        Capture(m_FramePointers, m_CapturedFrames);
    }
private:
    //implemented in platform layer
    static void Capture(void *(&frames)[MaxFramesCount], size_t &captured_frames);
};

//implemented in platform layer
template<>
struct Printer<Stacktrace>{
    static void Print(const Stacktrace &value, StringWriter &writer);
};

#endif//STRAITX_STACKTRACE_HPP