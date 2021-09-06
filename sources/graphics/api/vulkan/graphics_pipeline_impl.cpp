#include "graphics/api/vulkan/graphics_pipeline_impl.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"
#include "graphics/api/vulkan/debug.hpp"
#include "graphics/api/vulkan/shader_impl.hpp"
#include "graphics/api/vulkan/render_pass_impl.hpp"
#include "core/os/memory.hpp"

namespace Vk{

VkFormat ToVkVertexAttributeFormat(VertexAttribute attribute){
    static const VkFormat s_VertexAttributesTable[] = {
        VK_FORMAT_R32_SFLOAT,
        VK_FORMAT_R32G32_SFLOAT,
        VK_FORMAT_R32G32B32_SFLOAT,
        VK_FORMAT_R32G32B32A32_SFLOAT,

        VK_FORMAT_R8_UNORM,
        VK_FORMAT_R8G8_UNORM,
        VK_FORMAT_R8G8B8_UNORM,
        VK_FORMAT_R8G8B8A8_UNORM,

        VK_FORMAT_R8_SNORM,
        VK_FORMAT_R8G8_SNORM,
        VK_FORMAT_R8G8B8_SNORM,
        VK_FORMAT_R8G8B8A8_SNORM,

        VK_FORMAT_R32_UINT,
        VK_FORMAT_R32G32_UINT,
        VK_FORMAT_R32G32B32_UINT,
        VK_FORMAT_R32G32B32A32_UINT,

        VK_FORMAT_R32_SINT,
        VK_FORMAT_R32G32_SINT,
        VK_FORMAT_R32G32B32_SINT,
        VK_FORMAT_R32G32B32A32_SINT
    };

    return s_VertexAttributesTable[(size_t)attribute];
}
VkPrimitiveTopology ToVkPrimitiveTopology(PrimitivesTopology topology){
    static const VkPrimitiveTopology s_TopologyTable[]={
        VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
        VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
        VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
    };

    return s_TopologyTable[(size_t)topology];
}

VkPolygonMode ToVkPolygonMode(RasterizationMode mode){
    static const VkPolygonMode s_RasterizationModeTable[]={
        VK_POLYGON_MODE_FILL,
        VK_POLYGON_MODE_LINE,
        VK_POLYGON_MODE_POINT
    };

    return s_RasterizationModeTable[(size_t)mode];
}

VkBlendFactor ToVkBlendFactor(BlendFactor factor){
    static const VkBlendFactor s_BlendFactorTable[] = {
        VK_BLEND_FACTOR_ZERO,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
        VK_BLEND_FACTOR_SRC_ALPHA,
        VK_BLEND_FACTOR_DST_ALPHA
    };
    return s_BlendFactorTable[(size_t)factor];
}

VkBlendOp ToVkBlendOp(BlendFunction func){
    static const VkBlendOp s_BlendFunctionTable[] = {
        VK_BLEND_OP_ADD
    };
    return s_BlendFunctionTable[(size_t)func];
}

VkCompareOp ToVkDepthCompareOp(DepthFunction func){
    static const VkCompareOp s_DepthFunctionTable[] = {
        VK_COMPARE_OP_ALWAYS,
        VK_COMPARE_OP_LESS,
        VK_COMPARE_OP_LESS_OR_EQUAL,
        VK_COMPARE_OP_EQUAL,
        VK_COMPARE_OP_NOT_EQUAL,
        VK_COMPARE_OP_GREATER_OR_EQUAL,
        VK_COMPARE_OP_GREATER,
        VK_COMPARE_OP_NEVER
    };
    return s_DepthFunctionTable[(size_t)func];
}

GraphicsPipelineImpl::GraphicsPipelineImpl(const GraphicsPipelineProperties &props){    


    VkPipelineLayoutCreateInfo layout_info;
    layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.pNext = nullptr;
    layout_info.flags = 0;
    layout_info.setLayoutCount = 0;
    layout_info.pSetLayouts = nullptr;
    layout_info.pushConstantRangeCount = 0;
    layout_info.pPushConstantRanges = nullptr;
    VkShaderStageFlagBits n;

    SX_VK_ASSERT(vkCreatePipelineLayout(GPUImpl::s_Instance, &layout_info, nullptr, &m_Layout), "Vk: GraphicsPipelineImpl: Can't create Pipeline Layout");

    //===Shader Stages===
    Span<VkPipelineShaderStageCreateInfo> stages(SX_STACK_ARRAY_ALLOC(VkPipelineShaderStageCreateInfo, props.Shaders.Size()), props.Shaders.Size());

    for(size_t i = 0; i<stages.Size(); i++){
        const Vk::ShaderImpl* shader = static_cast<const Vk::ShaderImpl*>(props.Shaders[i]);
        ShaderLang lang = props.Shaders[i]->Lang();

        stages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stages[i].pNext = nullptr;
        stages[i].flags = 0;
        stages[i].pSpecializationInfo = nullptr;
        stages[i].module = *shader;
        stages[i].pName = "main";
        stages[i].stage = ToVkShaderStage(shader->Stage());

        SX_CORE_ASSERT(shader->Stage() != ShaderStageBits::TessellationControl && shader->Stage() != ShaderStageBits::TessellationEvaluation,"Vk: GraphicsPipelineImpl: Tessellation shaders are not supported yet");
    }
    //===VertexInputState===
    Span<VkVertexInputAttributeDescription> attributes((VkVertexInputAttributeDescription*)alloca(props.VertexAttributes.Size() * sizeof(VkVertexInputAttributeDescription)),props.VertexAttributes.Size());

    u32 offset = 0;
    for(size_t i = 0; i<attributes.Size(); i++){
        attributes[i].location = i;
        attributes[i].offset = offset;
        attributes[i].binding = 0;
        attributes[i].format = ToVkVertexAttributeFormat(props.VertexAttributes[i]);

        offset += GetVertexAttributeSize(props.VertexAttributes[i]);
    }

    VkVertexInputBindingDescription binding;
    binding.binding = 0;
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    binding.stride = offset; // final offset is stride

    VkPipelineVertexInputStateCreateInfo vertex_input_info;
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.pNext = nullptr;
    vertex_input_info.flags = 0;
    vertex_input_info.vertexBindingDescriptionCount = props.VertexAttributes.Size() ? 1 : 0;
    vertex_input_info.pVertexBindingDescriptions = &binding;
    vertex_input_info.vertexAttributeDescriptionCount = attributes.Size();
    vertex_input_info.pVertexAttributeDescriptions = attributes.Pointer();

    //===InputAssemblyState===

    VkPipelineInputAssemblyStateCreateInfo input_assembly_info;
    input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_info.pNext = nullptr;
    input_assembly_info.flags = 0;
    input_assembly_info.topology = ToVkPrimitiveTopology(props.PrimitivesTopology);
    input_assembly_info.primitiveRestartEnable = VK_FALSE;

    //===ViewportState===

    VkPipelineViewportStateCreateInfo viewport_state_info;
    viewport_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_info.pNext = nullptr;
    viewport_state_info.flags = 0;
    viewport_state_info.viewportCount = 1;
    viewport_state_info.pViewports = nullptr;
    viewport_state_info.scissorCount = 1;
    viewport_state_info.pScissors = nullptr;

    //===RasterizationState===

    VkPipelineRasterizationStateCreateInfo rasterization_info;
    rasterization_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_info.pNext = nullptr;
    rasterization_info.flags = 0;
    rasterization_info.depthClampEnable = VK_FALSE;
    rasterization_info.rasterizerDiscardEnable = VK_FALSE;
    rasterization_info.polygonMode = ToVkPolygonMode(props.RasterizationMode);
    rasterization_info.cullMode = VK_CULL_MODE_NONE;// following one is disabled
    rasterization_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterization_info.lineWidth = 1.0f;
    rasterization_info.depthBiasEnable = VK_FALSE; // following is disabled
    rasterization_info.depthBiasConstantFactor = 0.0f;
    rasterization_info.depthBiasClamp = 0.0f;
    rasterization_info.depthBiasSlopeFactor = 0.0f;

    //===MultisampleState===

    VkPipelineMultisampleStateCreateInfo multisampling_state;
    multisampling_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling_state.pNext = nullptr;
    multisampling_state.flags = 0;
    multisampling_state.sampleShadingEnable = VK_FALSE; // following is disabled
    multisampling_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling_state.minSampleShading = 1.0f; 
    multisampling_state.pSampleMask = nullptr;
    multisampling_state.alphaToCoverageEnable = VK_FALSE; 
    multisampling_state.alphaToOneEnable = VK_FALSE; 

    //===DepthStencilState===
    VkPipelineDepthStencilStateCreateInfo depth_stencil_info;
    depth_stencil_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil_info.pNext = nullptr;
    depth_stencil_info.flags = 0;
    depth_stencil_info.depthTestEnable = VK_TRUE;
    depth_stencil_info.depthWriteEnable = VK_TRUE;
    depth_stencil_info.depthCompareOp = ToVkDepthCompareOp(props.DepthFunction);
    depth_stencil_info.stencilTestEnable = VK_FALSE;
    depth_stencil_info.front = {};
    depth_stencil_info.back = {};
    depth_stencil_info.depthBoundsTestEnable = VK_FALSE;
    depth_stencil_info.minDepthBounds = 0.0;
    depth_stencil_info.maxDepthBounds = 1.0;

    auto pass = (const Vk::RenderPassImpl*)props.Pass;

    //===ColorBlendState===
    size_t states_count = props.Pass->Attachments().Size() - pass->HasDepth();

    Span<VkPipelineColorBlendAttachmentState> blend_states((VkPipelineColorBlendAttachmentState*)alloca(states_count * sizeof(VkPipelineColorBlendAttachmentState)), states_count);
    for(size_t i = 0; i<states_count; ++i){

        //TODO separate blending for color and alpha
        blend_states[i].blendEnable = VK_TRUE;
        blend_states[i].srcColorBlendFactor = ToVkBlendFactor(props.SrcBlendFactor);
        blend_states[i].dstColorBlendFactor = ToVkBlendFactor(props.DstBlendFactor);
        blend_states[i].colorBlendOp = ToVkBlendOp(props.BlendFunction);
        blend_states[i].srcAlphaBlendFactor = ToVkBlendFactor(props.SrcBlendFactor);
        blend_states[i].dstAlphaBlendFactor = ToVkBlendFactor(props.DstBlendFactor);
        blend_states[i].alphaBlendOp = ToVkBlendOp(props.BlendFunction);
        blend_states[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    }

    VkPipelineColorBlendStateCreateInfo blend_info;
    blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blend_info.pNext = nullptr;
    blend_info.flags = 0;
    blend_info.logicOpEnable = VK_FALSE;
    blend_info.logicOp = VK_LOGIC_OP_NO_OP;
    blend_info.attachmentCount = blend_states.Size();
    blend_info.pAttachments = blend_states.Pointer();

    //===DynamicState===

    VkDynamicState dynamic_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamic_info;
    dynamic_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_info.pNext = nullptr;
    dynamic_info.flags = 0;
    dynamic_info.dynamicStateCount = sizeof(dynamic_states)/sizeof(VkDynamicState);
    dynamic_info.pDynamicStates = dynamic_states;

    VkGraphicsPipelineCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.stageCount = stages.Size();
    info.pStages = stages.Pointer();
    info.pVertexInputState = &vertex_input_info;
    info.pInputAssemblyState = &input_assembly_info;
    info.pTessellationState = nullptr; // skip tessellation for now
    info.pViewportState = &viewport_state_info;
    info.pRasterizationState = &rasterization_info;
    info.pMultisampleState = &multisampling_state;
    info.pDepthStencilState = pass->HasDepth() ? &depth_stencil_info : nullptr;
    info.pColorBlendState = &blend_info;
    info.pDynamicState = &dynamic_info;
    info.layout = m_Layout;
    info.renderPass = *pass;
    info.subpass = 0;
    info.basePipelineHandle = VK_NULL_HANDLE;
    info.basePipelineIndex = InvalidIndex;

    vkCreateGraphicsPipelines(GPUImpl::s_Instance, VK_NULL_HANDLE, 1, &info, nullptr, &m_Handle);
}

GraphicsPipelineImpl::~GraphicsPipelineImpl(){
    vkDestroyPipelineLayout(GPUImpl::s_Instance, m_Layout, nullptr);
    vkDestroyPipeline(GPUImpl::s_Instance, m_Handle, nullptr);
}

ConstSpan<VertexAttribute> GraphicsPipelineImpl::VertexAttributes()const{
    return m_VertexAttributes;
}

}//namespace Vk::