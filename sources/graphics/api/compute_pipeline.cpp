#include "core/os/vulkan.hpp"
#include "graphics/api/compute_pipeline.hpp"
#include "graphics/api/graphics_api.hpp"
#ifdef SX_VULKAN_SUPPORTED
    #include "graphics/api/vulkan/compute_pipeline_impl.hpp"
#endif

ComputePipeline* ComputePipeline::Create(const ComputePipelineProperties& props)
{
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::ComputePipelineImpl(props);
#endif
	return nullptr;
}
