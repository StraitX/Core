#ifndef STRAITX_APPLICATION_HPP
#define STRAITX_APPLICATION_HPP

#include "core/os/events.hpp"
#include "core/math/vector2.hpp"
#include "core/result.hpp"

class Engine;

struct ApplicationConfig{
    const char *ApplicationName = "StraitX Game";
    Vector2u WindowSize = {1280, 720};
};

class Application{
public:
    Application() = default;

    virtual ~Application() = default;

    virtual Result OnInitialize();
    // return true if event was handled
    virtual bool OnEvent(const Event &event);

    virtual void OnFinalize();

    virtual void OnUpdate(float dt);
};

extern ApplicationConfig GetApplicationConfig();

#endif // STRAITX_APPLICATION_HPP