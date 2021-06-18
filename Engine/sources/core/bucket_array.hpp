#ifndef STRAITX_BUCKET_ARRAY_HPP
#define STRAITX_BUCKET_ARRAY_HPP

#include <new>
#include "platform/types.hpp"
#include "core/assert.hpp"
#include "core/memory/allocator.hpp"
#include "core/template.hpp"
#include "core/move.hpp"
//for now
#include "core/noncopyable.hpp"

namespace StraitX{

template<typename Type, size_t BucketSize = 16, typename PoolAllocator = DefaultPoolAllocator>
class BucketArray: protected PoolAllocator, public NonCopyable{
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
			for(size_t i = 0; i<Size; i++)
				Array[i].~Type();
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
	typedef IteratorBase<Type> iterator;
	typedef IteratorBase<const Type> const_iterator;
private:
	Bucket *m_Begin;
	Bucket **m_End;
	size_t m_Size = 0;
public:
	BucketArray():
		PoolAllocator(sizeof(Bucket), alignof(Bucket)),
		m_Begin(nullptr),
		m_End(&m_Begin)
	{}

	~BucketArray(){
		Clear();
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
			Bucket *current = m_Begin;

			m_Begin = m_Begin->Next;

			FreeBucket(current);
		}
		m_End = &m_Begin;
		m_Size = 0;
	}

	size_t Size(){
		return m_Size;
	}

	iterator begin(){
		return iterator(m_Begin, 0);
	}

	iterator end(){
		return iterator(nullptr, 0);
	}

	const_iterator begin()const{
		return const_iterator(m_Begin, 0);
	}

	const_iterator end()const{
		return const_iterator(nullptr, 0);
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

}//namespace StraitX::

#endif//STRAITX_BUCKET_ARRAY_HPP