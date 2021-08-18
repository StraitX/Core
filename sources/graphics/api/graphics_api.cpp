#include "graphics/api/graphics_api.hpp"

GraphicsAPIBackendImpl *GraphicsAPI::s_Impl = nullptr;
GraphicsAPIBackend GraphicsAPI::s_Backend = GraphicsAPIBackend::None;

Result GraphicsAPI::CreateBackend(GraphicsAPIBackend backend){
    if(s_Impl)
        return s_Impl->Create();
    
    return Result::Unsupported;
}

void GraphicsAPI::DestroyBackend(){
    if(s_Impl)
        s_Impl->Destroy();
    s_Impl = nullptr;
}