#ifndef STRAITX_INDEXED_RANGE_HPP
#define STRAITX_INDEXED_RANGE_HPP 

#include "core/type_traits.hpp"
#include "core/types.hpp"

template<typename BaseIt>
class IndexedRange{
private:
    struct IndexedIterator{
        const BaseIt It;
        const size_t Index;

        IndexedIterator(BaseIt it):
            It(it),
            Index(0)
        {}

        template<typename _BaseIt = BaseIt>
        IndexedIterator &operator++(){
            It++;
            const_cast<size_t&>(Index)++;   
            return *this;
        }

        template<typename _BaseIt = BaseIt>
        IndexedIterator &operator++(int){
            ++It;
            ++const_cast<size_t&>(Index);
            return *this;
        }

        template<typename _BaseIt = BaseIt>
        IndexedIterator &operator--(){
            It--;
            const_cast<size_t&>(Index)--;   
            return *this;
        }

        template<typename _BaseIt = BaseIt>
        IndexedIterator &operator--(int){
            --It;
            --const_cast<size_t&>(Index);
            return *this;
        }

        const IndexedIterator &operator*(){
            return *this;
        }

        template<typename _BaseIt = BaseIt>
        bool operator==(const IndexedIterator &other){
            return this->It == other.It;
        }

        template<typename _BaseIt = BaseIt>
        bool operator!=(const IndexedIterator &other){
            return this->It != other.It;
        }
    };
private:
    BaseIt m_Begin;
    BaseIt m_End;
public:
    IndexedRange(BaseIt begin, BaseIt end):
        m_Begin(begin),
        m_End(end)
    {}

    IndexedIterator begin()const{
        return {m_Begin};
    }

    IndexedIterator end()const{
        return {m_End};
    }

};

#endif//STRAITX_INDEXED_RANGE_HPP