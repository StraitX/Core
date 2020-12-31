#include "graphics/vulkan/logical_device.hpp"
#include "platform/file.hpp"
#include "graphics/vulkan/shader.hpp"
#include <vulkan/vulkan_core.h>

namespace StraitX{
namespace Vk{

Result Shader::Create(Vk::LogicalDevice *owner, const char *filename, VkShaderStageFlagBits stage){
    Owner = owner;
    Stage = stage;
    
    File shader_file;
    if(shader_file.Open(filename, File::Mode::Read) != Result::Success)
        return Result::NotFound;

    size_t shader_file_size = shader_file.Size();

    u8 *shader_code = new u8[shader_file_size];
    if(shader_file.Read(shader_code, shader_file_size) != shader_file_size)
        return Result::Unavailable;

    shader_file.Close();

    VkShaderModuleCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.codeSize = shader_file_size;
    info.pCode = (u32*)shader_code;

    auto res = vkCreateShaderModule(Owner->Handle, &info, nullptr, &Handle);
    delete[] shader_code;

    return ResultError(res != VK_SUCCESS);
}

void Shader::Destroy(){
    vkDestroyShaderModule(Owner->Handle, Handle, nullptr);
}

};//namespace Vk::
};//namespace StraitX::

