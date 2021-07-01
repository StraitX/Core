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


GPUCommand::GPUCommand(const GPUCmdSetScissors &cmd):
	Type(GPUCommandType::SetScissors)
{
	SetScissors = cmd;
}

GPUCommand::GPUCommand(const GPUCmdSetViewport &cmd):
	Type(GPUCommandType::SetViewport)
{
	SetViewport = cmd;
}

void GPUCommandBuffer::CopyCPUToGPUBuffer(const CPUBuffer &src, const GPUBuffer &dst, size_t size, size_t src_offset, size_t dst_offset){
	GPUCmdCopyCPUToGPUBuffer cmd;
	cmd.Source = src.Handle();
	cmd.SourcePointer = src.Pointer();
	cmd.Destination = dst.Handle();
	cmd.Size = size;
	cmd.SourceOffset = src_offset;
	cmd.DestinationOffset = dst_offset;

	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::ChangeTextureLayout(Texture *texture, TextureLayout old_layout, TextureLayout new_layout){
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

void GPUCommandBuffer::DrawIndexed(size_t indices_count, size_t index_offset){
	GPUCmdDrawIndexed cmd;
	cmd.IndicesCount = indices_count;
	cmd.IndexOffset = index_offset;

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

void GPUCommandBuffer::SetScissors(float width, float height, float x, float y){
	GPUCmdSetScissors cmd;
	cmd.Width = width;
	cmd.Height = height;
	cmd.x = x;
	cmd.y = y;

	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::SetViewport(float width, float height, float x, float y){
	GPUCmdSetViewport cmd;
	cmd.Width = width;
	cmd.Height = height;
	cmd.x = x;
	cmd.y = y;

	m_Commands.PushBack(cmd);
}

void GPUCommandBuffer::Reset(){
	m_Commands.Clear();
}

}//namespace StraitX::