#include "guarded_main.hpp"
#include "core/span.hpp"
#include "graphics/api/gpu.hpp"
#include "graphics/api/graphics_api.hpp"

extern int StraitXMain(Span<const char *> args);

int GuardedMain(int argc, const char **argv){
    return StraitXMain({argv, (size_t)argc});
}