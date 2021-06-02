#include <new>
#include "platform/memory.hpp"
#include "graphics/vulkan/graphics_pipeline_impl.hpp"
#include "graphics/vulkan/shader_impl.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/vulkan/gpu_texture_impl.hpp"
#include "graphics/vulkan/gpu.hpp"

namespace StraitX{
namespace Vk{

VkFormat GraphicsPipelineImpl::s_VertexAttributesTable[] = {
    VK_FORMAT_R32_SINT,
    VK_FORMAT_R32G32_SINT,
    VK_FORMAT_R32G32B32_SINT,
    VK_FORMAT_R32G32B32A32_SINT,

    VK_FORMAT_R32_UINT,
    VK_FORMAT_R32G32_UINT,
    VK_FORMAT_R32G32B32_UINT,
    VK_FORMAT_R32G32B32A32_UINT,

    VK_FORMAT_R32_SFLOAT,
    VK_FORMAT_R32G32_SFLOAT,
    VK_FORMAT_R32G32B32_SFLOAT,
    VK_FORMAT_R32G32B32A32_SFLOAT
};

VkPrimitiveTopology GraphicsPipelineImpl::s_TopologyTable[]={
    VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
    VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
    VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
};

VkPolygonMode GraphicsPipelineImpl::s_RasterizationModeTable[]={
    VK_POLYGON_MODE_FILL,
    VK_POLYGON_MODE_LINE,
    VK_POLYGON_MODE_POINT
};

VkBlendFactor GraphicsPipelineImpl::s_BlendFactorTable[] = {
    VK_BLEND_FACTOR_ZERO,
    VK_BLEND_FACTOR_ONE,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_FACTOR_DST_ALPHA
};

VkBlendOp GraphicsPipelineImpl::s_BlendFunctionTable[] = {
    VK_BLEND_OP_ADD
};

VkDescriptorType GraphicsPipelineImpl::s_DescriptorTypeTable[] = {
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
};

GraphicsPipelineImpl::GraphicsPipelineImpl(const GraphicsPipelineProperties &props):
    GraphicsPipeline(props),
    Pass(static_cast<const Vk::RenderPassImpl*>(props.Pass)),
    Scissors({{props.FramebufferViewport.x, props.FramebufferViewport.y}, {props.FramebufferViewport.Width, props.FramebufferViewport.Height}})
{
    SX_CORE_ASSERT(props.ShaderBindings.Size() <= MaxShaderBindings, "Vk: GraphicsPipelineImpl: shader bindings overflow");
    
    for(auto &shader: props.Shaders){
        if(!shader->IsValid())
            return;
    }

    Span<VkDescriptorPoolSize> descriptors((VkDescriptorPoolSize*)alloca(props.ShaderBindings.Size() * sizeof(VkDescriptorPoolSize)), props.ShaderBindings.Size());

    for(size_t i = 0; i<descriptors.Size(); ++i){
        descriptors[i].type = s_DescriptorTypeTable[(size_t)props.ShaderBindings[i].Type];
        descriptors[i].descriptorCount = props.ShaderBindings[i].Size;
    }

    VkDescriptorPoolCreateInfo pool_info;
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.pNext = nullptr;
    pool_info.flags = 0;
    pool_info.maxSets = 1;
    pool_info.poolSizeCount = descriptors.Size();
    pool_info.pPoolSizes = descriptors.Pointer();

    CoreFunctionAssert(vkCreateDescriptorPool(GPU::Get().Handle(), &pool_info, nullptr, &Pool), VK_SUCCESS, "Vk: GraphicsPipelineImpl: Can't create VkDescriptorPool");

    Span<VkDescriptorSetLayoutBinding> bindings((VkDescriptorSetLayoutBinding*)alloca(props.ShaderBindings.Size() * sizeof(VkDescriptorSetLayoutBinding)), props.ShaderBindings.Size());

    for(size_t i = 0; i<descriptors.Size(); ++i){
        bindings[i].binding = props.ShaderBindings[i].Binding;
        bindings[i].stageFlags = props.ShaderBindings[i].VisibleShaders;
        bindings[i].descriptorType = s_DescriptorTypeTable[(size_t)props.ShaderBindings[i].Type];
        bindings[i].descriptorCount = props.ShaderBindings[i].Size;
        bindings[i].pImmutableSamplers = nullptr;
    }

    VkDescriptorSetLayoutCreateInfo set_layout_info;
    set_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    set_layout_info.pNext = nullptr;
    set_layout_info.flags = 0;
    set_layout_info.bindingCount = bindings.Size();
    set_layout_info.pBindings = bindings.Pointer();

    vkCreateDescriptorSetLayout(GPU::Get().Handle(), &set_layout_info, nullptr, &SetLayout);

    VkPipelineLayoutCreateInfo layout_info;
    layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.pNext = nullptr;
    layout_info.flags = 0;
    layout_info.setLayoutCount = 1;
    layout_info.pSetLayouts = &SetLayout;
    layout_info.pushConstantRangeCount = 0;
    layout_info.pPushConstantRanges = nullptr;

    CoreFunctionAssert(vkCreatePipelineLayout(GPU::Get().Handle(), &layout_info, nullptr, &Layout), VK_SUCCESS, "Vk: GraphicsPipelineImpl: Can't create Pipeline Layout");

    VkDescriptorSetAllocateInfo set_info;
    set_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    set_info.pNext = nullptr;
    set_info.descriptorPool = Pool;
    set_info.descriptorSetCount = 1;
    set_info.pSetLayouts = &SetLayout;

    vkAllocateDescriptorSets(GPU::Get().Handle(), &set_info, &Set);

    //===Shader Stages===
    Span<VkPipelineShaderStageCreateInfo> stages((VkPipelineShaderStageCreateInfo *)alloca(props.Shaders.Size() * sizeof(VkPipelineShaderStageCreateInfo)), props.Shaders.Size());

    for(size_t i = 0; i<stages.Size(); i++){
        const Vk::ShaderImpl* shader = static_cast<const Vk::ShaderImpl*>(props.Shaders[i]);
        auto lang = props.Shaders[i]->GetLang();
        auto a = shader->Status;
        stages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stages[i].pNext = nullptr;
        stages[i].flags = 0;
        stages[i].pSpecializationInfo = nullptr;
        stages[i].module = shader->Handle;
        stages[i].pName = "main";
        stages[i].stage = Vk::ShaderImpl::GetStage(shader->GetType());

        SX_CORE_ASSERT(shader->GetType() != Shader::TessellationControl && shader->GetType() != Shader::TessellationEvaluation,"Vk: GraphicsPipelineImpl: Tessellation shaders are not supported");
    }
    //===VertexInputState===
    Span<VkVertexInputAttributeDescription> attributes((VkVertexInputAttributeDescription*)alloca(props.VertexAttributes.Size() * sizeof(VkVertexInputAttributeDescription)),props.VertexAttributes.Size());

    u32 offset = 0;
    for(size_t i = 0; i<attributes.Size(); i++){
        attributes[i].location = i;
        attributes[i].offset = offset;
        attributes[i].binding = 0;
        attributes[i].format = s_VertexAttributesTable[(size_t)props.VertexAttributes[i]];

        offset += s_VertexAttributeSizeTable[(size_t)props.VertexAttributes[i]];
    }

    VkVertexInputBindingDescription binding;
    binding.binding = 0;
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    binding.stride = offset; // final offset is stride

    VkPipelineVertexInputStateCreateInfo vertex_input_info;
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.pNext = nullptr;
    vertex_input_info.flags = 0;
    vertex_input_info.vertexBindingDescriptionCount = 1;
    vertex_input_info.pVertexBindingDescriptions = &binding;
    vertex_input_info.vertexAttributeDescriptionCount = attributes.Size();
    vertex_input_info.pVertexAttributeDescriptions = attributes.Pointer();

    //===InputAssemblyState===

    VkPipelineInputAssemblyStateCreateInfo input_assembly_info;
    input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_info.pNext = nullptr;
    input_assembly_info.flags = 0;
    input_assembly_info.topology = s_TopologyTable[(size_t)props.Topology];
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
    rasterization_info.polygonMode = s_RasterizationModeTable[(size_t)props.Rasterization];
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
    depth_stencil_info.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depth_stencil_info.stencilTestEnable = VK_FALSE;
    depth_stencil_info.front = {};
    depth_stencil_info.back = {};
    depth_stencil_info.depthBoundsTestEnable = VK_FALSE;
    depth_stencil_info.minDepthBounds = 0.0;
    depth_stencil_info.maxDepthBounds = 1.0;

    //===ColorBlendState===
    size_t states_count = Pass->Attachments().Size() - Pass->HasDepth();

    Span<VkPipelineColorBlendAttachmentState> blend_states((VkPipelineColorBlendAttachmentState*)alloca(states_count * sizeof(VkPipelineColorBlendAttachmentState)), states_count);
    for(size_t i = 0; i<states_count; ++i){

        //TODO separate blending for color and alpha
        blend_states[i].blendEnable = VK_TRUE;
        blend_states[i].srcColorBlendFactor = s_BlendFactorTable[(size_t)props.SrcBlendFactor];
        blend_states[i].dstColorBlendFactor = s_BlendFactorTable[(size_t)props.DstBlendFactor];
        blend_states[i].colorBlendOp = s_BlendFunctionTable[(size_t)props.BlendFunc];
        blend_states[i].srcAlphaBlendFactor = s_BlendFactorTable[(size_t)props.SrcBlendFactor];
        blend_states[i].dstAlphaBlendFactor = s_BlendFactorTable[(size_t)props.DstBlendFactor];
        blend_states[i].alphaBlendOp = s_BlendFunctionTable[(size_t)props.BlendFunc];
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
    info.pDepthStencilState = Pass->HasDepth() ? &depth_stencil_info : nullptr;
    info.pColorBlendState = &blend_info;
    info.pDynamicState = &dynamic_info;
    info.layout = Layout;
    info.renderPass = Pass->Handle();
    info.subpass = 0;
    info.basePipelineHandle = VK_NULL_HANDLE;
    info.basePipelineIndex = InvalidIndex;

    Status = vkCreateGraphicsPipelines(GPU::Get().Handle(), VK_NULL_HANDLE, 1, &info, nullptr, &Handle);

}

GraphicsPipelineImpl::~GraphicsPipelineImpl(){
    vkDestroyDescriptorSetLayout(GPU::Get().Handle(), SetLayout, nullptr);
    vkDestroyDescriptorPool(GPU::Get().Handle(), Pool, nullptr);
    vkDestroyPipelineLayout(GPU::Get().Handle(), Layout, nullptr);
    vkDestroyPipeline(GPU::Get().Handle(), Handle, nullptr);
}

bool GraphicsPipelineImpl::IsValid()const{
    return Status == VK_SUCCESS;
}

void GraphicsPipelineImpl::Bind(size_t binding, size_t index, const GPUBuffer &uniform_buffer){
    VkDescriptorBufferInfo buffer;
    buffer.buffer = reinterpret_cast<VkBuffer>(uniform_buffer.Handle().U64);
    buffer.offset = 0;
    buffer.range = VK_WHOLE_SIZE;

    VkWriteDescriptorSet write;
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;
    write.dstSet = Set;
    write.dstBinding = binding;
    write.dstArrayElement = index;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write.pImageInfo = nullptr;
    write.pBufferInfo = &buffer;
    write.pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(GPU::Get().Handle(), 1, &write, 0, nullptr);
}

void GraphicsPipelineImpl::Bind(size_t binding, size_t index, const GPUTexture &texture, const Sampler &sampler){
    VkDescriptorImageInfo image;
    image.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;//GPUTextureImpl::s_LayoutTable[(size_t)texture.GetLayout()];
    image.imageView = reinterpret_cast<VkImageView>(texture.ViewHandle().U64);
    image.sampler = reinterpret_cast<VkSampler>(sampler.Handle().U64);

    VkWriteDescriptorSet write;
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;
    write.dstSet = Set;
    write.dstBinding = binding;
    write.dstArrayElement = index;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.pImageInfo = &image;
    write.pBufferInfo = nullptr;
    write.pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(GPU::Get().Handle(), 1, &write, 0, nullptr);
}

GraphicsPipeline * GraphicsPipelineImpl::NewImpl(const GraphicsPipelineProperties &props){
    return new(Memory::Alloc(sizeof(GraphicsPipelineImpl))) GraphicsPipelineImpl(props);
}

void GraphicsPipelineImpl::DeleteImpl(GraphicsPipeline *pipeline){
    pipeline->~GraphicsPipeline();
    Memory::Free(pipeline);
}


}//namespace Vk::
}//namespace StraitX::