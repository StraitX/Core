#ifndef STRAITX_LINUX_WINDOW_IMPL_HPP
#define STRAITX_LINUX_WINDOW_IMPL_HPP

#include "core/os/events.hpp"
#include "core/result.hpp"
#include "core/noncopyable.hpp"
#include "core/os/screen.hpp"

namespace X11{
struct _XIM;
struct _XIC;
struct __GLXFBConfigRec;
}//namespace X11::

namespace Linux{

struct WindowImpl: NonCopyable{
    unsigned long Handle = 0;
    X11::__GLXFBConfigRec *FBConfig = nullptr;
	int Width = 0;
	int Height = 0;
	X11::_XIM *InputMethod = nullptr;
	X11::_XIC *InputContext = 0;
	PlatformScreen CurrentScreen;

	static WindowImpl s_MainWindow;

    WindowImpl() = default;

    Result Open(int width, int height);

    Result Close();

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    Vector2u Size()const;

	const PlatformScreen &Screen();

	static Vector2u GetSizeFromHandle(unsigned long handle);

    static X11::__GLXFBConfigRec *PickBestFBConfig(int screen_index);
};

}//namespace Linux::

#endif // STRAITX_LINUX_WINDOW_IMPL_HPP