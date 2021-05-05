#include "graphics/api/swapchain.hpp"

namespace StraitX{

Swapchain::VTable Swapchain::s_VTable;

Swapchain::Swapchain(const SwapchainProperties &props){
    (void)0;
    CoreAssert(props.DepthFormat == TextureFormat::Unknown || props.DepthFormat == TextureFormat::Depth32,"Swapchain: Only depth32 depth buffers are supported");
}

}//namespace StraitX::