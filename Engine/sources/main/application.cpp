#include "main/application.hpp"

namespace StraitX{



void Application::Stop(){
    mEngine->Stop();
}


void Application::SetEngine(Engine *engine){
    mEngine = engine;
}


}; // namespace StraitX::