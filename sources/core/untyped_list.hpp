#ifndef STRAITX_UNTYPED_LIST_HPP
#define STRAITX_UNTYPED_LIST_HPP

#include <typeindex>
#include "core/types.hpp"
#include "core/move.hpp"
#include "core/templates.hpp"
#include "core/type_traits.hpp"
#include "core/allocators/allocator.hpp"
#include "core/noncopyable.hpp"

template<typename GeneralAllocatorType>
class UntypedListBase: private GeneralAllocatorType, public NonCopyable{
public:
	struct TypeInfo {
		size_t Size;
		std::type_index TypeIndex;
	};

	template<typename ActualType>
	struct Type { 
		TypeInfo GetTypeInfo() {
			return {
				sizeof(ActualType),
				std::type_index(typeid(ActualType))
			};
		}
	};

private:
	using DestructorType = void (*)(void* obj);
	using CopyConstructorType = void (*)(const void* src, void *dst);
	using MoveElseCopyConstructorType = void (*)(void* src, void *dst);
private:
	void *m_Memory;
	TypeInfo m_TypeInfo;
	size_t m_Size;
	size_t m_Capacity;
	DestructorType m_Destructor;
	CopyConstructorType m_CopyConstructor;
	MoveElseCopyConstructorType m_MoveElseCopyConstructor;
public:
	
	template<typename ActualType>
	UntypedListBase(Type<ActualType> type):
		m_Memory(nullptr),
		m_TypeInfo(type.GetTypeInfo()),
		m_Size(0),
		m_Capacity(0)
	{
		static_assert(!IsConst<ActualType>::Value && !IsVolatile<ActualType>::Value && !IsReference<ActualType>::Value, "UntypedList type can't be reference or cv-qualified");
		AssignDtor<ActualType>(nullptr);
		AssignCopyCtor<ActualType>(nullptr);
		AssignMoveElseCopyCtor<ActualType>(nullptr);

		SX_CORE_ASSERT(m_Destructor && m_MoveElseCopyConstructor, "Type should have a destructor and copy or move constructor");
	}

	UntypedListBase(UntypedListBase&& other)noexcept:
		m_Memory(nullptr),
		m_TypeInfo(Type<int>().GetTypeInfo()),
		m_Size(0),
		m_Capacity(0)
	{
		*this = Move(other);
	}

	~UntypedListBase() {
		Free();
	}

	// Move From UntypedListBase does not erase type from move src
	UntypedListBase& operator=(UntypedListBase&& other)noexcept{
		Free();

		m_Memory = other.m_Memory;
		m_TypeInfo = other.m_TypeInfo;
		m_Size = other.m_Size;
		m_Capacity = other.m_Capacity;
		m_Destructor = other.m_Destructor;
		m_CopyConstructor = other.m_CopyConstructor;
		m_MoveElseCopyConstructor = other.m_MoveElseCopyConstructor;

		other.m_Memory = nullptr;
		other.m_Size = 0;
		other.m_Capacity = 0;
		
		return *this;
	}

	void AddByCopy(const void* obj) {
		SX_CORE_ASSERT(m_MoveElseCopyConstructor, "Type does not support copy constructor");

		if (m_Size == m_Capacity)
			Reserve(m_Size * 2 + !m_Size);
		m_CopyConstructor(obj, At(m_Size++));
	}

	void AddByMove(void* obj) {
		if (m_Size == m_Capacity)
			Reserve(m_Size * 2 + !m_Size);
		m_MoveElseCopyConstructor(obj, At(m_Size++));
	}

	void UnorderedRemove(void* element) {
		SX_CORE_ASSERT(IsValidElement(element), "Wrong element address");

		m_MoveElseCopyConstructor(At(m_Size - 1), element);

		RemoveLast();
	}

	void RemoveLast() {
		SX_CORE_ASSERT(Size(), "UntypedList: Can't remove last element from empty list");
		m_Destructor(At(m_Size - 1));
		m_Size--;
	}

	void* At(size_t index) {
		SX_CORE_ASSERT(IsValidIndex(index), "UntypedList: Index is out of range");
		return (byte*)m_Memory + index * m_TypeInfo.Size;
	}

	void Reserve(size_t new_capacity) {
		if (new_capacity <= m_Capacity)
			return;

		byte* new_memory = (byte*)GeneralAllocatorType::Alloc(new_capacity * m_TypeInfo.Size);
		byte* old_memory = (byte*)m_Memory;

		for (int i = 0; i < m_Size; i++) {
			byte* new_element = new_memory + i * m_TypeInfo.Size;
			byte* old_element = old_memory + i * m_TypeInfo.Size;
			
			m_MoveElseCopyConstructor(old_element, new_element);
			m_Destructor(old_element);
		}

		GeneralAllocatorType::Free(m_Memory);
		m_Memory = new_memory;
		m_Capacity = new_capacity;
	}

	void Free() {
		Clear();
		GeneralAllocatorType::Free(m_Memory);
		m_Memory = nullptr;
		m_Capacity = 0;
	}

	void Clear() {
		while (Size())
			RemoveLast();
	}

	bool IsValidIndex(size_t index)const {
		return index < m_Size;
	}

	bool IsValidElement(void* element)const {
		return element >= m_Memory && element < (byte*)m_Memory + m_Size && ((byte*)element - (byte*)m_Memory) % m_TypeInfo.Size == 0;
	}
	
	size_t Size()const {
		return m_Size;
	}

	size_t Capacity()const {
		return m_Size;
	}

	void* Data() {
		return m_Memory;
	}

	const void* Data() const{
		return m_Memory;
	}

	std::type_index TypeIndex()const {
		return m_TypeInfo.TypeIndex;
	}

private:
	//XXX: for now assume that all types have desctructors
	template<typename ActualType>
	void AssignDtor(void *) {
		m_Destructor = [](void* obj) {
			((ActualType*)obj)->~ActualType();
		};
	}

	template<typename ActualType, typename = EnableIfType<IsCopyConstructible<ActualType>::Value, void>>
	void AssignCopyCtor(void *) {
		m_CopyConstructor = [](const void* src, void* dst) {
			new(dst) ActualType(*(const ActualType*)src);
		};
	}
	template<typename ActualType, typename = EnableIfType<!IsCopyConstructible<ActualType>::Value, void>>
	void AssignCopyCtor(...) { }

	template<typename ActualType, typename = EnableIfType<IsCopyConstructible<ActualType>::Value || IsMoveConstructible<ActualType>::Value, void>>
	void AssignMoveElseCopyCtor(void *){
		m_MoveElseCopyConstructor = [](void* src, void* dst) {
			new(dst) ActualType((ActualType&&)*(ActualType*)src);
		};
	}

	template<typename ActualType, typename = EnableIfType<!IsCopyConstructible<ActualType>::Value && !IsMoveConstructible<ActualType>::Value, void>>
	void AssignMoveElseCopyCtor(...) {}
};

using UntypedList = UntypedListBase<DefaultGeneralAllocator>;

#endif//STRAITX_UNTYPED_LIST_HPP