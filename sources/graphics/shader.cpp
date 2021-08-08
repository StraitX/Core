#include "core/os/file.hpp"
#include "core/os/memory.hpp"
#include "core/log.hpp"
#include "graphics/shader.hpp"

Shader::VTable Shader::s_VTable;

Shader *Shader::New(Shader::Type type, Shader::Lang lang, const char *filename){
    File file;
    u8 *sources;
    u32 length;
    if(file.Open(filename, File::Mode::Read, false)){
        length = file.Size();
        sources = (u8*)Memory::Alloc(length);

        file.Read(sources, length);

        file.Close();

        auto shader = New(type, lang, sources, length);

        Memory::Free(sources);
        return shader;
    }else{
        LogError("Can't open shader file '%'", filename);
        // Zero size shader code is going to produce invalid shader object
        return New(type, lang, nullptr, 0);
    }
}