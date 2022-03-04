#ifndef LIST_H_
#define LIST_H_

#include <iostream>

namespace DSlist {

template <typename T>
struct node {
    T data{};
    node<T>* next = nullptr;
    node<T>* prev = nullptr;
    node<T>(const T& elem, node<T>* n_ptr = nullptr, node<T>* p_ptr = nullptr) :
        data(elem), next(n_ptr), prev(p_ptr) { }
    node<T>() = default;
};

template <typename T>
class List {
public:
    // Member functions
    List() : m_size(0), head(nullptr), tail(nullptr) { }

    List(const T& value) : head(new node<T>(value)) {
        tail = head;
        ++m_size;
    }

    List(unsigned int count, const T& value) {
        head = new node<T>(value);
        tail = head;
        node<T>* curr_head = head;
        for (unsigned int i = 0; i < count - 1; ++i)
        {
            curr_head->next = new node<T>(value, nullptr, curr_head);
            curr_head = curr_head->next;
            tail = curr_head;
        }
        m_size = count;    
    }

    List(const List& other) {
        head = new node<T>(other.head->data);
        node<T>* curr_head = head;
        tail = head;
        node<T>* curr_other_head = other.head;
        while (curr_other_head->next != nullptr) {
            curr_head->next = new node<T>(curr_other_head->next->data, nullptr, curr_head);
            curr_other_head = curr_other_head->next;
            curr_head = curr_head->next;
            tail = curr_head;
        }
        m_size = other.m_size;
    }

    List(List&& other) {
        head = new node<T>(other.head->data);
        node<T>* curr_head = head;
        tail = head;
        node<T>* curr_other_head = other.head;
        while (curr_other_head->next != nullptr) {
            curr_head->next = new node<T>(curr_other_head->next->data, nullptr, curr_head);
            curr_other_head = curr_other_head->next;
            curr_head = curr_head->next;
            tail = curr_head;
        }
        m_size = other.m_size;
        other.clear();
        other.head = nullptr;
        other.tail = nullptr;
    }

    List& operator=(const List& other) {
        if (this == &other) { 
            return *this; 
        }
        head = new node<T>(other.head->data);
        node<T>* curr_head = head;
        tail = head;
        node<T>* curr_other_head = other.head;
        while (curr_other_head->next != nullptr) {
            curr_head->next = new node<T>(curr_other_head->next->data, nullptr, curr_head);
            curr_other_head = curr_other_head->next;
            curr_head = curr_head->next;
            tail = curr_head;
        }
        m_size = other.m_size;
        return *this;
    }

    List& operator=(List&& other) noexcept {
        if (this == &other) { 
            return *this; 
        }
        head = new node<T>(other.head->data);
        node<T>* curr_head = head;
        tail = head;
        node<T>* curr_other_head = other.head;
        while (curr_other_head->next != nullptr) {
            curr_head->next = new node<T>(curr_other_head->next->data, nullptr, curr_head);
            curr_other_head = curr_other_head->next;
            curr_head = curr_head->next;
            tail = curr_head;
        }
        other.clear();
        other.head = nullptr;
        other.tail = nullptr;
        m_size = other.m_size;
        return *this;
    }

    ~List() {
        node<T>* curr_head = head;
        while (curr_head != nullptr) {
            curr_head = head->next;
            delete head;
            head = curr_head;
        }
        head = nullptr;
        tail = nullptr;
        m_size = 0;
    }

    void show() {
        node<T>* curr_head = head;
        while (curr_head != nullptr) {
            std::cout << curr_head->data << " ";
            curr_head = curr_head->next;
        }
        std::cout << std::endl;
    }

    void show_rev() {
        node<T>* curr_tail = tail;
        while (curr_tail != nullptr) {
            std::cout << curr_tail->data << " ";
            curr_tail = curr_tail->prev;
        }
        std::cout << std::endl;
    }

    // Element access
    T& front() const {
        if (this->empty()) { 
            throw std::out_of_range("Out of range\n");
        } 
        return head->data;
    }

    // Capacity
    bool empty() const noexcept {
        return m_size == 0;
    }

    unsigned int size() const noexcept {
        return m_size;
    }

    // Modifiers
    void clear() noexcept {
        node<T>* curr_head = head;
        while (curr_head != nullptr) {
            curr_head = head->next;
            delete head;
            head = curr_head;
        }
        head = nullptr;
        tail = nullptr;
        m_size = 0;
    }

