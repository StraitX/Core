#include "main/engine.hpp"
#include "main/application.hpp"
#include "platform/platform.hpp"
#include "platform/io.hpp"

// should be defined at client side
extern StraitX::Application *StraitXMain();
extern StraitX::Error StraitXExit(StraitX::Application *);

namespace StraitX{


Engine::Engine(){

}

Engine::~Engine(){

}


Error Engine::Run(){
    Output::Print("Engine: Initialize: Started");
    if(Initialize()==ErrorCode::Success)
        Output::Print("Engine::Initialize: Success");
    else{
        Output::Print("Engine::Initialize: Failed");
        return ErrorCode::Failure;
    }
    Output::Print("Engine: Starting main loop");
    MainLoop();
    Output::Print("Engine: Finalize: Started");
    if(Finalize()==ErrorCode::Success){
        Output::Print("Engine::Finalize: Success");
        return ErrorCode::Success;
    }else{
        Output::Print("Engine::Finalize: Failed");
        return ErrorCode::Failure;
    }
}

Error Engine::Initialize(){
    Error initCode = Platform::Initialize();

    //Engine should be completely initialized at this moment
    mApplication = StraitXMain();
    if(mApplication == nullptr)
        return ErrorCode::Failure;

    mApplication->OnInitialize();
    return initCode;
}

Error Engine::Finalize(){
    mApplication->OnFinalize();

    StraitXExit(mApplication);

    return Platform::Finalize();
}

void Engine::MainLoop(){
    for(;;){
        mApplication->OnUpdate();
    }
}

}; // namespace StraitX::
