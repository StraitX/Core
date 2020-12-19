#ifndef STRAITX_NONCOPYABLE_HPP
#define STRAITX_NONCOPYABLE_HPP

namespace StraitX{

struct NonCopyable{

    NonCopyable() = default;

    NonCopyable(const NonCopyable &other) = delete;

    NonCopyable &operator=(const NonCopyable &other) = delete;

    ~NonCopyable() = default;

};

};//namespace StraitX::

#endif//STRAITX_NONCOPYABLE_HPP 