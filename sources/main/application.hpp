#ifndef STRAITX_APPLICATION_HPP
#define STRAITX_APPLICATION_HPP

#include "core/os/events.hpp"
#include "core/math/vector2.hpp"
#include "graphics/graphics_context.hpp"

class Engine;

struct ApplicationConfig{
    const char *ApplicationName = "StraitX Game";
    GraphicsAPI DesiredAPI = GraphicsAPI::OpenGL;
    Vector2u WindowSize{1280, 720};
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

    void Stop();
};

extern ApplicationConfig GetApplicationConfig();

#endif // STRAITX_APPLICATION_HPP