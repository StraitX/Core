#ifndef STRAITX_LINUX_WINDOW_IMPL_HPP
#define STRAITX_LINUX_WINDOW_IMPL_HPP

#include "platform/events.hpp"
#include "platform/result.hpp"
#include "platform/noncopyable.hpp"
#include "platform/screen.hpp"

struct _XIM;
struct _XIC;
struct __GLXFBConfigRec;

namespace Linux{

struct WindowImpl: NonCopyable{
    unsigned long Handle = 0;
    struct __GLXFBConfigRec *FBConfig = nullptr;
	int Width = 0;
	int Height = 0;
	struct _XIM *InputMethod = nullptr;
	struct _XIC *InputContext = 0;
	PlatformScreen CurrentScreen;

	static WindowImpl s_MainWindow;

    WindowImpl() = default;

    Result Open(int width, int height);

    Result Close();

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    Size2u Size()const;

	const PlatformScreen &Screen();

	static Size2u GetSizeFromHandle(unsigned long handle);

    static __GLXFBConfigRec *PickBestFBConfig(int screen_index);
};

}//namespace Linux::

#endif // STRAITX_LINUX_WINDOW_IMPL_HPP