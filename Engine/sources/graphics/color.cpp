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

template<>
struct Printer<Color>{
	static void Print(const Color &value, void (*writer)(char, void*), void *writer_data);
};

void Printer<Color>::Print(const Color &value, void (*writer)(char, void*), void *writer_data){
	Printer<char>::Print('(', writer, writer_data);
	Printer<float>::Print(value.R, writer, writer_data);
	Printer<char>::Print(',', writer, writer_data);
	Printer<float>::Print(value.G, writer, writer_data);
	Printer<char>::Print(',', writer, writer_data);
	Printer<float>::Print(value.B, writer, writer_data);
	Printer<char>::Print(',', writer, writer_data);
	Printer<float>::Print(value.A, writer, writer_data);
	Printer<char>::Print(')', writer, writer_data);
}