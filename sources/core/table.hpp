#ifndef STRAITX_TABLE_HPP
#define STRAITX_TABLE_HPP

#include "core/types.hpp"
#include "core/move.hpp"
#include "core/noncopyable.hpp"
#include "core/templates.hpp"
#include "core/allocators/allocator.hpp"
#include <functional>

template <typename KeyType, typename ValueType, typename HashType = std::hash<KeyType>, typename GeneralAllocatorType = DefaultGeneralAllocator>
class Table: private HashType, private GeneralAllocatorType, public NonCopyable{
    static_assert(!IsConst<KeyType>::Value && !IsConst<ValueType>::Value);
public:
    struct Entry {
        KeyType Key;
        ValueType Value;

        Entry(KeyType key, ValueType value):
            Key(Move(key)),
            Value(Move(value))
        {}
    };
private:
    static constexpr size_t InvalidHash = -1;
    static constexpr double ResizeLoadFactor = 0.75;

    struct HashedEntry: Entry{
        size_t Hash = InvalidHash;

        HashedEntry(KeyType key, ValueType value, size_t hash):
            Entry(Move(key), Move(value)),
            Hash(hash)
        {}

        void Construct(KeyType key, ValueType value, size_t hash) {
            new (&Key) KeyType(Move(key));
            new (&Value) ValueType(Move(value));
            Hash = hash;
        }

        void Construct(HashedEntry entry) {
            Construct(Move(entry.Key), Move(entry.Value), entry.Hash);
        }

        void Destruct() {
            Key.~KeyType();
            Value.~ValueType();
            Hash = InvalidHash;
        }

        bool IsConstructed()const{
            return Hash != InvalidHash;
        }
    };
    
	template<typename DereferenceType>
    class IteratorBase {
    private:
        HashedEntry* Current;
        size_t TillTheEnd;
    public:
        explicit IteratorBase(HashedEntry *entry, size_t till_the_end):
            Current(entry),
            TillTheEnd(till_the_end)
        {
            while (!Current->IsConstructed() && TillTheEnd) {
                Current++;
                TillTheEnd--;
            }
        }

        bool operator!=(const IteratorBase& other) {
            return Current != other.Current && TillTheEnd != other.TillTheEnd;
        }

        IteratorBase& operator++() {
            do {
                Current++;
                TillTheEnd--;
            } while (!Current->IsConstructed() && TillTheEnd);
            return *this;
        }

        DereferenceType* operator->() {
            return Current;
        }
        DereferenceType &operator*() {
            return *Current;
        }
    };
private:
    HashedEntry* m_Table = nullptr;
    size_t m_Size = 0;
    size_t m_Capacity = 0;
public:
    Table() = default;

    ~Table() {
        Free();
    }

    void Add(KeyType key, ValueType value) {
        size_t hash = DoHash(key);
        Add(HashedEntry{ Move(key), Move(value),  hash});
    }

    IteratorBase<const Entry> Find(const KeyType &key)const{
        size_t key_hash = DoHash(key);
        size_t index = IndexOf(key, key_hash);
        
        if (!m_Table[index].IsConstructed())
            return end();

        return IteratorBase<const Entry>( &m_Table[index], m_Capacity - index );
    }

    void Remove(KeyType key) {
        size_t key_hash = DoHash(key);
        size_t index = IndexOf(key, key_hash);
        
        if (!m_Table[index].IsConstructed())
            return;
        
        m_Table[index].Destruct();

        m_Size -= 1;
    }

    bool Contains(const KeyType& key)const {
        size_t key_hash = DoHash(key);
        size_t index = IndexOf(key, key_hash);
        
        return m_Table[index].IsConstructed();
    }

    auto begin()const{
        return IteratorBase<const Entry>(m_Table, m_Capacity);
    }

    auto end()const{
        return IteratorBase<const Entry>(m_Table + m_Capacity, m_Capacity - m_Capacity);
    }

    size_t Size()const {
        return m_Size;
    }

    size_t Capacity()const {
        return m_Capacity;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity <= m_Capacity)
            return;

        HashedEntry* old_table = m_Table;
        size_t old_capacity = m_Capacity;

        m_Capacity = new_capacity;
        m_Table = NewTable(m_Capacity);
        m_Size = 0;

        for (size_t i = 0; i < old_capacity; i++) {
            if (old_table[i].IsConstructed()) {
                Add(Move(old_table[i]));
                old_table[i].Destruct();
            }
        }

        FreeTable(old_table);
    }

    void Clear() {
        for (size_t i = 0; i < m_Capacity; i++)
            if (m_Table[i].IsConstructed())
                m_Table[i].Destruct();
        m_Size = 0;
    }

    void Free() {
        Clear();
        FreeTable(m_Table);
        m_Capacity = 0;
    }

private:
    void EnsureCapacity() {
        if (m_Capacity * ResizeLoadFactor > m_Size) return;

        Reserve(GetNextCapacity(m_Capacity));
    }

    void Add(HashedEntry entry) {
        EnsureCapacity();

        size_t index = IndexOf(entry.Key, entry.Hash);

        if (!m_Table[index].IsConstructed()) {
            m_Table[index].Construct(Move(entry));
            m_Size += 1;
        } else {
            m_Table[index].Value = Move(entry.Value);
        }
    }

    size_t IndexOf(const KeyType &key, size_t key_hash)const{
        size_t index = key_hash % m_Capacity;
        size_t index_increment = 1;

        while (m_Table[index].IsConstructed() && !(m_Table[index].Key == key)) {
            index = (index + index_increment) % m_Capacity;
            index_increment += 1;
        }
        return index;
    }


    HashedEntry* NewTable(size_t capacity) {
        auto table = (HashedEntry*)GeneralAllocatorType::Alloc(capacity * sizeof(HashedEntry));
        for (size_t i = 0; i < capacity; i++)
            table[i].Hash = InvalidHash;
        return table;
    }

    void FreeTable(Entry* table) {
        GeneralAllocatorType::Free(table);
    }

    size_t DoHash(const KeyType &key)const{
        size_t hash = HashType::operator()(key);
        hash += hash == InvalidHash;
        return hash;
    }
    
    static size_t GetNextCapacity(size_t current) {
        return 1 + current * 3;
    }
};

#endif//STRAITX_TABLE_HPP