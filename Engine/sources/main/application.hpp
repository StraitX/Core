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

    virtual Error OnInitialize(){return ErrorCode::Success;};

    virtual Error OnFinalize(){return ErrorCode::Success;};

    virtual void OnUpdate(){};

    void Stop();

    const char *Name();

private:
    void SetEngine(Engine *engine);

};

}; // namespace StraitX::


#endif // STRAITX_APPLICATION_HPP