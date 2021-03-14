#include <new>
#include "platform/memory.hpp"
#include "core/log.hpp"
#include "core/string.hpp"
#include "graphics/opengl/shader_impl.hpp"
#include "graphics/opengl/graphics_pipeline_impl.hpp"
#include "graphics/opengl/graphics_api_impl.hpp"

namespace StraitX{
namespace GL{

void DescriptorSet::Bind() const{
    for(size_t i = 0; i<Bindings.Size(); ++i){
        switch (Bindings[i].Type){
        case ShaderBindingType::UniformBuffer: 
            glBindBufferBase(GL_UNIFORM_BUFFER, i, Bindings[i].Data.UniformBuffer); 
            break;
        }
    }
}

GLenum GraphicsPipelineImpl::s_BlendFunctionTable[]={
    GL_FUNC_ADD
};
GLenum GraphicsPipelineImpl::s_BlendFactorTable[]={
    GL_ZERO,
    GL_ONE,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    GL_SRC_ALPHA,
    GL_DST_ALPHA
};

GLenum GraphicsPipelineImpl::s_TopologyTable[] = {
    GL_POINTS,
    GL_LINES,
    GL_LINE_STRIP,
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP
};
GLenum GraphicsPipelineImpl::s_RasterizationModeTable[] = {
    GL_FILL,
    GL_POINT,
    GL_LINE
};

static u32 ElementsCount(VertexAttribute attribute){
    switch (attribute){
    case VertexAttribute::Int:
    case VertexAttribute::Uint:
    case VertexAttribute::Float:
        return 1;
    case VertexAttribute::Int2:
    case VertexAttribute::Uint2:
    case VertexAttribute::Float2:
        return 2;
    case VertexAttribute::Int3:
    case VertexAttribute::Uint3:
    case VertexAttribute::Float3:
        return 3;
    case VertexAttribute::Int4:
    case VertexAttribute::Uint4:
    case VertexAttribute::Float4:
        return 4;
    }
    CoreAssert(false, "GL: GraphicsPipeline: Unknown Elements Count");
    return 0;
}

static GLenum ElementType(VertexAttribute attribute){
    switch (attribute){
    case VertexAttribute::Int:
    case VertexAttribute::Int2:
    case VertexAttribute::Int3:
    case VertexAttribute::Int4:
        return GL_INT;
    case VertexAttribute::Uint:
    case VertexAttribute::Uint2:
    case VertexAttribute::Uint3:
    case VertexAttribute::Uint4:
        return GL_UNSIGNED_INT;
    case VertexAttribute::Float:
    case VertexAttribute::Float2:
    case VertexAttribute::Float3:
    case VertexAttribute::Float4:
        return GL_FLOAT;
    }
    CoreAssert(false, "GL: GraphicsPipeline: Unknown Element Type");
    return 0;
}

GraphicsPipelineImpl::GraphicsPipelineImpl(LogicalGPU &owner, const GraphicsPipelineProperties &props):
    GraphicsPipeline(props),
    AttributesStride(GraphicsPipeline::CalculateStride(props.VertexAttributes)),
    Topology(s_TopologyTable[(size_t)props.Topology]),
    Rasterization(s_RasterizationModeTable[(size_t)props.Rasterization]),
    FramebufferViewport(props.FramebufferViewport),
    BlendFunc(s_BlendFunctionTable[(size_t)props.BlendFunc]),
    SrcBlendFactor(s_BlendFactorTable[(size_t)props.SrcBlendFactor]),
    DstBlendFactor(s_BlendFactorTable[(size_t)props.DstBlendFactor])
{
    //OpenGL...
    (void)owner;

    for(size_t i = 0; i<props.ShaderBindings.Size(); ++i){
        if(props.ShaderBindings[i].Type == ShaderBindingType::UniformBuffer)
            Set.Bindings.Push({props.ShaderBindings[i].Type, 0/*Means no buffer is currently bound*/});
        else
            CoreAssert(false, "GL: GraphicsPipelineImpl: Unsupported shader binding type");
    }

    glGenVertexArrays(1, &VertexArray);
    glBindVertexArray(VertexArray);

    LogInfo("GL: VertexAttributes: Count: %", props.VertexAttributes.Size());


    for(size_t i = 0; i<props.VertexAttributes.Size(); ++i){
        Attributes.Push(props.VertexAttributes[i]);

        glEnableVertexAttribArray(i);
        
        if(GraphicsAPIImpl::Instance.LoadedOpenGLVersion.Major == 4 && GraphicsAPIImpl::Instance.LoadedOpenGLVersion.Minor >= 3){
            glVertexAttribFormat(i, ElementsCount(props.VertexAttributes[i]),ElementType(props.VertexAttributes[i]),false, 0 /*offset from the begining of the buffer*/);
            glVertexAttribBinding(i,i);
        }else{
            LogWarn("OpenGL: fallback to a compatible OpenGL profile")
        }
    }

    Program = glCreateProgram();
    for(const auto *shader: props.Shaders)
        glAttachShader(Program, static_cast<const GL::ShaderImpl*>(shader)->Handle);
    glLinkProgram(Program);

    LogInfo("GL: Shaders: Count: %", props.Shaders.Size());

    s32 link_status;
    glGetProgramiv(Program, GL_LINK_STATUS, &link_status);

    if(link_status != GL_TRUE){
        s32 link_log_length;
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &link_log_length);

        char *log = (char*)alloca(link_log_length + 1);
        log[link_log_length] = 0;
        glGetProgramInfoLog(Program, link_log_length, &link_log_length, log);

        LogError("GL: GraphicsPipeline: Shader Linkage failed: %", log);

        Valid = false;
    }