    void push_front(const T& value) { 
        if (this->empty()) {
            head = new node<T>(value);
            tail = head; ++m_size;
            return; 
        }
        node<T>* coming_node = new node<T>(value, head, nullptr);
        head->prev = coming_node;
        head = coming_node;
        ++m_size;
    }

    void push_front(T&& value) {
        if (this->empty()) { 
            head = new node<T>(value); 
            tail = head; ++m_size; 
            return; 
        }
        node<T>* coming_node = new node<T>(std::move(value), head, nullptr);
        head->prev = coming_node;
        head = coming_node;
        ++m_size;
    }

    void push_back(const T& value) {
        if (this->empty()) { 
            head = new node<T>(value); 
            tail = head; ++m_size; 
            return; 
        }
        node<T>* coming_node = new node<T>(value, nullptr, tail);
        tail->next = coming_node;
        tail = coming_node;
        ++m_size;
    }

    void push_back(T&& value) {
        if (this->empty()) { 
            head = new node<T>(value); 
            tail = head; ++m_size; 
            return; 
        }
        node<T>* coming_node = new node<T>(std::move(value), nullptr, tail);
        tail->next = coming_node;
        tail = coming_node;
        ++m_size;
    }

    T pop_back() {
        if (this->empty()) { 
            return T(); 
        } else if (m_size == 1) { 
            T res = head->data;
            this->clear();
            return res;
        }
        node<T>* curr_tail = tail;
        T res = curr_tail->data;
        tail->prev->next = nullptr;
        tail = tail->prev;
        delete curr_tail;
        --m_size;
        return res;
    }

    T pop_front() {
        if (this->empty()) { 
            return T(); 
        } else if (m_size == 1) { 
            T res = head->data;
            this->clear();
            return res;
        }
        node<T>* curr_head = head;
        T res = curr_head->data;
        head->next->prev = nullptr;
        head = head->next;
        delete curr_head;
        --m_size;
        return res;
    }

    bool insert(unsigned int pos, const T& value) {
        if (pos > m_size || pos < 0) { 
            return false; 
        } else if (pos == 0) { 
            this->push_front(value); 
            return true; 
        } else if (pos == m_size) { 
            this->push_back(value); 
            return true; 
        }
        node<T>* curr_head = head;
        while (pos != 1) {
            curr_head = curr_head->next;
            --pos;
        }
        node<T>* coming_node = new node<T>(value);
        coming_node->next = curr_head->next;
        coming_node->prev = curr_head;
        curr_head->next->prev = coming_node;
        curr_head->next = coming_node;
        ++m_size;
        return true;
    }

    bool insert(unsigned int pos, T&& value) {
        if (pos > m_size || pos < 0) { 
            return false; 
        } else if (pos == 0) {
            this->push_front(std::move(value)); 
            return true; 
        } else if (pos == m_size) { 
            this->push_back(std::move(value)); 
            return true; 
        }
        node<T>* curr_head = head;
        while (pos != 1) {
            curr_head = curr_head->next;
            --pos;
        }
        node<T>* coming_node = new node<T>(std::move(value));
        coming_node->next = curr_head->next;
        coming_node->prev = curr_head;
        curr_head->next->prev = coming_node;
        curr_head->next = coming_node;
        ++m_size;
        return true;
    }

    T find(unsigned int pos) {
        if (pos > m_size || pos < 0) { 
            return T(); 
        } else if (pos == 0) {
            return head->data;
        } else if (pos == m_size) {
            return tail->data;
        }
        node<T>* curr_head = head;
        while (pos != 1) {
            curr_head = curr_head->next;
            --pos;
        }
        return curr_head->data;
    }

    unsigned int remove_same_elements(unsigned int count, T value) {
        unsigned int removed_count{0};
        if (head == nullptr) { 
            return 0; 
        }
        node<T>* curr = head;
        while (curr != nullptr && count != 0) {
            if (curr->data == value) {
                auto tmp = curr;
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                delete tmp;
                tmp = nullptr;
                --count;
                ++removed_count;
            }
            curr = curr->next;
        }
        return removed_count;
    }

    void erase(unsigned int pos) {
        if (pos > m_size || pos < 0) { 
            return; 
        } else if (pos == 0) { 
            this->pop_front(); 
            return; 
        } else if (pos == m_size) { 
            this->pop_back(); 
            return; 
        }
        node<T>* curr_head = head;
        while (pos != 0) {
            curr_head = curr_head->next;
            --pos;
        }
        if (curr_head->next == nullptr) { 
            this->pop_back(); 
            return ; 
        }
        curr_head->next->prev = curr_head->prev;
        curr_head->prev->next = curr_head->next;
        delete curr_head;
        --m_size;
    }

