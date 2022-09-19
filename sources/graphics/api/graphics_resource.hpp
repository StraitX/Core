#ifndef STRAITX_GRAPHICS_RESOURCE_HPP
#define STRAITX_GRAPHICS_RESOURCE_HPP

#include "core/noncopyable.hpp"

class GraphicsResource: public NonCopyable{
private:
	static size_t s_GraphicsResourceCount;
public:
	GraphicsResource();

	GraphicsResource(GraphicsResource&& other)noexcept;

	~GraphicsResource();

	GraphicsResource &operator=(GraphicsResource&& other)noexcept;
};

#endif//STRAITX_GRAPHICS_RESOURCE_HPP