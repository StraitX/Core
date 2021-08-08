#ifndef STRAITX_BUCKET_LIST_HPP
#define STRAITX_BUCKET_LIST_HPP

#include <new>
#include "core/types.hpp"
#include "core/assert.hpp"
#include "core/allocators/allocator.hpp"
#include "core/type_traits.hpp"
#include "core/move.hpp"
//for now
#include "core/noncopyable.hpp"

template<typename Type, size_t BucketSize = 16, typename PoolAllocator = DefaultPoolAllocator>
class BucketList: protected PoolAllocator, public NonCopyable{
	static_assert(!IsConst<Type>() && !IsVolatile<Type>(), "Type can't be cv-qualified");
private:
	struct Bucket{
		Bucket *Next = nullptr;
		size_t Size = 0;
		struct alignas(Type) {
			u8 Memory[sizeof(Type) * BucketSize];

			Type &operator[](size_t index){
				SX_CORE_ASSERT(index < BucketSize, "BucketArray: Bucket::operator[]: index is out of range");
				return reinterpret_cast<Type*>(Memory)[index];
			}
		}Array;

		Bucket() = default;

		~Bucket(){
			Clear();
		}

		void Clear(){
			for(size_t i = 0; i<Size; i++)
				Array[i].~Type();
			Size = 0;
		}
	};
private:
	template<typename DereferenceType>
	class IteratorBase{
	private:
		Bucket *m_CurrentBucket;
		size_t m_CurrentIndex;
	public:
		explicit IteratorBase(Bucket *current_bucket, size_t current_index):
			m_CurrentBucket(current_bucket),
			m_CurrentIndex(current_index)
		{}

		IteratorBase &operator++(){
			++m_CurrentIndex;
			if(m_CurrentIndex == m_CurrentBucket->Size){
				m_CurrentBucket = m_CurrentBucket->Next;
				m_CurrentIndex = 0;
			}
			return *this;
		}

		bool operator!=(const IteratorBase &other){
			return m_CurrentBucket != other.m_CurrentBucket || m_CurrentIndex != other.m_CurrentIndex;
		}

		bool operator==(const IteratorBase &other){
			return m_CurrentBucket == other.m_CurrentBucket && m_CurrentIndex == other.m_CurrentIndex;
		}

		DereferenceType *operator->(){
			return &m_CurrentBucket->Array[m_CurrentIndex];
		}

		DereferenceType &operator*(){
			return m_CurrentBucket->Array[m_CurrentIndex];
		}
	};
public:
	typedef IteratorBase<Type> Iterator;
	typedef IteratorBase<const Type> ConstIterator;
private:
	Bucket *m_Begin = nullptr;
	Bucket **m_End = &m_Begin;
	size_t m_Size = 0;
public:
	BucketList():
		PoolAllocator(sizeof(Bucket), alignof(Bucket))
	{}

	~BucketList(){
		while(m_Begin != nullptr){
			Bucket *current = m_Begin;

			m_Begin = m_Begin->Next;

			FreeBucket(current);
		}
		m_End = &m_Begin;
		m_Size = 0;
	}

	template<typename...Args>
	void EmplaceBack(Args&&...args){
		if(*m_End == nullptr)
			*m_End = AllocBucket();

		auto &bucket = **m_End;

		new(&bucket.Array[m_Size++ % BucketSize]) Type(Forward<Args>(args)...);

		bucket.Size += 1;

		if(bucket.Size == BucketSize)
			m_End = &bucket.Next;
	}

	void PushBack(const Type &type){
		EmplaceBack(type);
	}

	void PushBack(Type &&type){
		EmplaceBack(Move(type));
	}

	void Clear(){
		while(m_Begin != nullptr){
			m_Begin->Clear();

			m_Begin = m_Begin->Next;
		}
		m_Size = 0;
	}

	size_t Size()const{
		return m_Size;
	}

	Iterator begin(){
		return Iterator(m_Begin, 0);
	}

	Iterator end(){
		return Iterator(nullptr, 0);
	}

	ConstIterator begin()const{
		return ConstIterator(m_Begin, 0);
	}

	ConstIterator end()const{
		return ConstIterator(nullptr, 0);
	}
private:
	Bucket *AllocBucket(){
		return new(PoolAllocator::Alloc())Bucket();
	}

	void FreeBucket(Bucket *bucket){
		bucket->~Bucket();
		PoolAllocator::Free(bucket);
	}
};

#endif//STRAITX_BUCKET_LIST_HPP