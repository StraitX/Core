#ifndef STRAITX_GPU_HPP
#define STRAITX_GPU_HPP

#include "core/result.hpp"
#include "core/span.hpp"
#include "graphics/api/command_buffer.hpp"
#include "graphics/api/semaphore.hpp"
#include "graphics/api/fence.hpp"

enum class GPUType{
    Unknown = 0,
    Virtual,
    Integrated,
    Discrete
};

enum class GPUVendor{
    Unknown = 0,
    Microsoft,
    Nvidia,
    Intel,
    AMD
};

struct GPUImpl{
    GPUType Type = GPUType::Unknown;
    GPUVendor Vendor = GPUVendor::Unknown;

    virtual Result Initialize() = 0;

    virtual void Finalize() = 0;

    virtual void Execute(CommandBuffer *buffer, Span<u64> wait_semaphore_handles, Span<u64> signal_semaphore_handles, const Fence &signal_fence) = 0;

    virtual void SyncSemaphores(ConstSpan<Semaphore> wait, ConstSpan<Semaphore> signal) = 0;
};

class GPU{
private:
    static GPUImpl *s_Impl;
private:
    static Result Initialize();

    static void Finalize();

    friend int GuardedMain(int argc, const char **argv);
public:
    static GPUType Type();

    static GPUVendor Vendor();

    static bool IsInitialized(){
        return s_Impl;
    }

    static void Execute(CommandBuffer *buffer, Span<u64> wait_semaphore_handles = {}, Span<u64> signal_semaphore_handles = {}, const Fence &signal_fence = Fence::Null);

    static void Execute(CommandBuffer *buffer, ConstSpan<Semaphore> wait_semaphores = {}, ConstSpan<Semaphore> signal_semaphores = {}, const Fence &signal_fence = Fence::Null);

    static void Execute(CommandBuffer *buffer, const Fence &signal_fence = Fence::Null);

    static void Execute(CommandBuffer *buffer, const Semaphore &wait_semaphore, const Semaphore &signal_semaphore, const Fence &signal_fence = Fence::Null);

    static void SyncSemaphores(ConstSpan<Semaphore> wait, ConstSpan<Semaphore> signal);

    static void SyncSemaphores(const Semaphore &wait, ConstSpan<Semaphore> signal);

    static void SyncSemaphores(ConstSpan<Semaphore> wait, const Semaphore &signal);

    static void SyncSemaphores(const Semaphore &wait, const Semaphore &signal);
};

#endif//STRAITX_GPU_HPP