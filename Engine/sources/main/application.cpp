#include "core/log.hpp"
#include "main/application.hpp"
#include "main/engine.hpp"

namespace StraitX{

Result Application::OnInitialize(){
    return Result::Success;
}
// return true if event was handled
bool Application::OnEvent(const Event &event){
    (void)event;
    return false;
}

void Application::OnFinalize(){
    (void)0;
}

void Application::OnUpdate(float dt){
    (void)dt;
}

void Application::Stop(){
    m_Engine->Stop();
}

void Application::SetEngine(Engine *engine){
    LogInfo("Application: Engine was bound");
    m_Engine = engine;
}

}; // namespace StraitX::