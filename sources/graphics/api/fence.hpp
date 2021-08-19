#ifndef STRAITX_FENCE_HPP
#define STRAITX_FENCE_HPP

#include "core/types.hpp"
#include "core/noncopyable.hpp"
#include "core/move.hpp"

class Fence: public NonCopyable{
    static constexpr u64 s_NullHandle = 0;
private:
    u64 m_Handle = s_NullHandle;
public:
    static const Fence Null;

    Fence(){
        Create();
    }

    Fence(Fence &&other)noexcept{
        *this = Move(other);
    }

    ~Fence(){
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
private:
    void Create();

    void Destroy();
};


#endif//STRAITX_FENCE_HPP