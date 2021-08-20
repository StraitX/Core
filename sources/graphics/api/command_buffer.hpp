#ifndef STRAITX_COMMAND_BUFFER_HPP
#define STRAITX_COMMAND_BUFFER_HPP

#include "core/noncopyable.hpp"
#include "graphics/api/texture.hpp"

class CommandBuffer;

class CommandPool: public NonCopyable{
public:
    CommandPool() = default;

    virtual ~CommandPool() = default;

    virtual CommandBuffer *Alloc() = 0;

    virtual void Free(CommandBuffer *buffer) = 0;

    //virtual void Reset() = 0

    static CommandPool *Create();
};

class CommandBuffer: public NonCopyable{
public:
    CommandBuffer() = default;

    virtual ~CommandBuffer() = default;

    virtual void Begin() = 0;

    virtual void End() = 0;

    virtual void Reset() = 0;

    virtual void ChangeLayout(Texture2D *texture, TextureLayout new_layout) = 0;

};

#endif//STRAITX_COMMAND_BUFFER_HPP