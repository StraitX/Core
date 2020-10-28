#include "main/engine.hpp"
#include "main/application.hpp"
// should be defined at client side
extern StraitX::Application *StraitXMain();
extern int StraitXExit(StraitX::Application *);

namespace StraitX{


Engine::Engine(){

}

Engine::~Engine(){

}


int Engine::Run(){
    Initialize();
    MainLoop();
    Finalize();
    // for now
    return 0;
}

int Engine::Initialize(){

    //Engine should be completely initialized at this moment
    mApplication = StraitXMain();

    mApplication->OnInitialize();

}

int Engine::Finalize(){
    mApplication->OnFinalize();

    StraitXExit(mApplication);
}

void Engine::MainLoop(){
    //for(;;)
}

}; // namespace StraitX::
