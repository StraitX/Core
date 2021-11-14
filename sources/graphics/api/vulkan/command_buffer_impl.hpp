#ifndef STRAITX_VULKAN_COMMAND_BUFFER_IMPL_HPP
#define STRAITX_VULKAN_COMMAND_BUFFER_IMPL_HPP

#include "core/os/vulkan.hpp"
#include "core/list.hpp"
#include "graphics/api/command_buffer.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"
#include "graphics/api/vulkan/texture_impl.hpp"
#include "graphics/api/vulkan/graphics_pipeline_impl.hpp"

namespace Vk{

class CommandPoolImpl: public CommandPool{
private:
    VkCommandPool m_Handle = VK_NULL_HANDLE;
    QueueFamily::Type m_TargetQueueFamily;
    u32 m_TargetQueueFamilyIndex = -1;
public:
    CommandPoolImpl(QueueFamily::Type target_queue_family = QueueFamily::Type::Graphics);

    ~CommandPoolImpl();

    CommandBuffer *Alloc()override;

    void Free(CommandBuffer *buffer)override;

    operator VkCommandPool()const{
        return m_Handle;
    }

    QueueFamily::Type TargetQueueType()const{
        return m_TargetQueueFamily;
    }
};

enum class ResourceOperationType{
    Nop,
    //BufferToBufferCopy,
    //BufferToImageCopy,
    LayoutChange,
};

struct LayoutChangeOp{
    ::Texture *Texture;
    TextureLayout NewLayout;
};

struct ResourceOperation{
    ResourceOperationType Type = ResourceOperationType::Nop;
    //bool IsSrcSynched = false;
    //bool IsDstSynched = false;

    union{
        LayoutChangeOp LayoutChange;
    };

    ResourceOperation(Texture *texture, TextureLayout new_layout){
        Type = ResourceOperationType::LayoutChange;
        LayoutChange.Texture = texture;
        LayoutChange.NewLayout = new_layout;
    }
};

struct CommandBufferBindings{
    const Vk::GraphicsPipelineImpl* Pipeline;

    CommandBufferBindings(){
        Reset();
    }

    void Reset(){
        Pipeline = nullptr;
    }

    void Bind(const GraphicsPipeline *pipeline){
        Pipeline = (const Vk::GraphicsPipelineImpl*)pipeline;
    }
};

class CommandBufferImpl: public CommandBuffer{
private:
    CommandPoolImpl *m_Pool = nullptr;
    VkCommandBuffer m_Handle = VK_NULL_HANDLE;
    List<ResourceOperation> m_Operations;
    CommandBufferBindings m_Bindings;
public:
    CommandBufferImpl(CommandPoolImpl *pool);

    ~CommandBufferImpl();

    operator VkCommandBuffer()const{
        return m_Handle;
    }

    CommandPoolImpl* Pool()const{
        return m_Pool;
    }

    LayoutChangeOp *GetLastTextureLayoutChange(const Texture *texture);

    VkImageLayout GetExecutionTextureLayout(const Texture *texture);

    void OnExecute();

    void Begin()override;

    void End()override;

    void Reset()override;

    void PipelineBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst)const;

    void MemoryBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst, VkAccessFlags src_acces, VkAccessFlags dst_access)const;

    void ImageBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst, VkAccessFlags src_acces, VkAccessFlags dst_access, VkImageLayout old, VkImageLayout next, VkImage img, VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT)const;

    void ChangeLayout(Texture2D *texture, TextureLayout new_layout)override;

    void BeginRenderPass(const RenderPass *rp, const Framebuffer *fb)override;

    void EndRenderPass()override;

    void Bind(const GraphicsPipeline *pipeline)override;

    void Draw(u32 vertices_count)override;

    void SetScissor(s32 x, s32 y, u32 width, u32 height)override;

    void SetViewport(s32 x, s32 y, u32 width, u32 height)override;

    void BindVertexBuffer(const Buffer *buffer)override;

    void BindIndexBuffer(const Buffer *buffer, IndicesType type)override;

    void DrawIndexed(u32 indices_count, u32 indices_offset)override;

    void Copy(const Buffer *src, const Buffer *dst, size_t size, size_t src_offset, size_t dst_offset)override;

    //Expects Texture to be in either General or TransferDstOptimal layouts
    void Copy(const Buffer *src, Texture2D *dst, Vector2u src_size, Vector2u dst_offset)override;

    //Expects Texture to be in either General or TransferDstOptimal layouts
    void ClearColor(Texture2D *texture, const Color &color)override;

    void Bind(const DescriptorSet *set)override;
};

}//namespace Vk::

#endif//STRAITX_VULKAN_COMMAND_BUFFER_IMPL_HPP