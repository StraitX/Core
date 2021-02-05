#ifndef STRAITX_GRAPHICS_API_LOADER_HPP
#define STRAITX_GRAPHICS_API_LOADER_HPP

#include "platform/result.hpp"
#include "graphics/api/graphics_api.hpp"

namespace StraitX{

class GraphicsAPILoader{
public:
    static Result Load(GraphicsAPI::API api);
};

}//namespace StraitX::

#endif//STRAITX_GRAPHICS_API_LOADER_HPP