#ifndef STRAITX_ENGINE_HPP
#define STRAITX_ENGINE_HPP

#include "platform/result.hpp"
#include "servers/display_server.hpp"
#include "graphics/vulkan/renderer_api.hpp"

namespace StraitX{

class Application;

class Engine{
private:
    Application *m_Application;
    bool m_Running;
    DisplayServer m_DisplayServer;
    Result m_ErrorRendererHW, m_ErrorRendererAPI;
    Result m_ErrorDisplayServer, m_ErrorApplication, m_ErrorMX;
public:
    Engine();
    ~Engine();

    int Run();

    void Stop();
    
private:
    Result Initialize();
    Result Finalize();
    void MainLoop();
};

}; // namespace StraitX::


#endif // STRAITX_ENGINE_HPP