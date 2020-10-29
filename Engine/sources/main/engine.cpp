#include "main/engine.hpp"
#include "main/application.hpp"
#include "platform/platform.hpp"


// should be defined at client side
extern StraitX::Application *StraitXMain();
extern StraitX::Error StraitXExit(StraitX::Application *);

namespace StraitX{


Engine::Engine(){

}

Engine::~Engine(){

}


Error Engine::Run(){
    Initialize();
    MainLoop();
    Finalize();
    // for now
    return 0;
}

Error Engine::Initialize(){
    Platform::Initialize();

    //Engine should be completely initialized at this moment
    mApplication = StraitXMain();

    mApplication->OnInitialize();

}

Error Engine::Finalize(){
    mApplication->OnFinalize();

    StraitXExit(mApplication);

    Platform::Finalize();
}

void Engine::MainLoop(){
    for(;;){
        mApplication->OnUpdate();
    }
}

}; // namespace StraitX::
