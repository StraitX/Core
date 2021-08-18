#ifndef STRAITX_SEMAPHORE_HPP
#define STRAITX_SEMAPHORE_HPP

#include "core/types.hpp"
#include "core/noncopyable.hpp"
#include "core/move.hpp"

class Semaphore: public NonCopyable{
private:
    u64 Handle;
    static constexpr u64 s_NullHandle = 0;
public:
    static const Semaphore Null;

    Semaphore(){
        Create();
    }

    Semaphore(Semaphore &&other)noexcept{
        *this = Move(other);
    }

    ~Semaphore(){
        Destroy();
    }

    Semaphore &operator=(Semaphore &&other)noexcept{
        if(!IsNull())
            Destroy();
        
        Handle = other.Handle;
        other.Handle = s_NullHandle;
    
        return *this;
    }

    bool IsNull()const{
        return Handle == s_NullHandle;
    }
private:
    void Create();

    void Destroy();
};


#endif//STRAITX_SEMAPHORE_HPP