#ifndef STRAITX_VULKAN_GRAPHICS_PIPELINE_HPP
#define STRAITX_VULKAN_GRAPHICS_PIPELINE_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "core/array_ptr.hpp"
#include "graphics/vulkan/command_buffer.hpp"
#include "graphics/vulkan/render_pass.hpp"
#include "graphics/vulkan/swapchain.hpp"
#include "graphics/vulkan/shader.hpp"

namespace StraitX{
namespace Vk{


enum VertexAttribute: u8{
    Int = 0,
    Uint,
    Float,
    Int2,
    Uint2,
    Float2,
    Int3,
    Uint3,
    Float3,
    Int4,
    Uint4,
    Float4,

    AttributesCount
};

struct GraphicsPipeline{
    VkPipeline      Handle = VK_NULL_HANDLE;
    VkPipelineCache Cache  = VK_NULL_HANDLE;
    Vk::RenderPass *Pass   = nullptr;

    Result Create(VkPrimitiveTopology topology, VkPolygonMode fill_mode, 
                    Vk::Swapchain *swapchain, Vk::RenderPass *render_pass, 
                    size_t subpass_index, const ArrayPtr<VertexAttribute> &vertex_attributes,
                    const ArrayPtr<Shader> &shaders);

    void Destroy();
};


};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_GRAPHICS_PIPELINE_HPP