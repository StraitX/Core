#include "platform/windows/wgl.hpp"

namespace StraitX {
namespace Windows {

wglCreateContextAttribsARBProc wglCreateContextAttribsARB = nullptr;
wglChoosePixelFormatARBProc wglChoosePixelFormatARB = nullptr;
wglGetPixelFormatAttribivARBProc wglGetPixelFormatAttribivARB = nullptr;

}//namespace Windows::
}//namespace StraitX::