#pragma once

#include <stdexcept>
#include <iostream>
#include <vector>

#define MEM_CHUNK 10 // Количество выделяемой памяти за раз в штуках

template<typename T>
struct otus_allocator {
    using value_type        = T;
    using pointer           = T*;
    using const_pointer     = const T*;
    using reference         = T&;
    using const_reference   = const T&;

private:
    size_t  _counter;

    std::vector<pointer> _memPool;

public:
    otus_allocator() : _counter(0){}

    template<typename U>
    struct rebind {
        using other = otus_allocator<U>;
    };

    pointer allocate(std::size_t n)
    {
        if (n != 1)
            throw std::invalid_argument("allocate(), bad 'n'");

        if (_counter % MEM_CHUNK == 0)
        {
            auto memPtr = static_cast<pointer>(malloc(MEM_CHUNK * sizeof(value_type)));

            if (!memPtr)
            {
                throw std::bad_alloc();
            }

            _memPool.emplace_back(memPtr);
        }

        auto retValue = _memPool[_counter / MEM_CHUNK] + _counter % MEM_CHUNK;
        _counter += n;

        return retValue;
    }

    void deallocate(pointer, std::size_t n)
    {
//        std::cout << __PRETTY_FUNCTION__ << "[n=" << n << " count=" << _counter << "]" << std::endl;

        if (n > _counter)
            throw std::invalid_argument("deallocate(), bad 'n'");

        _counter -= n;

        if (_counter == 0)
        {
            for(auto i : _memPool)
            {
                std::free(i);
            }
        }
    }

    template<typename U, class ...Args>
    void construct(U *p, Args&& ... args) {
//        std::cout << __PRETTY_FUNCTION__ << std::endl;
        new(p) U(std::forward<Args>(args)...);
    }

    void destroy(pointer p) {
//        std::cout << __PRETTY_FUNCTION__ << std::endl;
        p->~T();
    }
};
