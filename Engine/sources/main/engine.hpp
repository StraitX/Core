#ifndef STRAITX_ENGINE_HPP
#define STRAITX_ENGINE_HPP

#include "platform/error.hpp"
#include "platform/window.hpp"


namespace StraitX{

class Application;

class Engine{
private:
    Application *m_Application;
    bool m_Running;
    Display &m_Display;
    Window m_Window;
    Error m_ErrorWindow, m_ErrorDisplay, m_ErrorApplication, m_ErrorMX;
public:
    Engine();
    ~Engine();

    int Run();

    void Stop();
    
private:
    Error Initialize();
    Error Finalize();
    void MainLoop();
};

}; // namespace StraitX::


#endif // STRAITX_ENGINE_HPP