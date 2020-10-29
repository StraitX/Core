#ifndef STRAITX_PLATFORM_HPP
#define STRAITX_PLATFORM_HPP

namespace StraitX{

class Platform{
public:
    static int Initialize();
    static int Finalize();
};

}; // namespace StraitX::

#endif //STRAITX_PLATFORM_HPP