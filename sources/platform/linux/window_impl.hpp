#ifndef STRAITX_LINUX_WINDOW_IMPL_HPP
#define STRAITX_LINUX_WINDOW_IMPL_HPP

#include "core/os/events.hpp"
#include "core/result.hpp"
#include "core/noncopyable.hpp"
#include "core/os/screen.hpp"
#include "core/function.hpp"

namespace X11{
struct _XIM;
struct _XIC;
struct __GLXFBConfigRec;
}//namespace X11::

namespace Linux{

class WindowImpl: NonCopyable{
private:
    unsigned long m_Handle = 0;
    X11::__GLXFBConfigRec *m_FBConfig = nullptr;
	int m_Width = 0;
	int m_Height = 0;
	X11::_XIM *m_InputMethod = nullptr;
	X11::_XIC *m_InputContext = 0;
    Function<void(const Event &e)> m_EventsHandler;
	mutable Screen m_CurrentScreen;
public:
    WindowImpl() = default;

    Result Open(int width, int height, const char *title);

    Result Close();

    bool IsOpen()const;

    void SetEventsHandler(Function<void(const Event &e)> handler);

    void DispatchEvents();

    void SetTitle(const char *title);

    void SetSize(int width, int height);

    Vector2u Size()const;

    unsigned long Handle()const{
        return m_Handle;
    }

    X11::__GLXFBConfigRec *FBConfig()const{
        return m_FBConfig;
    }

	const Screen &CurrentScreen()const;

	static Vector2u GetSizeFromHandle(unsigned long handle);

    static X11::__GLXFBConfigRec *PickBestFBConfig(int screen_index);
};

}//namespace Linux::

#endif // STRAITX_LINUX_WINDOW_IMPL_HPP