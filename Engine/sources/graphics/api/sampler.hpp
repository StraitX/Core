#ifndef STRAITX_SAMPLER_HPP
#define STRAITX_SAMPLER_HPP

#include "platform/types.hpp"
#include "platform/compiler.hpp"
#include "core/assert.hpp"
#include "core/move.hpp"
#include "graphics/api/gpu_configuration.hpp"

namespace StraitX{

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
#ifdef SX_DEBUG
    ~Sampler();
#endif
    Sampler &operator=(Sampler &&other);

    void New(SamplerProperties props);

    void Delete();

    GPUResourceHandle Handle()const;

    bool IsEmpty()const;
};

sx_inline Sampler::Sampler(Sampler &&other){
    *this = Move(other);
}

#ifdef SX_DEBUG
sx_inline Sampler::~Sampler(){
    CoreAssert(IsEmpty(), "Sampler: Delete() should be called before destruction");
}
#endif
sx_inline Sampler &Sampler::operator=(Sampler &&other){
    CoreAssert(IsEmpty(), "Sampler: Can't move into non-empty object");
    m_Handle = other.m_Handle;
    other.m_Handle = {};
    return *this;
}

sx_inline void Sampler::New(SamplerProperties props){
    s_VTable.New(*this, props);
}

sx_inline void Sampler::Delete(){
    s_VTable.Delete(*this);
    m_Handle.U64 = 0;
}

sx_inline GPUResourceHandle Sampler::Handle()const{
    return m_Handle;
}

sx_inline bool Sampler::IsEmpty()const{
    return m_Handle.U64 == 0;
}
};

#endif//STRAITX_SAMPLER_HPP