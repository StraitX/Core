#include "core/log.hpp"
#include "main/application.hpp"
#include "main/engine.hpp"

namespace StraitX{

void Application::Stop(){
    m_Engine->Stop();
}


void Application::SetEngine(Engine *engine){
    LogInfo("Application: Engine was bound");
    m_Engine = engine;
}

}; // namespace StraitX::