#include "graphics/api/gpu_command_buffer.hpp"

namespace StraitX{


GPUCommand::GPUCommand(const GPUCmdCopyCPUToGPUBuffer &cmd):
	Type(GPUCommandType::CopyCPUToGPUBuffer)
{
	CopyCPUToGPUBuffer = cmd;
}

GPUCommand::GPUCommand(const GPUCmdChangeTextureLayout &cmd):
	Type(GPUCommandType::ChangeTextureLayout)
{
	ChangeTextureLayout = cmd;
}

GPUCommand::GPUCommand(const GPUCmdBindPipeline &cmd):
	Type(GPUCommandType::BindPipeline)
{
	BindPipeline = cmd;
}

GPUCommand::GPUCommand(const GPUCmdBindDescriptorSet &cmd):
	Type(GPUCommandType::BindDescriptorSet)
{
	BindDescriptorSet = cmd;
}

GPUCommand::GPUCommand(const GPUCmdBindVertexBuffer &cmd):
	Type(GPUCommandType::BindVertexBuffer)
{
	BindVertexBuffer = cmd;
}

GPUCommand::GPUCommand(const GPUCmdBindIndexBuffer &cmd):
	Type(GPUCommandType::BindIndexBuffer)
{
	BindIndexBuffer = cmd;
}

GPUCommand::GPUCommand(const GPUCmdBeginRenderPass &cmd):
	Type(GPUCommandType::BeginRenderPass)
{
	BeginRenderPass = cmd;
}

GPUCommand::GPUCommand(const GPUCmdEndRenderPass &cmd):
	Type(GPUCommandType::EndRenderPass)
{
	EndRenderPass = cmd;
}

GPUCommand::GPUCommand(const GPUCmdDrawIndexed &cmd):
	Type(GPUCommandType::DrawIndexed)
{
	DrawIndexed = cmd;
}

GPUCommand::GPUCommand(const GPUCmdClearFramebufferColorAttachments &cmd):
	Type(GPUCommandType::ClearFramebufferColorAttachments)
{
	ClearFramebufferColorAttachments = cmd;
}

GPUCommand::GPUCommand(const GPUCmdClearFramebufferDepthAttachments &cmd):
	Type(GPUCommandType::ClearFramebufferDepthAttachments)
{
	ClearFramebufferDepthAttachments = cmd;
}

void GPUCommandBuffer::CopyCPUToGPUBuffer(const CPUBuffer &src, const GPUBuffer &dst, size_t size, size_t src_offset, size_t dst_offset){
	GPUCmdCopyCPUToGPUBuffer cmd;
	cmd.Source = src.Handle();
	cmd.Destination = dst.Handle();
	cmd.Size = size;
	cmd.SourceOffset = src_offset;
	cmd.DestinationOffset = dst_offset;

	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::ChangeTextureLayout(GPUTexture *texture, GPUTexture::Layout old_layout, GPUTexture::Layout new_layout){
	GPUCmdChangeTextureLayout cmd;
	cmd.Texture = texture;
	cmd.OldLayout = old_layout;
	cmd.NewLayout = new_layout;

	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::BindPipeline(const GraphicsPipeline *pipeline){
	GPUCmdBindPipeline cmd;
	cmd.Pipeline = pipeline;

	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::BindDescriptorSet(const DescriptorSet *set){
	GPUCmdBindDescriptorSet cmd;
	cmd.DescriptorSet = set;

	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::BindVertexBuffer(const GPUBuffer &vb){
	GPUCmdBindVertexBuffer cmd;
	cmd.VertexBuffer = vb.Handle();

	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::BindIndexBuffer(const GPUBuffer &ib, IndicesType indices_type){
	GPUCmdBindIndexBuffer cmd;
	cmd.IndexBuffer = ib.Handle();
	cmd.IndicesType = indices_type;

	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::BeginRenderPass(const RenderPass *pass, const Framebuffer *fb){
	GPUCmdBeginRenderPass cmd;
	cmd.RenderPass = pass;
	cmd.Framebuffer = fb;

	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::EndRenderPass(){
	GPUCmdEndRenderPass cmd;
	
	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::DrawIndexed(size_t indices_count){
	GPUCmdDrawIndexed cmd;
	cmd.IndicesCount = indices_count;

	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::ClearFramebufferColorAttachments(const Framebuffer *fb, const Vector4f color){
	GPUCmdClearFramebufferColorAttachments cmd;
	cmd.Framebuffer = fb;
	cmd.Color = color;

	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::ClearFramebufferDepthAttachments(const Framebuffer *fb, float depth){
	GPUCmdClearFramebufferDepthAttachments cmd;
	cmd.Framebuffer = fb;
	cmd.Depth = depth;
	
	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::Reset(){
	m_Commands.Clear();
}

}//namespace StraitX::