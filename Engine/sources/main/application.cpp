#include "main/application.hpp"

namespace StraitX{

Application::Application(const char *name):
    m_Name(name)
{}

void Application::Stop(){
    m_Engine->Stop();
}


void Application::SetEngine(Engine *engine){
    m_Engine = engine;
}

const char *Application::Name(){
    return m_Name;
}

}; // namespace StraitX::