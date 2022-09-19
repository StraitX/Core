#ifndef STRAITX_SEMAPHORE_HPP
#define STRAITX_SEMAPHORE_HPP

#include "core/types.hpp"
#include "core/move.hpp"
#include "graphics/api/graphics_resource.hpp"

class Semaphore: public GraphicsResource{
    static constexpr u64 s_NullHandle = 0;

    struct NullSemaphore {};
private:
    u64 m_Handle = s_NullHandle;
public:
    static const NullSemaphore Null;

    Semaphore(){
        Create();
    }

    Semaphore(NullSemaphore):
        m_Handle(s_NullHandle)
    {}

    Semaphore(Semaphore &&other)noexcept{
        *this = Move(other);
    }

    ~Semaphore(){
        if(!IsNull())
            Destroy();
    }

    Semaphore &operator=(Semaphore &&other)noexcept{
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

    void WaitFor()const;
private:
    void Create();

    void Destroy();
};


#endif//STRAITX_SEMAPHORE_HPP