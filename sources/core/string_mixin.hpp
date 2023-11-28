#pragma once

#include "core/unicode.hpp"
#include "core/span.hpp"
#include "core/assert.hpp"

template <typename StringType>
struct StringMixin {

	const char* Data()const {
		return ((StringType*)this)->Data();
	}

	size_t CodeunitsCount()const {
		return ((StringType*)this)->CodeunitsCount();
	}

    size_t Size()const {
        return CodeunitsCount();
    }

    UnicodeString Unicode()const {
        return UnicodeString(Data(), Size());
    }
    
    ConstSpan<char> Span()const {
        return {Data(), Size()};
    }

	bool IsEmpty()const {
        return !Size();
    }

    size_t CodepointsCount()const {
        size_t counter = 0;
        for (u32 ch : Unicode())
            counter++;
        return counter;
    }

    size_t Count(char ch)const {
        size_t count = 0;
        for (size_t i = 0; i<CodeunitsCount(); i++) {
            if(Data()[i] == ch)
                count++; 
        }
        return count;
    }

    size_t Count(u32 ch)const {
        size_t count = 0;
        for (u32 character : Unicode()) {
            if(character == ch)
                count++; 
        }
        return count;
    }

    StringType Sub(size_t start, size_t count) {
        SX_CORE_ASSERT(start + count <= Size(), "Out of range SubString operation");
        return {&Data()[start], count};
    }

    operator ConstSpan<char>()const {
        return Span();
    }

    operator UnicodeString()const {
		return Unicode();
	}

	static constexpr size_t StaticCodeunitsCount(const char* string) {
		if(!string)return 0;

		size_t counter = 0;
		while (*string++) 
			counter++;
		return counter;
	}
};