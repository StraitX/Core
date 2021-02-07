#include <new>
#include "platform/memory.hpp"
#include "platform/opengl.hpp"
#include "graphics/opengl/gpu_context_impl.hpp"

namespace StraitX{
namespace GL{

void GPUContextImpl::BeginFrame(){
    (void)0;
}

void GPUContextImpl::EndFrame(){
    (void)0;
}

void GPUContextImpl::Submit(){
    glFlush();
}


void GPUContextImpl::SumbitAsync(){
    Submit();
}


GPUContext *GPUContextImpl::NewImpl(LogicalGPU &owner){
    // In OpenGL we don't have GPU distinctions so owner doesn't matter
    (void)owner;
    return new(Memory::Alloc(sizeof(GPUContextImpl))) GPUContextImpl();
}

void GPUContextImpl::DeleteImpl(GPUContext *context){
    context->~GPUContext();
    Memory::Free(context);
}

}//namespace GL::
}//namespace StraitX::