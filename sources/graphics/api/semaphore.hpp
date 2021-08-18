#ifndef STRAITX_SEMAPHORE_HPP
#define STRAITX_SEMAPHORE_HPP

#include "core/types.hpp"
#include "core/noncopyable.hpp"

class Semaphore: public NonCopyable{   
private:
    u64 Handle = 0;
public:
    Semaphore();

    ~Semaphore();
};

#endif//STRAITX_SEMAPHORE_HPP