#ifndef STRAITX_ENGINE_HPP
#define STRAITX_ENGINE_HPP

#include "platform/error.hpp"

namespace StraitX{

class Application;

class Engine{
private:
    Application *mApplication;
    bool mRunning;
public:
    Engine();
    ~Engine();

    Error Run();

    void Stop();
    
private:
    Error Initialize();
    Error Finalize();
    void MainLoop();
};

}; // namespace StraitX::


#endif // STRAITX_ENGINE_HPP