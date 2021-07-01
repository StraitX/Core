#include "platform/opengl.hpp"
#include "platform/glloader.hpp"
#include "core/log.hpp"
#include "core/string.hpp"
#include "graphics/opengl/graphics_context_impl.hpp"
#include "graphics/opengl/dma_impl.hpp"
#include "graphics/opengl/graphics_pipeline_impl.hpp"

namespace StraitX{
namespace GL{


GLenum GraphicsContextImpl::s_IndexTypeTable[] = {
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT
};

GraphicsContextImpl GraphicsContextImpl::s_Instance;

constexpr Version OpenGLVersion = {4, 6, 0};

static GPUVendor ExtractGPUVendor(const char *string){
    auto length = String::Length(string) + 1;
    char *str = (char*)alloca(sizeof(char)*length);
    Memory::Copy(string, str, length);

    String::ToLowerCase(str);

    if(String::Contains(str, "nvidia"))
        return GPUVendor::Nvidia;
    if(String::Contains(str, "intel"))
        return GPUVendor::Intel;
    if(String::Contains(str, "amd"))
        return GPUVendor::AMD;
    if(String::Contains(str, "microsoft"))
        return GPUVendor::Microsoft;

    return GPUVendor::Unknown;
}

static const GPUTexture *FakeFramebufferTexturePtr = nullptr;

static AttachmentDescription SwapchainAttachments[] = {
    {
        GPUTexture::Layout::PresentSrcOptimal,
        GPUTexture::Layout::PresentSrcOptimal,
        GPUTexture::Layout::ColorAttachmentOptimal,
        TextureFormat::BGRA8,
        SamplePoints::Samples_1,
    },
    {
        GPUTexture::Layout::DepthStencilAttachmentOptimal,
        GPUTexture::Layout::DepthStencilAttachmentOptimal,
        GPUTexture::Layout::DepthStencilAttachmentOptimal,
        TextureFormat::Depth32,
        SamplePoints::Samples_1,
    }
};

static RenderPassProperties GetFramebufferProperties(){
    return {{SwapchainAttachments, 2}};
}

Result GraphicsContextImpl::Initialize(const Window &window){
	if (!m_OpenGLContext.CreateDummy()) {
        LogError("OpenGLLoader: Can't create dummy OpenGL context");
        return Result::Unsupported;
    }
    if(!(m_OpenGLContext.MakeCurrent() && OpenGLLoader::Load())){
        LogError("OpenGLLoader: Can't load OpenGL Procedures");
        m_OpenGLContext.DestroyDummy();
        return Result::Unsupported;
    }
    m_OpenGLContext.DestroyDummy();

    if(m_OpenGLContext.Create(window, OpenGLVersion) != Result::Success)
        return Result::Failure;
    if(m_OpenGLContext.MakeCurrent() != Result::Success)
        return Result::Unavailable;

    m_LoadedOpenGLVersion = OpenGLLoader::OpenGLVersion();
    m_VendorString = (const char*)glGetString(GL_VENDOR);
    m_RendererString = (const char *)glGetString(GL_RENDERER);
    m_VersionString = (const char*)glGetString(GL_VERSION);

    m_Vendor = ExtractGPUVendor(m_VendorString);
    LogInfo("OpenGL: Loaded Version %",m_LoadedOpenGLVersion);
    LogInfo("OpenGL: Renderer: %", m_RendererString);
    LogInfo("OpenGL: Vendor: %", m_VendorString);
    LogInfo("OpenGL: Version: %", m_VersionString);


    glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_MaxTextureUnits);
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &m_MaxUniformBufferBindings);

    DLogInfo("GL: MaxTextureUnits:          %", m_MaxTextureUnits);
    DLogInfo("GL: MaxUniformBufferBindings: %", m_MaxUniformBufferBindings);

	if(m_MaxTextureUnits < DescriptorSetLayout::s_MaxTexturesBindings 
	|| m_MaxUniformBufferBindings < DescriptorSetLayout::s_MaxUniformBuffersBindings ){
		LogError("GL: Unsupported amount of bindings");
		return Result::Unsupported;
	}

	m_FramebufferPass.Construct(GetFramebufferProperties());

	m_DefaultFramebuffer.Construct(0, &m_FramebufferPass, FramebufferProperties{ {window.Size().width, window.Size().height}, {&FakeFramebufferTexturePtr, 1} });

	return Result::Success;
}

void GraphicsContextImpl::Finalize(){
	m_DefaultFramebuffer.Destruct();
	m_FramebufferPass.Destruct();

	m_OpenGLContext.Destroy();
}

