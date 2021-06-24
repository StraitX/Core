#include "core/assert.hpp"
#include "graphics/api/graphics_context.hpp"

namespace StraitX{

const char *GraphicsAPI::Name()const{
	static const char *s_Names[]={
		"None",
		"Vulkan",
		"OpenGL"
	};

	SX_CORE_ASSERT(m_Value < lengthof(s_Names),"GraphicsAPI: Unnamed API");

	return s_Names[m_Value];
}


GraphicsAPI GraphicsContext::s_CurrentAPI = GraphicsAPI::None;
GraphicsContext* GraphicsContext::s_Instance = nullptr;


}//namespace StraitX::