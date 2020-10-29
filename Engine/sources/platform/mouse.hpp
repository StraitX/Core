#ifndef STRAITX_MOUSE_HPP
#define STRAITX_MOUSE_HPP

#include "core/math/vector2.hpp"



namespace StraitX{
namespace Mouse{

    enum Button{
        Left = 0,
        Right,
        Middle,
        XButton1,   //The first extra mouse button
        XButton2,   //The second extra mouse button
        
        ButtonCount
    };

    bool IsButtonPressed(Button button);

    //Vector2i GlobalPosition();
    //static Vector2i RelativePosition(const Window &window);

}; // namespace Mouse::


}; // namespace StraitX::

#endif // STRAITX_KEYBOARD_HPP