    void erase(T value) {
        if (head == nullptr) { 
            return; 
        }
        else if (head->data == value) { 
            this->pop_front(); 
            return; 
        } else if (tail->data == value) {
            this->pop_back();
            return;
        }
        node<T>* curr = head;
        while (curr != nullptr) {
            if (curr->data == value) {
                node<T>* tmp = curr;
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                delete tmp;
                tmp = nullptr;
                --m_size;
                return;
            }
            curr = curr->next;
        }
    }

    bool exists(T value) const {
        if (head == nullptr) { 
            return false; 
        }
        node<T>* curr = head;
        while (curr != nullptr) {
            if (curr->data == value) {
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    void resize(unsigned int count) {
        if (count == 0) {
            this->clear();
        } else if (count > m_size) {
            this->insert(m_size, count - m_size, 0);
        } else if (count < m_size) {
            this->erase(count, m_size - count);
        } else { 
            return; 
        }
    }

    void resize(unsigned int count, const T& value) {
        if (count == 0) {
            this->clear();
        } else if (count > m_size) {
            this->insert(m_size, count - m_size, value);
        } else if (count < m_size) {
            this->erase(count, m_size - count);
        } else { 
            return; 
        }
    }

    void swap(List<T>& other) noexcept {
        List<T> tmp(other);
        other = std::move(*this);
        *this = std::move(tmp);
    }

    void sort() {
        head = sort_helper(head);
    }

    void reverse() noexcept {
        node<T>* current = head;
        node<T>* prev = nullptr;
        node<T>* next = nullptr;
        tail = head;
        while (current != nullptr) {
            next = current->next;
            current->next = prev;
            prev = current;
            current->prev = next;
            current = next;
        }
        head = prev;
    }

    friend std::ostream& operator<<(std::ostream& strm, List& list) {
        auto iter = list.begin();
        for(; iter != list.end(); ++iter) {
            strm << *iter << " ";
        }
        std::cout << std::endl;
        return strm;
    }

    // Iterator
    struct Iterator
    {
        // Iteragor tags
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = node<T>;
        using pointer = node<T>*;
        using reference = node<T>&; 
        // Iterator ctors
        Iterator(pointer ptr) : m_ptr(ptr) { }
        Iterator(const Iterator& other) : m_ptr(other.m_ptr) { }
        Iterator& operator=(const Iterator& other) {
            if (this == &other) { 
                return *this; 
            }
            m_ptr = other.m_ptr;
            return *this;
        }
        Iterator& operator=(const T& other) {
            m_ptr = other ;
        }
        ~Iterator() = default;
        void swap(Iterator& other) {
            Iterator tmp(*this);
            *this = std::move(other);
            other = std::move(tmp);
        }
        // Iterator modifiers
        T& operator*() {
            return m_ptr->data;
        }
        pointer operator->() {
            return m_ptr;
        }
        Iterator& operator++() {
            m_ptr = m_ptr->next;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp(*this);
            m_ptr = m_ptr->next;
            return tmp;
        }
        Iterator& operator--() {
            m_ptr = m_ptr->prev;
            return *this;
        }
        Iterator operator--(int) {
            Iterator tmp(*this);
            m_ptr = m_ptr->prev;
            return tmp;
        }
        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs.m_ptr == rhs.m_ptr;
        }
        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return lhs.m_ptr != rhs.m_ptr;
        }
    private:
        pointer m_ptr;
    };
    Iterator begin() {
        return head;
    }
    Iterator end()
    {
        return nullptr;
    }

private:
    unsigned int m_size{0};
    node<T>* head;
    node<T>* tail;

    node<T>* sort_helper(node<T>* root) { // insertion sort
        node<T>* start = new node<T>();
        start->next = root;
        node<T>* curr = root;
        node<T>* prev = start;
        while (curr != nullptr) {
            if (curr->next && (curr->next->data < curr->data)) {
                while (prev->next && (prev->next->data < curr->next->data)) {
                    prev = prev->next;
                }
                node<T>* tmp = prev->next;
                prev->next = curr->next;
                curr->next = curr->next->next;
                prev->next->next = tmp;
                prev = start;
            } else {
                curr = curr->next;
            }
        }
        return start->next;
    }
};

} // DSlist

#endif