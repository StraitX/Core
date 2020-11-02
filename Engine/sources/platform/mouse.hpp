#ifndef STRAITX_MOUSE_HPP
#define STRAITX_MOUSE_HPP

#include "platform/types.hpp"


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
    //static Vector2i RelativePosition(const Window &window);

}; // namespace Mouse::


}; // namespace StraitX::

#endif // STRAITX_KEYBOARD_HPP