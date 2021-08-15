#ifndef STRAITX_MACOS_WINDOW_IMPL_HPP
#define STRAITX_MACOS_WINDOW_IMPL_HPP

#include "core/result.hpp"
#include "core/os/events.hpp"
#include "core/os/screen.hpp"
#include "core/function.hpp"
#include "core/noncopyable.hpp"

namespace MacOS{

class WindowImpl: public NonCopyable{
private:
    void *m_Handle = nullptr;
    void *m_View = nullptr;
    void *m_Delegate = nullptr;
    Function<void(const Event &)> m_EventsHandler;
    mutable Screen m_CurrentScreen;
public:
    Result Open(int width, int height, const char *title);

    void Close();

    bool IsOpen()const;

    void SetEventsHandler(Function<void(const Event &)> handler);

    Function<void(const Event &)> EventsHandler(){
        return m_EventsHandler;
    }

    void *Handle()const{
        return m_Handle;
    }

    void *View()const{
        return m_View;
    }

    void DispatchEvents();

    void SetTitle(const char *title);

    Vector2u Size()const;

    void SetSize(u32 width, u32 height);

    const Screen &CurrentScreen()const;
};

}//namespace MacOS::

#endif//STRAITX_MACOS_WINDOW_IMPL_HPP