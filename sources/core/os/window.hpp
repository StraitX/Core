#ifndef STRAITX_WINDOW_HPP
#define STRAITX_WINDOW_HPP

#include "core/env/os.hpp"
#include "core/os/events.hpp"
#include "core/os/screen.hpp"
#include "core/type_traits.hpp"
#include "core/function.hpp"

#if defined(SX_OS_LINUX)
    #include "platform/linux/window_impl.hpp"
    typedef Linux::WindowImpl OSWindowImpl;
#elif defined(SX_OS_WINDOWS)
    #include "platform/windows/window_impl.hpp"
    typedef Windows::WindowImpl OSWindowImpl;
#elif defined(SX_OS_MACOS)
    #include "platform/macos/window_impl.hpp"
    typedef MacOS::WindowImpl OSWindowImpl;
#else
    #error "Your platform does not support window creation"
#endif

//Window has two EventHandlers, first for a user, second
class Window: private OSWindowImpl{
private:
	using Super = OSWindowImpl;
public:
	//static Window Main;

	Result Open(u32 width, u32 height, const char *title);

	void Close();

	bool IsOpen()const;

	void SetEventHandler(Function<void(const Event &e)> handler);

	void SetTitle(const char *title);

	Vector2u Size() const;

	void SetSize(u32 width, u32 height);

	void SetSize(Vector2u size);

	const PlatformScreen &Screen()const;

	const OSWindowImpl &Impl()const;
};

SX_INLINE Result Window::Open(u32 width, u32 height, const char *title){
	return Super::Open(width, height, title);
}

SX_INLINE void Window::Close(){
	Super::Close();
}

SX_INLINE bool Window::IsOpen()const{
	return Super::IsOpen();
}

SX_INLINE void Window::SetTitle(const char *title){
	Super::SetTitle(title);
}

SX_INLINE Vector2u Window::Size()const{
	return Super::Size();
}

SX_INLINE void Window::SetSize(u32 width, u32 height){
	Super::SetSize(width, height);
}

SX_INLINE void Window::SetSize(Vector2u size){
	Super::SetSize(size.x, size.y);
}

SX_INLINE const PlatformScreen &Window::Screen()const{
	return Super::Screen();
}

SX_INLINE const OSWindowImpl &Window::Impl()const{
	return *this;
}

#endif // STRAITX_WINDOW_HPP