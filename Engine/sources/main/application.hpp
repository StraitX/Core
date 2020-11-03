#ifndef STRAITX_APPLICATION_HPP
#define STRAITX_APPLICATION_HPP

#include "main/engine.hpp"

namespace StraitX{

class Application{
private:
    Engine *mEngine;
    friend class Engine;
public:
    Application() = default;
    virtual ~Application() = default;

    virtual void OnInitialize(){};

    virtual void OnFinalize(){};

    virtual void OnUpdate(){};

    void Stop();

private:
    void SetEngine(Engine *engine);

};

}; // namespace StraitX::


#endif // STRAITX_APPLICATION_HPP