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

class Window: private OSWindowImpl{
private:
	using Super = OSWindowImpl;
	bool m_IsFocused = true;
	Function<void(const Event& e)> m_EventsHandler;
public:
	Window(u32 width, u32 height, StringView title);

	~Window();

	Result Open(u32 width, u32 height, StringView title);

	void Close();

	bool IsOpen()const;

	bool IsFocused()const;

	void SetEventsHandler(Function<void(const Event &e)> handler);

	void DispatchEvents();

	void SetTitle(StringView title);

	Vector2u Size() const;

	void SetSize(u32 width, u32 height);

	void SetSize(Vector2u size);

	const Screen &CurrentScreen()const;

	const OSWindowImpl &Impl()const;
private:
	void OnEvent(const Event& e);
};

SX_INLINE Window::Window(u32 width, u32 height, StringView title) {
	(void)Open(width, height, title);

	Super::SetEventsHandler({this, &Window::OnEvent});
}

SX_INLINE Window::~Window() {
	if(IsOpen())
		Close();
}

SX_INLINE Result Window::Open(u32 width, u32 height, StringView title){
	return Super::Open(width, height, title);
}

SX_INLINE void Window::Close(){
	Super::Close();
}

SX_INLINE bool Window::IsOpen()const{
	return Super::IsOpen();
}
SX_INLINE bool Window::IsFocused()const {
	return m_IsFocused;
}

SX_INLINE void Window::SetEventsHandler(Function<void(const Event& e)> handler) {
	m_EventsHandler = handler;
}

SX_INLINE void Window::DispatchEvents(){
	Super::DispatchEvents();
}

SX_INLINE void Window::SetTitle(StringView title){
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

SX_INLINE const Screen &Window::CurrentScreen()const{
	return Super::CurrentScreen();
}

SX_INLINE const OSWindowImpl &Window::Impl()const{
	return *this;
}

SX_INLINE void Window::OnEvent(const Event& e) {
	if (e.Type == EventType::FocusIn)
		m_IsFocused = true;

	if (e.Type == EventType::FocusOut)
		m_IsFocused = false;

	m_EventsHandler.TryCall(e);
}

#endif // STRAITX_WINDOW_HPP