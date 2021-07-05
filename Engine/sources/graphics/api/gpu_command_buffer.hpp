#ifndef STRAIX_COMMAND_BUFFER_HPP
#define STRAIX_COMMAND_BUFFER_HPP

#include "core/bucket_list.hpp"
#include "core/math/vector4.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/texture.hpp"

#include "graphics/api/cpu_buffer.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/api/framebuffer.hpp"
#include "graphics/api/descriptor_set.hpp"

enum class IndicesType{
    Uint16,
    Uint32
};

enum class GPUCommandType: u32{
	None = 0,
	CopyCPUToGPUBuffer,
	ChangeTextureLayout,
	BindPipeline,
	BindDescriptorSet,
	BindVertexBuffer,
	BindIndexBuffer,
	BeginRenderPass,
	EndRenderPass,
	//Draw,
	DrawIndexed,

	ClearFramebufferColorAttachments,
	ClearFramebufferDepthAttachments,

	SetScissors,
	SetViewport
};

struct GPUCmdCopyCPUToGPUBuffer{
	GPUResourceHandle Source;
	void *SourcePointer;
	GPUResourceHandle Destination;
	size_t Size;
	size_t SourceOffset;
	size_t DestinationOffset;
};

struct GPUCmdChangeTextureLayout{
	class Texture *Texture;
	TextureLayout OldLayout;
	TextureLayout NewLayout;
};

struct GPUCmdBindPipeline{
	const class GraphicsPipeline *Pipeline;	
};

struct GPUCmdBindDescriptorSet{
	const class DescriptorSet *DescriptorSet;
};

struct GPUCmdBindVertexBuffer{
	GPUResourceHandle VertexBuffer;
};

struct GPUCmdBindIndexBuffer{
	GPUResourceHandle IndexBuffer;
	enum IndicesType IndicesType;
};

struct GPUCmdBeginRenderPass{
	const class RenderPass *RenderPass;
	const class Framebuffer *Framebuffer;
};

struct GPUCmdEndRenderPass{ };

/*
struct GPUCmdDraw{

};
*/

struct GPUCmdDrawIndexed{
	size_t IndicesCount;
	size_t IndexOffset;
};

struct GPUCmdClearFramebufferColorAttachments{
	const class Framebuffer *Framebuffer;
	Vector4f Color;
};

struct GPUCmdClearFramebufferDepthAttachments{
	const class Framebuffer *Framebuffer;
	float Depth;
};

struct GPUCmdSetScissors{
	float Width;
	float Height;
	float x;
	float y;
};

struct GPUCmdSetViewport{
	float Width;
	float Height;
	float x;
	float y;
};

struct GPUCommand{
	GPUCommandType Type = GPUCommandType::None;

	union {
		GPUCmdCopyCPUToGPUBuffer CopyCPUToGPUBuffer;
		GPUCmdChangeTextureLayout ChangeTextureLayout;
		GPUCmdBindPipeline BindPipeline;
		GPUCmdBindDescriptorSet BindDescriptorSet;
		GPUCmdBindVertexBuffer BindVertexBuffer;
		GPUCmdBindIndexBuffer BindIndexBuffer;
		GPUCmdBeginRenderPass BeginRenderPass;
		GPUCmdEndRenderPass EndRenderPass;
		//GPUCmdDraw Draw;
		GPUCmdDrawIndexed DrawIndexed;
		GPUCmdClearFramebufferColorAttachments ClearFramebufferColorAttachments;
		GPUCmdClearFramebufferDepthAttachments ClearFramebufferDepthAttachments;
		GPUCmdSetScissors SetScissors;
		GPUCmdSetViewport SetViewport;
	};

	GPUCommand(const GPUCmdCopyCPUToGPUBuffer &cmd);

	GPUCommand(const GPUCmdChangeTextureLayout &cmd);

	GPUCommand(const GPUCmdBindPipeline &cmd);

	GPUCommand(const GPUCmdBindDescriptorSet &cmd);

	GPUCommand(const GPUCmdBindVertexBuffer &cmd);

	GPUCommand(const GPUCmdBindIndexBuffer &cmd);

	GPUCommand(const GPUCmdBeginRenderPass &cmd);

	GPUCommand(const GPUCmdEndRenderPass &cmd);

	GPUCommand(const GPUCmdDrawIndexed &cmd);

	GPUCommand(const GPUCmdClearFramebufferColorAttachments &cmd);

	GPUCommand(const GPUCmdClearFramebufferDepthAttachments &cmd);

	GPUCommand(const GPUCmdSetScissors &cmd);

	GPUCommand(const GPUCmdSetViewport &cmd);
};

class GPUCommandBuffer{
private:
	typedef BucketList<GPUCommand, 32> CmdBucketList;

	CmdBucketList m_Commands;
public:	
	void CopyCPUToGPUBuffer(const CPUBuffer &src, const GPUBuffer &dst, size_t size, size_t src_offset = 0, size_t dst_offset = 0);

	void ChangeTextureLayout(Texture *texture, TextureLayout old_layout, TextureLayout new_layout);
	//NOTE: state of Bind* commands and Begin/End RenderPass is not shared between command buffer submitions
	void BindPipeline(const GraphicsPipeline *pipeline);

	void BindDescriptorSet(const DescriptorSet *set);

	void BindVertexBuffer(const GPUBuffer &vb);

	void BindIndexBuffer(const GPUBuffer &ib, IndicesType indices_type);

	void BeginRenderPass(const RenderPass *pass, const Framebuffer *fb);

	void EndRenderPass();
	//void Draw();
	void DrawIndexed(size_t indices_count, size_t index_offset = 0);

	void ClearFramebufferColorAttachments(const Framebuffer *fb, const Vector4f color);

	void ClearFramebufferDepthAttachments(const Framebuffer *fb, float depth);

	void SetScissors(float width, float height, float x, float y);

	void SetViewport(float width, float height, float x, float y);

	void Reset();

	size_t CommandsCount()const{
		return m_Commands.Size();
	}

	CmdBucketList::ConstIterator begin()const;

	CmdBucketList::ConstIterator end()const;
};

SX_INLINE GPUCommandBuffer::CmdBucketList::ConstIterator GPUCommandBuffer::begin()const{
	return m_Commands.begin();
}

SX_INLINE GPUCommandBuffer::CmdBucketList::ConstIterator GPUCommandBuffer::end()const{
	return m_Commands.end();	
}

#endif//STRAIX_COMMAND_BUFFER_HPP