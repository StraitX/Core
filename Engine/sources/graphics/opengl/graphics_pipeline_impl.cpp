#include <new>
#include "platform/memory.hpp"
#include "core/log.hpp"
#include "graphics/opengl/shader_impl.hpp"
#include "graphics/opengl/graphics_pipeline_impl.hpp"

namespace StraitX{
namespace GL{

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
    Topology(props.Topology),
    Rasterization(props.Rasterization),
    BlendFunc(props.BlendFunc),
    SrcBlendFactor(props.SrcBlendFactor),
    DstBlendFactor(props.DstBlendFactor)
{
    //OpenGL...
    (void)owner;
    glGenVertexArrays(1, &VertexArray);
    glBindVertexArray(VertexArray);

    for(size_t i = 0; i<props.VertexAttributes.Size(); ++i){
        glVertexAttribFormat(i, ElementsCount(props.VertexAttributes[i]),ElementType(props.VertexAttributes[i]),false, 0 /*offset from the begining of the buffer*/);
        glVertexAttribBinding(i,i);
    }

    Program = glCreateProgram();
    for(const auto &shader: props.Shaders)
        glAttachShader(Program, static_cast<const GL::ShaderImpl*>(shader)->Handle);
    glLinkProgram(Program);

    i32 link_status;
    glGetProgramiv(Program, GL_LINK_STATUS, &link_status);

    if(link_status != GL_TRUE){
        i32 link_log_length;
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &link_log_length);

        char *log = (char*)alloca(link_log_length + 1);
        log[link_log_length] = 0;
        glGetProgramInfoLog(Program, link_log_length, &link_log_length, log);

        LogError("GL: GraphicsPipeline: Shader Linkage failed: %", log);

        Valid = false;
        return;
    }

}

GraphicsPipelineImpl::~GraphicsPipelineImpl(){
    glDeleteVertexArrays(1, &VertexArray);
    glDeleteProgram(Program);
}

bool GraphicsPipelineImpl::IsValid(){
    return Valid;
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