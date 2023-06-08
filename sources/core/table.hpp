#ifndef STRAITX_TABLE_HPP
#define STRAITX_TABLE_HPP

#include "core/types.hpp"
#include "core/move.hpp"
#include "core/noncopyable.hpp"
#include "core/templates.hpp"
#include "core/algorithm.hpp"
#include "core/allocators/allocator.hpp"
#include <functional>

template <typename KeyType, typename ValueType, typename HashType = std::hash<KeyType>, typename GeneralAllocatorType = DefaultGeneralAllocator>
class Table: private HashType, private GeneralAllocatorType, public NonCopyable{
    static_assert(!IsConst<KeyType>::Value && !IsConst<ValueType>::Value, "Key and Value types should not be const");
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
        template <typename Other>
        friend class IteratorBase;

        HashedEntry* Current;
        size_t TillTheEnd;
    public:
        template<typename OtherDereferencedType, typename _ = EnableIfType<IsCastable<OtherDereferencedType, DereferenceType>::Value,void>>
        explicit IteratorBase(const IteratorBase<OtherDereferencedType> &it):
            IteratorBase(it.Current, it.TillTheEnd)
        {}

        explicit IteratorBase(HashedEntry *entry, size_t till_the_end):
            Current(entry),
            TillTheEnd(till_the_end)
        {
            while (TillTheEnd && !Current->IsConstructed()) {
                Current++;
                TillTheEnd--;
            }
        }

        bool operator!=(const IteratorBase& other) const{
            return !(*this == other);
        }

        bool operator==(const IteratorBase& other) const{
            return Current == other.Current && TillTheEnd == other.TillTheEnd;
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

    Table(std::initializer_list<Entry> entries) {
        for(const Entry &entry: entries)
            Add(entry.Key, entry.Value);
    }

    Table(Table&& other) {
        *this = Move(other);
    }

    ~Table() {
        Free();
    }

    Table &operator=(Table&& other) {
        Free();
        Swap(m_Table, other.m_Table);
        Swap(m_Size, other.m_Size);
        Swap(m_Capacity, other.m_Capacity);
        return *this;
    }

    bool Add(KeyType key, ValueType value) {
        auto it = Find(key);

        if (it != end()){
            SX_ASSERT(it->Key == key);
            it->Value = value;
            return false;
        } else {
            EnsureCapacity();

            HashedEntry& entry = m_Table[m_Size++];
            entry.Construct(key, value, 0);
            return true;
        }
    }

    ValueType& operator[](const KeyType& key) {
        auto it = Find(key);

        if (it == end()) {
            Add(key, ValueType{});
            it = Find(key);
        } 

        return it->Value;
    }

    const ValueType& operator[](const KeyType& key)const{
        auto it = Find(key);
        SX_CORE_ASSERT(it != end(), "Key was not created");
        return it->Value;
    }

    IteratorBase<const Entry> Find(const KeyType &key)const{
        for (auto it = begin(); it != end(); ++it)
            if (it->Key == key)
                return it;

        return end();
    }

    IteratorBase<Entry> Find(const KeyType& key) { 
        return IteratorBase<Entry>(const_cast<const Table*>(this)->Find(key));
    }

    void Remove(KeyType key) {
        
        IteratorBase<Entry> it = Find(key);

        if (it == end())
            return;

        *((HashedEntry*)&*it) = Move(m_Table[m_Size - 1]);
        m_Table[m_Size - 1].Destruct();

        m_Size -= 1;
    }

    bool Contains(const KeyType& key)const {
        return Find(key) != end();
    }

    auto begin()const{
        return IteratorBase<const Entry>(m_Table, m_Capacity);
    }

    auto end()const{
        return IteratorBase<const Entry>(m_Table + m_Capacity, m_Capacity - m_Capacity);
    }

    auto begin(){
        return IteratorBase<Entry>(m_Table, m_Capacity);
    }

    auto end(){
        return IteratorBase<Entry>(m_Table + m_Capacity, m_Capacity - m_Capacity);
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
	    m_Table = NewTable(new_capacity);
        m_Size = 0;

        for (size_t i = 0; i < old_capacity; i++) {
            if (old_table[i].IsConstructed()) {
                Add(Move(old_table[i].Key), Move(old_table[i].Value));
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

    HashedEntry* NewTable(size_t capacity) {
        HashedEntry *table = (HashedEntry*)GeneralAllocatorType::Alloc(capacity * sizeof(HashedEntry));
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