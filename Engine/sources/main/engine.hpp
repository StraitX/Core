#ifndef STRAITX_ENGINE_HPP
#define STRAITX_ENGINE_HPP

#include "platform/error.hpp"

namespace StraitX{

class Application;

class Engine{
private:
    Application *mApplication;
public:
    Engine();
    ~Engine();

    Error Run();
private:
    Error Initialize();
    Error Finalize();
    void MainLoop();
};

}; // namespace StraitX::


#endif // STRAITX_ENGINE_HPP