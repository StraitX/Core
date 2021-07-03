#include "core/log.hpp"
#include "main/application.hpp"
#include "main/engine.hpp"

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
    Engine::Get().Stop();
}