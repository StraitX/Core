#ifndef STRAITX_NONCOPYABLE_HPP
#define STRAITX_NONCOPYABLE_HPP

struct NonCopyable{

    NonCopyable() = default;

    NonCopyable(const NonCopyable &other) = delete;

    NonCopyable &operator=(const NonCopyable &other) = delete;

    ~NonCopyable() = default;

};

#endif//STRAITX_NONCOPYABLE_HPP 