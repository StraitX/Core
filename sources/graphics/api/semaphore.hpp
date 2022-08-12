#ifndef STRAITX_SEMAPHORE_HPP
#define STRAITX_SEMAPHORE_HPP

#include "core/types.hpp"
#include "core/noncopyable.hpp"
#include "core/move.hpp"

class Semaphore: public NonCopyable{
    static constexpr u64 s_NullHandle = 0;
private:
    u64 m_Handle = s_NullHandle;
public:
    static const Semaphore Null;

    Semaphore(){
        Create();
    }
private:
    Semaphore(nullptr_t):
        m_Handle(s_NullHandle)
    {}
public:
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