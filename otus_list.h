#pragma once
#include <cstddef>
#include <memory>

template <class T, class Al = std::allocator<T> >
class OtusList
{
public:
    using value_type        = T;
    using pointer           = T*;
    using const_pointer     = const T*;
    using reference         = T&;
    using const_reference   = const T&;
    using size_type         = size_t;

private:
    struct OL_Node
    {
        OL_Node() : next(nullptr), value(nullptr) {}

        OL_Node* next;
        pointer  value;
    };

    inline OL_Node* create_node(const_reference value)
    {
        OL_Node *newNode = new OL_Node();
        newNode->value   = _allocator.allocate(1);

        _allocator.construct(newNode->value, value);

        return newNode;
    }

    inline OL_Node* create_node()
    {
        return new OL_Node();
    }

    inline void deleteNode(OL_Node *node)
    {
        _allocator.destroy(node->value);
        _allocator.deallocate(node->value, 1);

        delete node;
    }

    OL_Node  *_tail;
    OL_Node  *_head;

    Al        _allocator;
    size_type _count;

public:
    struct iterator
    {
        iterator()
            : _node(nullptr) {}

        explicit iterator(OL_Node* node)
            : _node(node) {}

        reference operator* () const {
            return *(_node->value);
        }

        pointer operator-> () const {
            return _node->value;
        }

        iterator& operator++ () {
            _node = _node->next;
            return *this;
        }

        iterator operator++ (int) {
            iterator old = *this;
            _node = _node->next;
            return old;
        }

        iterator& operator-- ()    = delete;
        iterator  operator-- (int) = delete;

        bool operator == (const iterator& other) const {
            return _node == other._node;
        }

        bool operator != (const iterator& other) const {
            return _node != other._node;
        }

        bool operator == (const OL_Node* node) const {
            return _node == node;
        }

        bool operator != (const OL_Node* node) const {
            return _node != node;
        }

    private:
        OL_Node* _node;
    };

    explicit OtusList(): _tail(create_node()), _head(_tail), _allocator(Al{}), _count(0) {}

    OtusList(const_reference value) : OtusList()
    {
        push_front(value);
    }

    ~OtusList()
    {
        while (end() != _head->next)
        {
            OL_Node *toDelete = _head;
            _head = _head->next;

            deleteNode(toDelete);
        }
    }

    //    OtusList(T &&value);

    //    OtusList(OtusList &other);

    //    OtusList(OtusList &&other);

    iterator begin() const
    {
        return iterator(_head);
    }

    iterator end() const
    {
        return iterator(_tail);
    }

    iterator at(size_type pos)
    {
        iterator retVal(_head);

        while (pos && retVal != end()) {
            ++pos;
            ++retVal;
        }

        return retVal;
    }

    void push_front(const_reference value)
    {
        auto newNode  = create_node(value);

        newNode->next = _head;
        _head         = newNode;

        ++_count;
    }

    void push_back(const_reference value)
    {
        auto newNode  = create_node(value);
        auto back     = _head;

        while (end() != back->next) {
            back = back->next;
        }

        newNode->next = back->next;
        back->next    = newNode;

        ++_count;
    }

    value_type pop_front(void)
    {
        if (_head->next != end() && _head->value){
            auto nodeToDel = _head;
            auto retValue  = *(nodeToDel->value);

            _head          = nodeToDel->next;

            deleteNode(nodeToDel);

            --_count;

            return retValue;
        }

        throw std::invalid_argument("Call pop for empty list");
        return value_type();
    }

    value_type pop_back(void)
    {
        if (_head->next != end()) {
            auto back = _head;

            while (back->next->next != end()) {
                back  = back->next;
            }

            auto nodeToDel = back->next->next;
            auto retValue  = *(nodeToDel->value);

            back->next     = nodeToDel->next;

            deleteNode(nodeToDel);

            --_count;

            return retValue;
        }

        throw std::invalid_argument("Call pop for empty list");
        return value_type();
    }

    //    void insert(iterator pos, const_reference value);

    size_type size()
    {
        return _count;
    }
};
