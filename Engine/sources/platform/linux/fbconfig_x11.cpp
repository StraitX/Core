#include "platform/linux/fbconfig_x11.hpp"
#include "platform/linux/display_x11.hpp"
#include <X11/Xlib.h>
#include <GL/glx.h>


namespace StraitX{
namespace Linux{

FBConfigX11::FBConfigX11():
    m_Handle(nullptr)
{
    ::Display *display = DisplayX11::Instance().Handle();


    int configsCount = 0;
    GLXFBConfig *configs = glXGetFBConfigs(display,DefaultScreen(display),&configsCount);

    // in case if we have not find any FBConfig
    if(configsCount == 0){
        m_PixelFormat = PixelFormat();
        return;
    }

    int bestIndex = 0;
    float bestScore = -100;
    PixelFormat best;


    for(int i = 0; i<configsCount; i++){
        PixelFormat current;
        
        glXGetFBConfigAttrib(display, configs[i], GLX_RED_SIZE,     &current.Red);
        glXGetFBConfigAttrib(display, configs[i], GLX_GREEN_SIZE,   &current.Green);
        glXGetFBConfigAttrib(display, configs[i], GLX_BLUE_SIZE,    &current.Blue);
        glXGetFBConfigAttrib(display, configs[i], GLX_ALPHA_SIZE,   &current.Alpha);
        glXGetFBConfigAttrib(display, configs[i], GLX_DEPTH_SIZE,   &current.Depth);
        glXGetFBConfigAttrib(display, configs[i], GLX_STENCIL_SIZE, &current.Stencil);
        glXGetFBConfigAttrib(display, configs[i], GLX_SAMPLES,      &current.Samples);

        float score = float(current.Red)/8.f + float(current.Green)/8.f + float(current.Blue)/8.f + 
                float(current.Alpha)/8.f + float(current.Depth)/24.f + float(current.Stencil)/8.f + float(current.Samples)/4.f;
        
        if(score > bestScore){
            score = bestScore;
            bestIndex = i;
            best = current;
        }
    }


    m_Handle = configs[bestIndex];
    m_PixelFormat = best;

    XFree(configs);
}


FBConfigX11::FBConfigX11(const PixelFormat &desired):
    m_Handle(nullptr)
{
    ::Display *display = DisplayX11::Instance().Handle();

    int glxAttributes[]={
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , desired.Red,
		GLX_GREEN_SIZE      , desired.Green,
		GLX_BLUE_SIZE       , desired.Blue,
		GLX_ALPHA_SIZE      , desired.Alpha,
		GLX_DEPTH_SIZE      , desired.Depth,
		GLX_STENCIL_SIZE    , desired.Stencil,
		GLX_DOUBLEBUFFER    , True,
        GLX_SAMPLE_BUFFERS  , desired.Samples > 0 ? 1 : 0,
        GLX_SAMPLES         , desired.Samples,
        
		None // NULL Terminated arg array
    };

    int configsCount = 0;
    GLXFBConfig *configs = glXChooseFBConfig(display,DefaultScreen(display),glxAttributes,&configsCount);

    // in case if we have not find any suitable FBConfig
    if(configsCount == 0){
        m_PixelFormat = PixelFormat();
        return;
    }

    int bestIndex = 0;
    float bestScore = 0;
    PixelFormat best;


    for(int i = 0; i<configsCount; i++){
        PixelFormat current;
        
        glXGetFBConfigAttrib(display, configs[i], GLX_RED_SIZE,     &current.Red);
        glXGetFBConfigAttrib(display, configs[i], GLX_GREEN_SIZE,   &current.Green);
        glXGetFBConfigAttrib(display, configs[i], GLX_BLUE_SIZE,    &current.Blue);
        glXGetFBConfigAttrib(display, configs[i], GLX_ALPHA_SIZE,   &current.Alpha);
        glXGetFBConfigAttrib(display, configs[i], GLX_DEPTH_SIZE,   &current.Depth);
        glXGetFBConfigAttrib(display, configs[i], GLX_STENCIL_SIZE, &current.Stencil);
        glXGetFBConfigAttrib(display, configs[i], GLX_SAMPLES,      &current.Samples);

        float score = float(current.Red)/float(desired.Red) + float(current.Green)/float(desired.Green) + float(current.Blue)/float(desired.Blue) + 
                float(current.Alpha)/float(desired.Alpha) + float(current.Depth)/float(desired.Depth) + float(current.Stencil)/float(desired.Stencil) + float(current.Samples)/float(desired.Samples) - 7;
        
        if(score > bestScore){
            score = bestScore;
            bestIndex = i;
            best = current;
        }
    }


    m_Handle = configs[bestIndex];
    m_PixelFormat = best;

    XFree(configs);
}

bool FBConfigX11::IsValid() const{
    return m_Handle == nullptr ? false : true;
}

const PixelFormat &FBConfigX11::Pixel() const{
    return m_PixelFormat;
}

void* FBConfigX11::Handle(){
    return m_Handle;
}


}; //namespace Linux::
}; //namespace StraitX::