void GraphicsContextImpl::ExecuteCmdBuffer(const GPUCommandBuffer &cmd_buffer){
	const GL::GraphicsPipelineImpl* pipeline_bind_point = nullptr;
	const GL::FramebufferImpl *render_pass_framebuffer = nullptr;
	GLenum current_indices_type = 0;

	for(const auto &cmd: cmd_buffer){
		switch(cmd.Type){
		case GPUCommandType::None:
		{
			(void)0;
		}
		break;
		case GPUCommandType::CopyCPUToGPUBuffer: 
		{	
			void *src_ptr = cmd.CopyCPUToGPUBuffer.SourcePointer;
			u32 dst = cmd.CopyCPUToGPUBuffer.Destination.U32;
			size_t size = cmd.CopyCPUToGPUBuffer.Size;
			size_t src_offset = cmd.CopyCPUToGPUBuffer.SourceOffset;
			size_t dst_offset = cmd.CopyCPUToGPUBuffer.DestinationOffset;
			glBindBuffer(GL_COPY_WRITE_BUFFER, dst);
    		glBufferSubData(GL_COPY_WRITE_BUFFER, dst_offset, size, (u8*)src_ptr + src_offset);
		}
		break;
		case GPUCommandType::ChangeTextureLayout: 
		{
			cmd.ChangeTextureLayout.Texture->m_Layout = cmd.ChangeTextureLayout.NewLayout;
		}
		break;
		case GPUCommandType::BindPipeline: 
		{
			pipeline_bind_point = static_cast<const GL::GraphicsPipelineImpl*>(cmd.BindPipeline.Pipeline);
			pipeline_bind_point->Bind();
		}		
		break;
		case GPUCommandType::BindDescriptorSet: 
		{
			const DescriptorSetImpl *set = static_cast<const DescriptorSetImpl*>(cmd.BindDescriptorSet.DescriptorSet);
			for(const auto &binding: set->UniformBufferBindings){
				const VirtualBinding &virtual_binding = pipeline_bind_point->VirtualBindings[binding.Binding];
				SX_CORE_ASSERT(binding.Index < virtual_binding.ArraySize, "GL: GraphicsContextImpl: binding array overflow");

				u32 buffer_binding = virtual_binding.BaseGLBinding + binding.Index;

				glBindBufferBase(GL_UNIFORM_BUFFER, buffer_binding, binding.UniformBuffer);
			}

			for(const auto &binding: set->TextureBindings){
				const VirtualBinding &virtual_binding = pipeline_bind_point->VirtualBindings[binding.Binding];

				SX_CORE_ASSERT(binding.Index < virtual_binding.ArraySize, "GL: GraphicsContextImpl: binding array overflow");

				u32 texture_slot = virtual_binding.BaseGLBinding + binding.Index;

				glActiveTexture(GL_TEXTURE0 + texture_slot);
				glBindTexture(GL_TEXTURE_2D, binding.Texture);
				glBindSampler(texture_slot, binding.Sampler);
			}
		}		
		break;
		case GPUCommandType::BindVertexBuffer: 
		{
			pipeline_bind_point->BindVertexBuffer(cmd.BindVertexBuffer.VertexBuffer.U32);
		}		
		break;
		case GPUCommandType::BindIndexBuffer: 
		{
			pipeline_bind_point->BindIndexBuffer(cmd.BindIndexBuffer.IndexBuffer.U32);
			current_indices_type = s_IndexTypeTable[(size_t)cmd.BindIndexBuffer.IndicesType];
		}		
		break;
		case GPUCommandType::BeginRenderPass: 
		{
			render_pass_framebuffer = static_cast<const GL::FramebufferImpl *>(cmd.BeginRenderPass.Framebuffer);
    		render_pass_framebuffer->Bind();
		}		
		break;
		case GPUCommandType::EndRenderPass: 
		{
			render_pass_framebuffer = nullptr;
		}		
		break;
		case GPUCommandType::DrawIndexed: 
		{
			glDrawElements(pipeline_bind_point->Topology, cmd.DrawIndexed.IndicesCount, current_indices_type, nullptr);
		}		
		break;
		case GPUCommandType::ClearFramebufferColorAttachments:
		{
			const GL::FramebufferImpl *target_framebuffer = static_cast<const GL::FramebufferImpl *>(cmd.ClearFramebufferColorAttachments.Framebuffer);
			
			s32 current_framebuffer_handle = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_framebuffer_handle);

			u32 target_framebuffer_handle = target_framebuffer->Handle();

			target_framebuffer->Bind();

			if(target_framebuffer_handle != current_framebuffer_handle)
				glBindFramebuffer(GL_FRAMEBUFFER, target_framebuffer_handle);
			
			Vector4f color = cmd.ClearFramebufferColorAttachments.Color;
			glClearColor(color.x, color.y, color.z, color.w);
			glClear(GL_COLOR_BUFFER_BIT);

			if(target_framebuffer_handle != current_framebuffer_handle)
				glBindFramebuffer(GL_FRAMEBUFFER, current_framebuffer_handle);
		}
		break;
		case GPUCommandType::ClearFramebufferDepthAttachments:
		{
			const GL::FramebufferImpl *target_framebuffer = static_cast<const GL::FramebufferImpl *>(cmd.ClearFramebufferDepthAttachments.Framebuffer);
			
			s32 current_framebuffer_handle = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_framebuffer_handle);

			u32 target_framebuffer_handle = target_framebuffer->Handle();

			if(target_framebuffer_handle != current_framebuffer_handle)
				glBindFramebuffer(GL_FRAMEBUFFER, target_framebuffer_handle);
			
			float depth = cmd.ClearFramebufferDepthAttachments.Depth;
			glClearDepth(depth);
			glClear(GL_DEPTH_BUFFER_BIT);

			if(target_framebuffer_handle != current_framebuffer_handle)
				glBindFramebuffer(GL_FRAMEBUFFER, current_framebuffer_handle);
		}
		break;
		}
	}
}

void GraphicsContextImpl::SwapBuffers(){
	m_OpenGLContext.SwapBuffers();	
}

const Framebuffer *GraphicsContextImpl::CurrentFramebuffer(){
	return &m_DefaultFramebuffer;
}

const RenderPass *GraphicsContextImpl::FramebufferPass(){
	return &m_FramebufferPass;
}

Version GraphicsContextImpl::LoadedVersion()const{
	return m_LoadedOpenGLVersion;
}

u32 GraphicsContextImpl::MaxTextureUnits()const{
	return m_MaxTextureUnits;
}

}//namespace GL::
}//namespace StraitX::
