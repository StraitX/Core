#ifndef STRAITX_MOUSE_HPP
#define STRAITX_MOUSE_HPP

#include "platform/types.hpp"

namespace StraitX{
    class Window;
};

namespace StraitX{
namespace Mouse{

    enum Button{
        Unknown = 0,
        Left,
        Right,
        Middle,
        XButton1,   //The first extra mouse button
        XButton2,   //The second extra mouse button

        ButtonCount
    };

    bool IsButtonPressed(Button button);

    Point GlobalPosition();

    Point RelativePosition(const Window &window);

    void SetGlobalPosition(const Point &position);

    void SetVisible(bool is_visible, const Window &window);

}; // namespace Mouse::


}; // namespace StraitX::

#endif // STRAITX_MOUSE_HPP