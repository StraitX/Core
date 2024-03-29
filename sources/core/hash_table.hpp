#ifndef STRAITX_TABLE_HPP
#define STRAITX_TABLE_HPP

#include "core/types.hpp"
#include "core/move.hpp"
#include "core/templates.hpp"
#include "core/type_traits.hpp"
#include "core/pair.hpp"
#include "core/list.hpp"
#include <functional>
#include <unordered_map>

template <typename KeyType, typename ValueType, typename HashType = std::hash<KeyType>>
class HashTable: public std::unordered_map<KeyType, ValueType, HashType>{
private:
    using ImplHashTableType = std::unordered_map<KeyType, ValueType, HashType>;
public:
    using Iterator = typename ImplHashTableType::iterator;
    using ConstIterator = typename ImplHashTableType::const_iterator;
public:
    HashTable() = default;

    HashTable(std::initializer_list<Pair<KeyType, ValueType>> entries) {
        for(const auto &entry: entries)
            Add(entry.First, entry.Second);
    }
    
    HashTable(const HashTable& other) {
        *this = other;
    }

    HashTable(HashTable&& other) {
        *this = Move(other);
    }

    HashTable &operator=(HashTable&& other) {
        return Assign(Move(other));
    }

    HashTable &operator=(const HashTable &other) {
        return Assign(other);
    }

    HashTable& Assign(HashTable&& other) {
        ImplHashTableType::operator=(Move(other));
        return *this;
    }

    HashTable& Assign(const HashTable& other) {
        ImplHashTableType::operator=(other);
        return *this;
    }

    void Add(KeyType &&key, ValueType &&value) {
        ImplHashTableType::emplace(Move(key), Move(value));
    }

    void Add(const KeyType &key, const ValueType &value) {
        ImplHashTableType::emplace(key, value);
    }

    using ImplHashTableType::operator[];

    Iterator Find(const KeyType& key) {
        return ImplHashTableType::find(key);
    }

    ConstIterator Find(const KeyType& key)const{
        return ImplHashTableType::find(key);
    }

    void Remove(const KeyType &key) {
        ImplHashTableType::erase(key);
    }

    bool Has(const KeyType& key)const {
        return Find(key) != ImplHashTableType::end();
    }

    size_t Size()const {
        return ImplHashTableType::size();
    }

    size_t Capacity()const {
        return ImplHashTableType::capacity();
    }

    void Reserve(size_t new_capacity) {
        ImplHashTableType::reserve(new_capacity);
    }

    void Clear() {
        ImplHashTableType::clear();
    }

    //TODO: rewrite using ranges
    template<
        typename _KeyType = KeyType,
        typename = EnableIfType<IsCopyConstructible<_KeyType>::Value>>
    List<KeyType> Keys()const {
        List<KeyType> keys;
        for (const auto &[key, _] : *this) {
            keys.Add(key);
        }
        return keys;
    }

    template<
        typename _ValueType = ValueType,
        typename = EnableIfType<IsCopyConstructible<_ValueType>::Value>>
    List<ValueType> Values()const {
        List<KeyType> values;
        for (const auto &[_, value] : *this) {
            values.Add(value);
        }
        return values;
    }

    template<
        typename _KeyType = KeyType,
        typename = EnableIfType<IsCopyConstructible<_KeyType>::Value>,
        typename _ValueType = ValueType,
        typename = EnableIfType<IsCopyConstructible<_ValueType>::Value>>
    List<Pair<KeyType, ValueType>> ToList()const {
        List<Pair<KeyType, ValueType>> list;
        for (const auto &[key, value]: *this) {
            list.Emplace(key, value);
        }
        return list;
    }
};

#endif//STRAITX_TABLE_HPP