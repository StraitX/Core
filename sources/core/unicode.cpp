#include "core/unicode.hpp"
#include "core/assert.hpp"

size_t Codepoint::Encode(u8* utf8)const {
	SX_ASSERT(false);
	return 0;
}

size_t Codepoint::Encode(u16* utf16)const {
	SX_ASSERT(false);
	return 0;
}

size_t Codepoint::Encode(u32* utf32)const {
	*utf32 = Value;
	return 1;
}

//XXX: Validation
size_t Codepoint::Decode(const u8* utf8) {
	if ((utf8[0] & 0b10000000) == 0) {
		Value = utf8[0] & 0b01111111;
		return 1;
	} else if ((utf8[0] & 0b11100000) == 0b11000000) {
		Value = (utf8[0] & 0b00011111) | ((utf8[1] & 0b00111111) << 5);
		return 2;
	} else if ((utf8[0] & 0b11110000) == 0b11100000) {
		Value = (utf8[0] & 0b00001111) | ((utf8[1] & 0b00111111) << 4) | ((utf8[2] & 0b00111111) << 10);
		return 3;
	} else if ((utf8[0] & 0b11111000) == 0b11110000) {
		Value = (utf8[0] & 0b00000111) | ((utf8[1] & 0b00111111) << 3) | ((utf8[2] & 0b00111111) << 9) | ((utf8[3] & 0b00111111) << 15);
		return 4;
	} else {
		return 0;
	}
}

size_t Codepoint::Decode(const u16* utf16) {
	SX_ASSERT(false);
	return 0;
}

size_t Codepoint::Decode(const u32* utf32) {
	Value = *utf32;
	return 1;
}
