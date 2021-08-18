#include "graphics/api/graphics_api.hpp"

GraphicsAPIBackendImpl *GraphicsAPI::s_Backend = nullptr;

Result GraphicsAPI::CreateBackend(GraphicsAPIBackend backend){
    if(s_Backend)
        return s_Backend->Create();
    
    return Result::Unsupported;
}

void GraphicsAPI::DestroyBackend(){
    if(s_Backend)
        s_Backend->Destroy();
    s_Backend = nullptr;
}