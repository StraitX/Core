#include "platform/windows/wgl.hpp"

namespace Windows {

wglCreateContextAttribsARBProc wglCreateContextAttribsARB = nullptr;

wglChoosePixelFormatARBProc wglChoosePixelFormatARB = nullptr;

wglGetPixelFormatAttribivARBProc wglGetPixelFormatAttribivARB = nullptr;

}//namespace Windows::