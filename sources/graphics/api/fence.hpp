#ifndef STRAITX_FENCE_HPP
#define STRAITX_FENCE_HPP

#include "core/types.hpp"
#include "core/move.hpp"
#include "graphics/api/graphics_resource.hpp"

class Fence: public GraphicsResource{
    static constexpr u64 s_NullHandle = 0;

    struct NullFence {};
private:
    u64 m_Handle = s_NullHandle;
public:
    static const NullFence Null;
public:
    Fence(){
        Create();
    }

    Fence(NullFence):
        m_Handle(s_NullHandle)
    {}

    Fence(Fence &&other)noexcept{
        *this = Move(other);
    }

    ~Fence(){
        if(!IsNull())
            Destroy();
    }

    Fence &operator=(Fence &&other)noexcept{
        if(!IsNull())
            Destroy();
        
        m_Handle = other.m_Handle;
        other.m_Handle = s_NullHandle;
    
        return *this;
    }

    bool IsNull()const{
        return m_Handle == s_NullHandle;
    }

    u64 Handle()const{
        return m_Handle;
    }

    void WaitFor();

    void Reset();

    void WaitAndReset(){
        WaitFor();
        Reset();
    }

    bool IsSignaled()const;

    void Signal();
private:
    void Create();

    void Destroy();
};


#endif//STRAITX_FENCE_HPP