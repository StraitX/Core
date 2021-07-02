#ifndef STRAITX_IMGUI_BACKEND_HPP
#define STRAITX_IMGUI_BACKEND_HPP

#include "core/subsystem.hpp"
#include "graphics/api/texture.hpp"
#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/api/descriptor_set.hpp"
#include "graphics/api/graphics_context.hpp"
#include "graphics/api/gpu_command_buffer.hpp"

namespace StraitX{

class ImGuiBackend: public Subsystem{
private:
	static ShaderBinding s_Bindings[2];
	static VertexAttribute s_Attributes[3];

	Texture2D m_ImGuiFont;

	DescriptorSetLayout *m_DescriptorSetLayout = nullptr;

	DescriptorSetPool *m_DescriptorSetPool = nullptr;
	DescriptorSet *m_Set = nullptr;

	const Shader *m_Shaders[2] = {};

	GraphicsPipeline *m_Pipeline = nullptr;

	GPUBuffer m_VertexBuffer;
	GPUBuffer m_IndexBuffer;
	GPUBuffer m_UniformBuffer;

	struct Uniform{
		Vector2f u_Scale;
		Vector2f u_Translate;
	};

	GPUCommandBuffer m_CmdBuffer;

public:
	ImGuiBackend();

	Result OnInitialize()override;

	void OnBeginFrame()override;

	void OnEndFrame()override;

	void OnEvent(const Event &e)override;

	void OnFinalize()override;
};

}//namespace StraitX::

#endif//STRAITX_IMGUI_BACKEND_HPP