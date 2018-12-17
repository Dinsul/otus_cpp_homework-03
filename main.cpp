#include "allocator.h"
#include "linked_list.h"

#include <list>
#include <map>

int main(int, char *[]) {

    {
        std::cout << "\n>>> std::map whith standart allocator" << std::endl;

        auto m = std::map<int, unsigned long long int,
                std::less<unsigned long long int>>{};

        unsigned long long int factorial = 1;

        for (int i = 0; i < 21; ++i) {
            m[i] = factorial;
            factorial *= (i + 1);
        }

        for (auto i: m) {
            std::cout << i.first << " " << i.second << std::endl;
        }

        std::cout << std::endl;
    }

    {
        std::cout << "\n>>> std::map whith custom allocator" << std::endl;

        auto m = std::map<int, unsigned long long int,
                std::less<unsigned long long int>,
                simple_allocator<std::pair<const int, unsigned long long int>>>{};

        unsigned long long int factorial = 1;

        for (int i = 0; i < 21; ++i) {
            m[i] = factorial;
            factorial *= (i + 1);
        }

        for (auto i: m) {
            std::cout << i.first << " " << i.second << std::endl;
        }

        std::cout << std::endl;
    }

    {
        std::cout << "\n>>> LinkedList whith standart allocator" << std::endl;
        LinkedList<int> list;

        for (int i = 0; i < 10; ++i)
        {
            list.push_front(i);
        }

        for (auto val : list)
        {
            std::cout << val << " ";
        }

        std::cout << std::endl;
    }

    {
        std::cout << "\n>>> LinkedList whith custom allocator" << std::endl;
        LinkedList<int, simple_allocator<int> > list_alloc;

        for (int i = 0; i < 10; ++i)
        {
            list_alloc.push_front(i);
        }

        for (auto val : list_alloc)
        {
            std::cout << val << " ";
        }

        std::cout << std::endl;
    }

    return 0;
}
