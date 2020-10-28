#ifndef STRAITX_APPLICATION_HPP
#define STRAITX_APPLICATION_HPP

#include "main/engine.hpp"

namespace StraitX{

class Application{
private:
public:
    Application() = default;
    virtual ~Application() = default;

    virtual void OnInitialize(){};

    virtual void OnFinalize(){};

};

}; // namespace StraitX::


#endif // STRAITX_APPLICATION_HPP