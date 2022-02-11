#include "core/unicode.hpp"
#include "core/assert.hpp"

size_t Codepoint::Encode(char* utf8)const {
	return Encode((u8*)utf8);
}

size_t Codepoint::Encode(u8* utf8)const {
	if ((Value & ~0b0111'1111) == 0) {
		utf8[0] = Value;
		return 1;
	}else if((Value & ~0b0111'1111'1111) == 0){
		utf8[0] = (Value >> 6 & 0b0001'1111) | 0b1100'0000;
		utf8[1] = (Value      & 0b0011'1111) | 0b1000'0000;
		return 2;
	}else if ((Value & ~0b1111'1111'1111'1111) == 0) {
		utf8[0] = (Value >> 12 & 0b0000'1111) | 0b1110'0000;
		utf8[1] = (Value >> 6  & 0b0011'1111) | 0b1000'0000;
		utf8[2] = (Value       & 0b0011'1111) | 0b1000'0000;
		return 3;
	}else if ((Value & ~0b1111'1111'1111'1111) == 0) {
		utf8[0] = (Value >> 18 & 0b0000'0111) | 0b1111'0000;
		utf8[1] = (Value >> 12 & 0b0011'1111) | 0b1000'0000;
		utf8[1] = (Value >> 6  & 0b0011'1111) | 0b1000'0000;
		utf8[2] = (Value       & 0b0011'1111) | 0b1000'0000;
		return 4;
	}
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

size_t Codepoint::Decode(const char* utf8) {
	return Decode((const u8*)utf8);
}
//XXX: Validation
size_t Codepoint::Decode(const u8* utf8) {
	if ((utf8[0] & 0b10000000) == 0) {
		Value = (utf8[0] & 0b01111111);
		return 1;
	} else if ( (utf8[0] & 0b11100000) == 0b11000000) {
		Value = (utf8[0] & 0b00011111) << 6  | ((utf8[1] & 0b00111111));
		return 2;
	} else if ( (utf8[0] & 0b11110000) == 0b11100000) {
		Value = (utf8[0] & 0b00001111) << 12 | ((utf8[1] & 0b00111111) << 6)  | ((utf8[2] & 0b00111111));
		return 3;
	} else if ( (utf8[0] & 0b11111000) == 0b11110000) {
		Value = (utf8[0] & 0b00000111) << 18 | ((utf8[1] & 0b00111111) << 12) | ((utf8[2] & 0b00111111) << 6 ) | ((utf8[3] & 0b00111111));
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
