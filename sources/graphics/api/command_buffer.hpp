#ifndef STRAITX_COMMAND_BUFFER_HPP
#define STRAITX_COMMAND_BUFFER_HPP

#include "core/noncopyable.hpp"

class CommandBuffer;

class CommandBufferPool: public NonCopyable{
public:
    CommandBufferPool() = default;

    virtual ~CommandBufferPool() = default;

    virtual CommandBuffer *Alloc() = 0;

    virtual void Free(CommandBuffer *buffer) = 0;

    //virtual void Reset() = 0

    static CommandBufferPool *Create();
};

class CommandBuffer: public NonCopyable{
public:
    CommandBuffer() = default;

    virtual ~CommandBuffer() = default;

    virtual void Begin() = 0;

    virtual void End() = 0;

    virtual void Reset() = 0;

};

#endif//STRAITX_COMMAND_BUFFER_HPP