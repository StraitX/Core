#ifndef STRAITX_WINDOW_X11_HPP
#define STRATIX_WINDOW_X11_HPP


namespace StraitX{
namespace Linux{

class WindowX11{
private:
    unsigned long mHandle;
    int mScreenIndex;
public:
    WindowX11(int width, int height);
    ~WindowX11();

    void SetTitle(const char *title);
};

}; //namespace Linux::
}; //namespace StraitX::

#endif // STRAITX_WINDOW_X11_HPP