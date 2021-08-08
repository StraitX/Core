#ifndef STRAITX_MOUSE_HPP
#define STRAITX_MOUSE_HPP

#include "core/types.hpp"

class PlatformWindow;

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

    Point2i GlobalPosition();

    Point2i RelativePosition();

    void SetGlobalPosition(const Point2i &position);

    void SetVisible(bool is_visible);

}// namespace Mouse::

#endif // STRAITX_MOUSE_HPP