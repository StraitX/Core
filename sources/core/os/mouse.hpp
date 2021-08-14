#ifndef STRAITX_MOUSE_HPP
#define STRAITX_MOUSE_HPP

#include "core/types.hpp"
#include "core/math/vector2.hpp"

class Window;

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

    Vector2s GlobalPosition();

    Vector2s RelativePosition(const Window &window);

    void SetGlobalPosition(const Vector2s &position);

    void SetVisible(bool is_visible);

}// namespace Mouse::

#endif // STRAITX_MOUSE_HPP