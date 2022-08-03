#include "graphics/color.hpp"
#include "core/printer.hpp"

const Color Color::Red        = {1.f, 0, 0, 1.f};
const Color Color::Green      = {0, 1.f, 0, 1.f};
const Color Color::Blue       = {0, 0, 1.f, 1.f};
const Color Color::Yellow     = {1.f, 1.f, 0, 1.f};
const Color Color::Cyan       = {0, 1.f, 1.f, 1.f};
const Color Color::Magenta    = {1.f, 0, 1.f, 1.f};
const Color Color::Orange     = {1.f, 0.5f, 0, 1.f};
const Color Color::Mint       = {0, 1.f, 0.5f, 1.f};
const Color Color::LightBlue  = {0.2, 0.8f, 1.f, 1.f};
const Color Color::Black      = {0, 0, 0, 1.f};
const Color Color::White      = {1.f, 1.f, 1.f, 1.f};
const Color Color::Transparent= {1.f, 1.f, 1.f, 0.f};

template<>
struct Printer<Color> {
	static void Print(const Color& value, StringWriter &writer) {
		Printer<char>::Print('(', writer);
		Printer<float>::Print(value.R, writer);
		Printer<char>::Print(',', writer);
		Printer<float>::Print(value.G, writer);
		Printer<char>::Print(',', writer);
		Printer<float>::Print(value.B, writer);
		Printer<char>::Print(',', writer);
		Printer<float>::Print(value.A, writer);
		Printer<char>::Print(')', writer);
	}
};