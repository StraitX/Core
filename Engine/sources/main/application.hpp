#ifndef STRAITX_APPLICATION_HPP
#define STRAITX_APPLICATION_HPP

#include "main/engine.hpp"

namespace StraitX{

class Application{
private:
    Engine *m_Engine;
    const char *m_Name;
    friend class Engine;
public:
    Application(const char *name);
    virtual ~Application() = default;

    virtual Result OnInitialize(){return Result::Success;};

    virtual Result OnFinalize(){return Result::Success;};

    virtual void OnUpdate(){};

    void Stop();

    const char *Name();

private:
    void SetEngine(Engine *engine);

};

}; // namespace StraitX::


#endif // STRAITX_APPLICATION_HPP