    for(const auto *shader: props.Shaders){
        const auto *shader_impl = static_cast<const GL::ShaderImpl*>(shader);
        if(shader_impl->RequiresPreprocess()){
            char *sources = (char *)alloca(shader_impl->SourcesLength + 1);
            int got = 0;
            glGetShaderSource(shader_impl->Handle,shader_impl->SourcesLength, &got, sources);
            sources[shader_impl->SourcesLength] = 0;

            LogInfo("Sources: %",sources);
            
            for(size_t i = 0;(sources = String::Find(sources,"uniform")); ++i){
                LogInfo("Size %, index %",shader_impl->UniformBindings.Size(),i);
                sources += 7;
                while(*sources == ' ')
                    ++sources;

                size_t name_len = 0;
                for(; *(sources + name_len) != '{' && *(sources + name_len) != ' ' && *(sources + name_len) != '\n';)
                    ++name_len;
                
                char *name = (char*)alloca(name_len + 1);
                Memory::Copy(sources, name, name_len);
                name[name_len] = 0;

                auto index = glGetUniformBlockIndex(Program, name);
                glUniformBlockBinding(Program,index, shader_impl->UniformBindings[i]);
            }
        }
    }
}

GraphicsPipelineImpl::~GraphicsPipelineImpl(){
    glDeleteVertexArrays(1, &VertexArray);
    glDeleteProgram(Program);
}

bool GraphicsPipelineImpl::IsValid(){
    return Valid;
}

void GraphicsPipelineImpl::Bind(size_t index, const GPUBuffer &uniform_buffer){
    Set.Bindings[index].Data.UniformBuffer = uniform_buffer.Handle().U32;

    glBindBufferBase(GL_UNIFORM_BUFFER, index, Set.Bindings[index].Data.UniformBuffer);
}

void GraphicsPipelineImpl::Bind()const{
    glPolygonMode(GL_FRONT_AND_BACK, Rasterization);
    glViewport(FramebufferViewport.x, FramebufferViewport.y, FramebufferViewport.Width, FramebufferViewport.Height);
    glBlendFunc(SrcBlendFactor, DstBlendFactor);
    glBlendEquation(BlendFunc);
    glBindVertexArray(VertexArray);
    glUseProgram(Program);

    Set.Bind();
}

void GraphicsPipelineImpl::BindVertexBuffer(u32 id)const{
    if(GraphicsAPIImpl::Instance.LoadedOpenGLVersion.Major == 4 && GraphicsAPIImpl::Instance.LoadedOpenGLVersion.Minor >= 3){
        size_t offset = 0;
        for(size_t i = 0; i<Attributes.Size(); ++i){
            glBindVertexBuffer(i, id, offset, AttributesStride);
            offset+=GraphicsPipeline::s_VertexAttributeSizeTable[(size_t)Attributes[i]];
        }
    }else{
        glBindBuffer(GL_ARRAY_BUFFER, id);
        size_t offset = 0;
        for(size_t i = 0; i<Attributes.Size(); ++i){
            glVertexAttribPointer(i, ElementsCount(Attributes[i]),ElementType(Attributes[i]), false, AttributesStride, (void*)offset);
            offset+=GraphicsPipeline::s_VertexAttributeSizeTable[(size_t)Attributes[i]];
        }
    }
}

void GraphicsPipelineImpl::BindIndexBuffer(u32 id)const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

GraphicsPipeline * GraphicsPipelineImpl::NewImpl(LogicalGPU &owner, const GraphicsPipelineProperties &props){
    return new(Memory::Alloc(sizeof(GL::GraphicsPipelineImpl))) GL::GraphicsPipelineImpl(owner, props);
}

void GraphicsPipelineImpl::DeleteImpl(GraphicsPipeline *pipeline){
    pipeline->~GraphicsPipeline();
    Memory::Free(pipeline);
}


}//namespace GL::
}//namespace StraitX::