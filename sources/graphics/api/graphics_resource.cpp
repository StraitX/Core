#include "graphics/api/graphics_resource.hpp"
#include "graphics/api/gpu.hpp"
#include "graphics/api/graphics_api.hpp"

size_t GraphicsResource::s_GraphicsResourceCount = 0;

GraphicsResource::GraphicsResource() {
	if (s_GraphicsResourceCount++ == 0)
		GraphicsAPI::CreateBackend() && GPU::Initialize();
}

GraphicsResource::GraphicsResource(GraphicsResource&& other)noexcept{
	SX_ASSERT(false);
}

GraphicsResource::~GraphicsResource() {
	if (s_GraphicsResourceCount-- == 0)
		GPU::Finalize();
}

GraphicsResource& GraphicsResource::operator=(GraphicsResource&& other) noexcept{
	SX_ASSERT(false);
	return *this;
}
