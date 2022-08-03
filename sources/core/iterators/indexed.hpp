#ifndef STRAITX_ITERATORS_INDEXED_HPP
#define STRAITX_ITERATORS_INDEXED_HPP

#include "core/types.hpp"
#include "core/type_traits.hpp"
#include "core/templates.hpp"
#include "core/printer.hpp"

template<typename BaseIt>
struct IndexedIterator{
    const BaseIt It;
    const size_t Index;

    IndexedIterator(BaseIt it):
        It(it),
        Index(0)
    {}

    template<typename _BaseIt = BaseIt>
    IndexedIterator &operator++(){
        const_cast<BaseIt&>(It)++;
        const_cast<size_t&>(Index)++;   
        return *this;
    }

    template<typename _BaseIt = BaseIt>
    IndexedIterator &operator++(int){
        ++const_cast<BaseIt&>(It);
        ++const_cast<size_t&>(Index);
        return *this;
    }

    template<typename _BaseIt = BaseIt>
    IndexedIterator &operator--(){
        const_cast<BaseIt&>(It)--;
        const_cast<size_t&>(Index)--;   
        return *this;
    }

    template<typename _BaseIt = BaseIt>
    IndexedIterator &operator--(int){
        --const_cast<BaseIt&>(It);
        --const_cast<size_t&>(Index);
        return *this;
    }

    const IndexedIterator &operator*(){
        return *this;
    }

    const IndexedIterator &operator*()const{
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


template<typename T>
struct Printer<IndexedIterator<T>>{
	static void Print(const IndexedIterator<T> &iterator, StringWriter &writer){
        using TypePrinter = Printer<typename RemoveConstVolatile<typename RemoveReference<decltype(*iterator.It)>::Type>::Type>;
        Printer<size_t>::Print(iterator.Index, writer);
        writer.Write(':');
        writer.Write(' ');
        TypePrinter::Print(*iterator.It, writer);
    }
};


#endif//STRAITX_ITERATORS_INDEXED_HPP