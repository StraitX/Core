#include "guarded_main.hpp"
#include "core/span.hpp"
#include "graphics/api/gpu.hpp"
#include "graphics/api/graphics_api.hpp"

extern int StraitXMain(Span<const char *> args);

int GuardedMain(int argc, const char **argv){

    if(!GraphicsAPI::CreateBackend(GraphicsAPIBackend::Vulkan)
    || !GPU::Initialize())
        return -1;

    int sx_main_result = StraitXMain({argv, (size_t)argc});

    GPU::Finalize();

    return sx_main_result;
}