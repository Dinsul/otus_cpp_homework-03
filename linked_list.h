#pragma once
#include <cstddef>
#include <memory>

template <class T, class Al = std::allocator<T> >
class LinkedList
{
public:
    using value_type        = T;
    using pointer           = T*;
    using const_pointer     = const T*;
    using reference         = T&;
    using const_reference   = const T&;
    using size_type         = size_t;

protected:
    struct Node
    {
        Al     *_al_ptr;
        Node   *next;
        pointer value;

        Node(Al *al_ptr) : _al_ptr(al_ptr), next(nullptr), value(nullptr){}

        Node(Al *al_ptr, Node &&other) noexcept : _al_ptr(al_ptr)
        {
            std::swap(this->next,  other.next);
            std::swap(this->value, other.value);
        }

        Node(Al *al_ptr, const Node &other) : _al_ptr(al_ptr)
        {
            this->next  = other.next;
            this->value = other.value;
        }

        Node(Al *al_ptr, const_reference newVal) : _al_ptr(al_ptr), next(nullptr)
        {
            value = _al_ptr->allocate(1);
            _al_ptr->construct(value, newVal);
        }

        ~Node()
        {
            if (value)
            {
                _al_ptr->destroy(value);
                _al_ptr->deallocate(value, 1);
            }
        }
    };

    using NodeAl = typename Al::template rebind<Node>::other;

    template<class ...Args>
    Node* create_node(Args&& ... args)
    {
        Node *newNode = _node_al.allocate(1);
        _node_al.construct(newNode, &_allocator, std::forward<Args>(args)...);

        return newNode;
    }

    inline void deleteNode(Node *node)
    {
        _node_al.destroy(node);
        _node_al.deallocate(node, 1);
    }

    Al      _allocator;
    NodeAl  _node_al;

    Node  *_tail;
    Node  *_head;

    size_type _count;

public:
    struct iterator
    {
        iterator()
            : _node(nullptr) {}

        explicit iterator(Node* node)
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

        bool operator == (const Node* node) const {
            return _node == node;
        }

        bool operator != (const Node* node) const {
            return _node != node;
        }

    private:
        Node* _node;
    };

    explicit LinkedList() : _allocator(Al{}), _node_al(NodeAl{}), _tail(create_node()), _head(_tail), _count(0) {}


//    LinkedList(LinkedList &other)
//        : _allocator(other.allocator()), _node_al(other.node_al()),
//          _tail(create_node(*(other.end()))),
//          _head(create_node(*(other.end()))),
//          _count(other.size())
//    {}

//    LinkedList(LinkedList &&other)
//        : _allocator(other.allocator()), _node_al(other.node_al()),
//          _tail(create_node(*(other.end()))),
//          _head(create_node(*(other.begin()))),
//          _count(other.size())
//    {}

    LinkedList(const_reference value) : LinkedList()
    {
        push_front(value);
    }

    ~LinkedList()
    {
        while (end() != _head)
        {
            Node *toDelete = _head;

            _head = _head->next;

            deleteNode(toDelete);
        }

        deleteNode(_tail);

        std::cout << std::endl;
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

    template<class ...Args>
    void push_front(Args&& ... args)
    {
        auto newNode  = create_node(std::forward<Args>(args)...);

        newNode->next = _head;
        _head         = newNode;

        ++_count;
    }

    template<class ...Args>
    void push_back(Args&& ... args)
    {
        auto newNode  = create_node(std::forward<Args>(args)...);
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

    Al allocator() const
    {
        return _allocator;
    }

    NodeAl node_al() const
    {
        return _node_al;
    }
};






