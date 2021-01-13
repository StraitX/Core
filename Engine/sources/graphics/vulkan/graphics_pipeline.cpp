#include "platform/memory.hpp"
#include "platform/result.hpp"
#include "core/log.hpp"
#include "graphics/vulkan/graphics_pipeline.hpp"
#include "graphics/vulkan/shader.hpp"
#include "graphics/vulkan/swapchain.hpp"

namespace StraitX{
namespace Vk{

struct VertexAttributeEntry{
    VkFormat Format;
    u8 Size;
};

static VertexAttributeEntry AttributesTable[]={
    {VK_FORMAT_R32_SINT,            1 * sizeof(i32)},
    {VK_FORMAT_R32_UINT,            1 * sizeof(u32)},
    {VK_FORMAT_R32_SFLOAT,          1 * sizeof(float)},
    {VK_FORMAT_R32G32_SINT,         2 * sizeof(i32)},
    {VK_FORMAT_R32G32_UINT,         2 * sizeof(u32)},
    {VK_FORMAT_R32G32_SFLOAT,       2 * sizeof(float)},
    {VK_FORMAT_R32G32B32_SINT,      3 * sizeof(i32)},
    {VK_FORMAT_R32G32B32_UINT,      3 * sizeof(u32)},
    {VK_FORMAT_R32G32B32_SFLOAT,    3 * sizeof(float)},
    {VK_FORMAT_R32G32B32A32_SINT,   4 * sizeof(i32)},
    {VK_FORMAT_R32G32B32A32_UINT,   4 * sizeof(u32)},
    {VK_FORMAT_R32G32B32A32_SFLOAT, 4 * sizeof(float)}
};

u32 CalculateStride(const ArrayPtr<VertexAttribute> &vertex_attributes){
    u32 stride = 0;
    for(auto &attr: vertex_attributes){
        stride += AttributesTable[attr].Size;
    }
    return stride;
}


Result GraphicsPipeline::Create(VkPrimitiveTopology topology, VkPolygonMode fill_mode, 
                Vk::Swapchain *swapchain, Vk::RenderPass *render_pass, 
                size_t subpass_index, 
                const ArrayPtr<VkDescriptorSetLayout> &set_layouts,
                const ArrayPtr<VertexAttribute> &vertex_attributes,
                const ArrayPtr<Shader> &shaders)
{
    Pass = render_pass;

    CoreAssert(set_layouts.Size <= 4, "By Vulkan Spec, set_layout size can be at least 4, we support just that");
    VkPipelineLayoutCreateInfo layout_info;
    layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.pNext = nullptr;
    layout_info.flags = 0;
    layout_info.setLayoutCount = set_layouts.Size;
    layout_info.pSetLayouts = set_layouts.Pointer;
    layout_info.pushConstantRangeCount = 0;
    layout_info.pPushConstantRanges = nullptr;
    
    if(vkCreatePipelineLayout(Pass->Owner->Handle, &layout_info, nullptr, &Layout) != VK_SUCCESS)
        return Result::Unavailable;


    VkPipelineCacheCreateInfo cache_info;
    cache_info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    cache_info.pNext = nullptr;
    cache_info.flags = 0;
    cache_info.initialDataSize = 0;
    cache_info.pInitialData = nullptr;

    if(vkCreatePipelineCache(Pass->Owner->Handle, &cache_info, nullptr, &Cache) != VK_SUCCESS)
        return Result::Unavailable;
    
    // === ShaderStages===
    VkPipelineShaderStageCreateInfo *shader_stages = (VkPipelineShaderStageCreateInfo*)alloca(shaders.Size * sizeof(VkPipelineShaderStageCreateInfo));

    for(size_t i = 0; i<shaders.Size; i++){
        shader_stages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages[i].pNext = nullptr;
        shader_stages[i].flags = 0;
        shader_stages[i].stage = shaders[i].Stage;
        shader_stages[i].module = shaders[i].Handle;
        shader_stages[i].pName = "main";
        shader_stages[i].pSpecializationInfo = nullptr;
    }

    //=== VertexInputState ===
    constexpr u32 vertex_bindings_count = 1;
    constexpr u32 vertex_binding_index = 0;

    VkVertexInputBindingDescription vertex_binding;
    vertex_binding.binding = vertex_binding_index; // we can use just first buffer in vkCmdBindBuffer
    vertex_binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    vertex_binding.stride = CalculateStride(vertex_attributes);

    VkVertexInputAttributeDescription *attributes = (VkVertexInputAttributeDescription*)alloca(vertex_attributes.Size * sizeof(VkVertexInputAttributeDescription));

    u32 offset = 0;
    for(int i = 0; i<vertex_attributes.Size; i++){
        attributes[i].binding = vertex_binding_index; // 
        attributes[i].format = AttributesTable[vertex_attributes[i]].Format;
        attributes[i].location = i;
        attributes[i].offset = offset;

        offset += AttributesTable[vertex_attributes[i]].Size;
    }

    VkPipelineVertexInputStateCreateInfo vertex_input_state;
    vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_state.pNext = nullptr;
    vertex_input_state.flags = 0;
    vertex_input_state.vertexBindingDescriptionCount = vertex_bindings_count;
    vertex_input_state.pVertexBindingDescriptions = &vertex_binding;
    vertex_input_state.vertexAttributeDescriptionCount = vertex_attributes.Size;
    vertex_input_state.pVertexAttributeDescriptions = attributes;

    // === InputAssemblyState ===
    VkPipelineInputAssemblyStateCreateInfo input_assembly_state;
    input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state.pNext = nullptr;
    input_assembly_state.flags = 0;
    input_assembly_state.topology = topology;
    input_assembly_state.primitiveRestartEnable = VK_FALSE; // we don't want to fall into STRIP topology by index 0xFFFFFF

    // === ViewportState ===

    VkViewport viewport;
    viewport.minDepth = 0.0;
    viewport.maxDepth = 1.0;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = swapchain->SurfaceSize.x;
    viewport.height = swapchain->SurfaceSize.y;

    LogInfo("Swapchain size %", swapchain->SurfaceSize);

    VkRect2D scissors;
    scissors.offset = {0, 0};
    scissors.extent.width = swapchain->SurfaceSize.x;
    scissors.extent.height = swapchain->SurfaceSize.y;

    VkPipelineViewportStateCreateInfo viewport_state;
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.pNext = nullptr;
    viewport_state.flags = 0;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &scissors;

    // === RasterizationState ===

    VkPipelineRasterizationStateCreateInfo raster_state;
    raster_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    raster_state.pNext = nullptr;
    raster_state.flags = 0;
    raster_state.lineWidth = 1.0;
    raster_state.rasterizerDiscardEnable = VK_FALSE; // we don't want to discard all the work we have done
    raster_state.polygonMode = fill_mode;
    raster_state.cullMode = VK_CULL_MODE_BACK_BIT;
    raster_state.frontFace = VK_FRONT_FACE_CLOCKWISE;
    raster_state.depthClampEnable = VK_FALSE;
    raster_state.depthBiasEnable = VK_FALSE; // we don't want to bias depth values
    raster_state.depthBiasClamp = 0;
    raster_state.depthBiasConstantFactor = 0;
    raster_state.depthBiasSlopeFactor = 0;

    // === MultisampleState ===

    VkPipelineMultisampleStateCreateInfo ms_state;
    ms_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms_state.pNext = nullptr;
    ms_state.flags = 0;
    ms_state.sampleShadingEnable = VK_FALSE;
    ms_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    ms_state.minSampleShading = 1.0f; 
    ms_state.pSampleMask = nullptr; 
    ms_state.alphaToCoverageEnable = VK_FALSE; 
    ms_state.alphaToOneEnable = VK_FALSE; 

    // === ColorBlendState ===
    VkPipelineColorBlendAttachmentState blend_attachment;
    blend_attachment.blendEnable = VK_TRUE;
    blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
    blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;
    blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // !!
    blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;// !!

    VkPipelineColorBlendStateCreateInfo blend_state;
    blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blend_state.pNext = nullptr;
    blend_state.flags = 0;
    blend_state.attachmentCount = 1;
    blend_state.pAttachments = &blend_attachment;
    blend_state.logicOpEnable = VK_FALSE; // we blend by factor, not logic
    blend_state.logicOp = VK_LOGIC_OP_NO_OP;
    blend_state.blendConstants[0] = 0;
    blend_state.blendConstants[1] = 0;
    blend_state.blendConstants[2] = 0;
    blend_state.blendConstants[3] = 0;

    // === DynamicState ===
/*
    VkDynamicState dynamics[]={
        //VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamic_state;
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.pNext = nullptr;
    dynamic_state.flags = 0;
    dynamic_state.dynamicStateCount = sizeof(dynamics)/sizeof(VkDynamicState);
    dynamic_state.pDynamicStates = dynamics;
*/
    VkGraphicsPipelineCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.renderPass = Pass->Handle;
    info.subpass = subpass_index;
    info.basePipelineIndex = InvalidIndex;
    info.basePipelineHandle = VK_NULL_HANDLE;
    info.stageCount = shaders.Size;
    info.pStages = shader_stages;
    info.pVertexInputState = &vertex_input_state;
    info.pInputAssemblyState = &input_assembly_state;
    info.pTessellationState = nullptr; // don't have tesselation for now
    info.pViewportState = &viewport_state;
    info.pRasterizationState = &raster_state;
    info.pMultisampleState = &ms_state;
    info.pDepthStencilState = nullptr; // don't have depth testing for now
    info.pColorBlendState = &blend_state;
    info.pDynamicState = nullptr;//&dynamic_state;
    info.layout = Layout;

    return ResultError(vkCreateGraphicsPipelines(Pass->Owner->Handle, Cache, 1, &info, nullptr, &Handle) != VK_SUCCESS);
}

void GraphicsPipeline::Destroy(){
    vkDestroyPipelineLayout(Pass->Owner->Handle, Layout, nullptr);
    vkDestroyPipelineCache(Pass->Owner->Handle, Cache, nullptr);
    vkDestroyPipeline(Pass->Owner->Handle, Handle, nullptr);
}



};//namespace Vk::
};//namespace StraitX::
