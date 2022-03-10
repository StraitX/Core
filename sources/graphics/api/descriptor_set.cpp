#include "graphics/api/descriptor_set.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"

#ifdef SX_VULKAN_SUPPORTED
    #include "graphics/api/vulkan/descriptor_set_impl.hpp"
#endif

DescriptorSetLayout *DescriptorSetLayout::Create(ConstSpan<ShaderBinding> bindings){
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::DescriptorSetLayoutImpl(bindings);
#endif
    return nullptr;
}

DescriptorSetPool *DescriptorSetPool::Create(const DescriptorSetPoolProperties &props){
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::DescriptorSetPoolImpl(props);
#endif

    return nullptr;
}

SingleFrameDescriptorSetPool::SingleFrameDescriptorSetPool(const DescriptorSetPoolProperties &props, size_t preallocate_sets){
    m_Pool = DescriptorSetPool::Create(props);

    m_TotalAllocatedSets.Reserve(preallocate_sets);
    for(int i = 0; i<preallocate_sets; i++){
        m_TotalAllocatedSets.Add(m_Pool->Alloc());
    }
}

SingleFrameDescriptorSetPool::~SingleFrameDescriptorSetPool(){
    for(auto *set: m_TotalAllocatedSets)
        m_Pool->Free(set);
    delete m_Pool;
}

DescriptorSet *SingleFrameDescriptorSetPool::Alloc(){
    if(m_AllocatedThisFrame == m_TotalAllocatedSets.Size()){
        m_TotalAllocatedSets.Add(m_Pool->Alloc());
    }

    return m_TotalAllocatedSets[m_AllocatedThisFrame++];
}

void SingleFrameDescriptorSetPool::NextFrame(){
    m_AllocatedThisFrame = 0;
}