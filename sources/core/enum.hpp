#ifndef SX_ENUM_HPP
#define SX_ENUM_HPP

#include "core/array.hpp"
#include "core/string_view.hpp"

namespace Details{

constexpr size_t CountChars(const char* string, char search) {
	size_t count = 0;
	while (*string) {
		char ch = *string;
		string++;

		if(ch == search)
			count ++;
	}
	return count;
}

template<size_t N>
constexpr void SplitElementsIntoArray(const char *string, Array<StringView, N> &array) {
	int splited = 0;
	int start = 0;

	for (int i = 0; *string; i++) {
		char ch = string[i];

		if(!ch)
			break;

		if (ch == ',') {
			array.Data()[splited++] = StringView(&string[start], i - start );
			i += 2;
			start = i;
		}
	}

	array.Data()[splited] = StringView(&string[start]);
}

constexpr bool EnsureNoValueAssignment(const char* string) {
	return CountChars(string, '=') == 0;
}

template<typename EnumType, size_t SizeValue>
struct EnumNamesArray : Array<StringView, SizeValue> {
	constexpr const StringView &operator[](EnumType Value)const{
		return Data()[(size_t)Value];
	}
};

}//namespace Details::

#define GEN_ENUM_NAMES_NAMED(EnumType, NamesArrayName, ...) \
    static constexpr auto NamesArrayName = [](){ \
        Details::EnumNamesArray<EnumType, Details::CountChars(#__VA_ARGS__, ',') + 1> result{}; \
		Details::SplitElementsIntoArray(#__VA_ARGS__, result); \
		return result; \
	}();

#define GEN_ENUM_VALUES_NAMED(EnumType, ValuesArrayName, ...) \
    static constexpr EnumType ValuesArrayName[] = {__VA_ARGS__}; 

#define GEN_ENUM_MEMBERS_STRING_NAMED(EnumStringName, ...) \
    static constexpr const char *EnumStringName = #__VA_ARGS__; 

#define GEN_ENUM_SIZE_NAMED(EnumSizeName, ...) \
    static constexpr size_t EnumSizeName = Details::CountChars(#__VA_ARGS__, ',') + 1; 

#define GEN_ENUM_METADATA(EnumType, ...) \
    GEN_ENUM_VALUES_NAMED(EnumType, EnumType##s, __VA_ARGS__) \
    GEN_ENUM_NAMES_NAMED(EnumType, EnumType##Names,__VA_ARGS__) \
	GEN_ENUM_SIZE_NAMED(EnumType##Size, __VA_ARGS__)

#define SX_GEN_ENUM_START(EnumName) \
class EnumName{ \
    using ThisEnumType = EnumName; 
#define SX_GEN_ENUM_MEMBERS_AND_END(...) \
public: \
    enum Type{ \
        __VA_ARGS__ \
	}; \
private: \
    Type m_Value;\
public: \
	\
    constexpr ThisEnumType(Type value): \
        m_Value(value) \
	{} \
    \
	constexpr size_t AsInt()const{ \
	    return (size_t)m_Value; \
	} \
	constexpr Type AsType()const{ \
	    return m_Value; \
	} \
    constexpr operator Type()const { \
	    return AsType(); \
	} \
    constexpr StringView Name()const{ \
        return Names[m_Value]; \
	} \
    static_assert(Details::EnsureNoValueAssignment(#__VA_ARGS__), "StraitX Enum does not support explicit enum value assignment"); \
	GEN_ENUM_MEMBERS_STRING_NAMED(Summary, __VA_ARGS__) \
	GEN_ENUM_VALUES_NAMED(Type, Values, __VA_ARGS__) \
	GEN_ENUM_NAMES_NAMED(ThisEnumType, Names, __VA_ARGS__) \
	GEN_ENUM_SIZE_NAMED(Size, __VA_ARGS__) \
	\
}; 


#define SX_ENUM(EnumType) SX_GEN_ENUM_START(EnumType) SX_GEN_ENUM_MEMBERS_AND_END


#endif //SX_ENUM_HPP