#include "platform/glcontext.hpp"
#include "platform/window.hpp"

namespace StraitX{


GLContext::GLContext(Window &window):
    m_Impl(window.Impl())
{}

Error GLContext::Create(const FBConfig &config, const Version &version){
    return m_Impl.Create(config.Impl(), version);
}

void GLContext::Destory(){
    m_Impl.Destory();
}

Error GLContext::MakeCurrent(){
    return m_Impl.MakeCurrent();
}

void GLContext::SwapBuffers(){
    m_Impl.SwapBuffers();
}


}; // namespace StraitX::