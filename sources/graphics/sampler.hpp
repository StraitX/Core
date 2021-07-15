#ifndef STRAITX_SAMPLER_HPP
#define STRAITX_SAMPLER_HPP

#include "platform/types.hpp"
#include "platform/compiler.hpp"
#include "core/assert.hpp"
#include "core/move.hpp"
#include "graphics/gpu_configuration.hpp"

enum class FilteringMode: u8{
    Linear,
    Nearest
};

enum class WrapMode: u8{
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

struct SamplerProperties{
    FilteringMode MinFiltering = FilteringMode::Linear;
    FilteringMode MagFiltering = FilteringMode::Linear;
    WrapMode WrapU = WrapMode::ClampToEdge;
    WrapMode WrapV = WrapMode::ClampToEdge;
    WrapMode WrapS = WrapMode::ClampToEdge;
};

namespace GL{
struct SamplerImpl;
}//namespace GL::

namespace Vk{
struct SamplerImpl;
}//namespace Vk::

class GraphicsAPILoader;

class Sampler{
public:
    struct VTable{
        using NewProc    = void (*)(Sampler &sampler, SamplerProperties props);
        using DeleteProc = void (*)(Sampler &sampler);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    GPUResourceHandle m_Handle;

    static VTable s_VTable;
    friend class GraphicsAPILoader;
    friend class Vk::SamplerImpl;
    friend class GL::SamplerImpl;
public:
    Sampler() = default;

    Sampler(Sampler &&other);

    ~Sampler();

    Sampler &operator=(Sampler &&other);

    void New(SamplerProperties props);

    void Delete();

    GPUResourceHandle Handle()const;

    bool IsEmpty()const;
};

SX_INLINE Sampler::Sampler(Sampler &&other){
    *this = Move(other);
}

SX_INLINE Sampler::~Sampler(){
    if(!IsEmpty())
        Delete();
}

SX_INLINE Sampler &Sampler::operator=(Sampler &&other){
    SX_CORE_ASSERT(IsEmpty(), "Sampler: Can't move into non-empty object");
    m_Handle = other.m_Handle;
    other.m_Handle = {};
    return *this;
}

SX_INLINE void Sampler::New(SamplerProperties props){
    s_VTable.New(*this, props);
}

SX_INLINE void Sampler::Delete(){
    s_VTable.Delete(*this);
    m_Handle.U64 = 0;
}

SX_INLINE GPUResourceHandle Sampler::Handle()const{
    return m_Handle;
}

SX_INLINE bool Sampler::IsEmpty()const{
    return m_Handle.U64 == 0;
}

#endif//STRAITX_SAMPLER_HPP