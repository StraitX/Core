#ifndef STRAITX_POLYMORPH_LIST
#define STRAITX_POLYMORPH_LIST

#include "core/untyped_list.hpp"
#include "core/span.hpp"

//XXX: Allocator usage is not optimal for custom ones

template<typename BaseType, typename GeneralAllocatorType = DefaultGeneralAllocator>
class PolymorphList: private GeneralAllocatorType, public NonCopyable{
	static_assert(IsPolymorhpic<BaseType>::Value && !IsConst<BaseType>::Value && !IsVolatile<BaseType>::Value && !IsReference<BaseType>::Value, "PolymorphList requires Type to be polymorhpic, non cv-qualified and non reference");
private:
	using UntypedList = UntypedListBase<GeneralAllocatorType>;
	
	template<typename CastType>
	class IteratorBase {
	private:
		UntypedList* m_TypeList = nullptr;
		size_t m_TypeListIndex;
	public:
		IteratorBase(UntypedList *type_list, size_t type_list_index):
			m_TypeList(type_list),
			m_TypeListIndex(type_list_index)
		{}

		bool operator!=(const IteratorBase& other)const{
			return m_TypeList != other.m_TypeList || m_TypeListIndex != other.m_TypeListIndex;
		}

		IteratorBase& operator++() {
			if (m_TypeListIndex == m_TypeList->Size() - 1) {
				m_TypeList++;
				m_TypeListIndex = 0;
			} else {
				m_TypeListIndex++;
			}
			return *this;
		}

		CastType* operator->() {
			return (CastType*)m_TypeList->At(m_TypeListIndex);
		}

		CastType& operator*() {
			return *(CastType*)m_TypeList->At(m_TypeListIndex);
		}
	};
private:
	UntypedList *m_TypeLists = nullptr;
	size_t m_Size = 0;
	size_t m_Capacity = 0;
public:
	PolymorphList() = default;

	PolymorphList(PolymorphList&& other) {
		*this = Move(other);
	}

	~PolymorphList() {
		Free();
	}

	PolymorphList& operator=(PolymorphList&& other) {
		Free();
		m_TypeLists = other.m_TypeLists;
		m_Size = other.m_Size;
		m_Capacity = other.m_Capacity;
		other.m_TypeLists = nullptr;
		other.m_Size = 0;
		other.m_Capacity = 0;
		return *this;
	}

	template<typename SubType, typename = EnableIfType<IsBaseOf<BaseType, SubType>::Value, void>>
	void Add(const SubType &element) {
		FindOrCreate<SubType>().AddByCopy(&element);
	}

	template<
		typename SubType, 
		typename = EnableIfType<!IsConst<SubType>::Value, void>, 
		typename = EnableIfType<!IsVolatile<SubType>::Value, void>, 
		typename = EnableIfType<!IsLValueReference<SubType>::Value, void>, 
		typename = EnableIfType<IsBaseOf<BaseType, SubType>::Value, void>
	>
	void Add(SubType &&element) {
		FindOrCreate<SubType>().AddByMove(&element);
	}
	
	void Add(PolymorphList&& other) {
		for (size_t i = 0; i < other.m_Size; i++)
			Add(Move(other.m_TypeLists[i]));
	}

	void Add(UntypedList&& other) {
		UntypedList* list = Find(other.TypeIndex());
		if (list) {
			list->AddByMove(Move(other));
		} else {
			Reserve(m_Size + 1);
			new (&m_TypeLists[m_Size++]) UntypedList(Move(other));
		}
	}

	template<typename SubType, typename = EnableIfType<IsBaseOf<BaseType, SubType>::Value, void>>
	void UnorderedRemove(SubType *element) {
		UntypedList* list = Find(std::type_index(typeid(*element)));
		SX_CORE_ASSERT(list, "Can't remove object which is not in PolymorphList");
		
		list->UnorderedRemove(element);
	}

	void Free() {
		Clear();

		GeneralAllocatorType::Free(m_TypeLists);
		m_TypeLists = nullptr;
		m_Capacity = 0;
	}
	
	template<typename FunctorType>
	void ForEach(FunctorType functor) {
		for (size_t i = 0; i < m_Size; i++) 
			for (size_t j = 0; j < m_TypeLists[i].Size(); j++)
				functor((BaseType*)m_TypeLists[i].At(j));
	}

	void Reserve(size_t new_capacity) {
		if(new_capacity <= m_Capacity)
			return;

        UntypedList *new_types_list = (UntypedList*)GeneralAllocatorType::Alloc(new_capacity * sizeof(UntypedList));
		UntypedList *old_types_list = m_TypeLists;

        for(size_t i = 0; i< m_Size; i++){
			new(&new_types_list[i]) UntypedList(Move(old_types_list[i]));
            old_types_list[i].~UntypedList();
        }

        GeneralAllocatorType::Free(old_types_list);
        m_TypeLists = new_types_list;
        m_Capacity = new_capacity;
	}

	void Clear() {
		for (size_t i = 0; i < m_Size; i++)
			m_TypeLists[i].~UntypedList();
		m_Size = 0;
	}

	IteratorBase<BaseType> begin() {
		return { m_TypeLists, 0 };
	}

	IteratorBase<BaseType> end() {
		return { m_TypeLists + m_Size, 0 };
	}

	IteratorBase<const BaseType> begin() const{
		return { m_TypeLists, 0 };
	}

	IteratorBase<const BaseType> end() const{
		return { m_TypeLists + m_Size, 0 };
	}

	template<
		typename SubType, 
		typename = EnableIfType<!IsConst<SubType>::Value, void>, 
		typename = EnableIfType<!IsVolatile<SubType>::Value, void>, 
		typename = EnableIfType<!IsLValueReference<SubType>::Value, void>, 
		typename = EnableIfType<IsBaseOf<BaseType, SubType>::Value, void>
	>
	Span<SubType> TypeRange() {
		UntypedList* list = Find(std::type_index(typeid(SubType)));

		SX_CORE_ASSERT(list, "SubType is not present in Polymorph list");
		
		return { (SubType*)list->Data(), list->Size() };
	}

	template<
		typename SubType, 
		typename = EnableIfType<!IsConst<SubType>::Value, void>, 
		typename = EnableIfType<!IsVolatile<SubType>::Value, void>, 
		typename = EnableIfType<!IsLValueReference<SubType>::Value, void>, 
		typename = EnableIfType<IsBaseOf<BaseType, SubType>::Value, void>
	>
	ConstSpan<SubType>TypeRange()const{
		UntypedList* list = Find(std::type_index(typeid(SubType)));
		
		if(list)
			return {(const SubType*)list->Data(), list->Size()};
		return { nullptr, 0 };
	}
private:
	template<typename SubType>
	UntypedList& FindOrCreate() {
		UntypedList* list = Find(std::type_index(typeid(SubType)));

		if (list)
			return *list;

		if (m_Size == m_Capacity)
			Reserve(m_Size * 2 + !m_Size);
		
		return * new (m_TypeLists + m_Size++) UntypedList(UntypedList::Type<SubType>());
	}

	UntypedList* Find(std::type_index index)const{
		for (size_t i = 0; i < m_Size; i++) {
			if (m_TypeLists[i].TypeIndex() == index)
				return &m_TypeLists[i];
		}
		return nullptr;
	}


};

#endif//STRAITX_POLYMORPH_LIST