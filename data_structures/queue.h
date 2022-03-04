#ifndef QUEUE_H_
#define QUEUE_H_

#include "vector.h"

using namespace DSvector;

namespace DSqueue {

template<typename T>
class Queue {
public:
    // Member functions
    Queue() : m_size(0), m_vec(0) { }

    Queue(unsigned int size) : m_size(size), m_vec(m_size) { }

    Queue(const Queue<T>& other) {
        m_size = other.m_size;
        m_count = other.m_count;
        m_vec = other.m_vec;
    }

    Queue(Queue<T>&& other) {
        m_size = other.m_size;
        m_count = other.m_count;
        m_vec = other.m_vec;
        other.m_size = 0;
        other.m_count = 0;
        other.m_vec.clear();
    }

    Queue& operator=(const Queue<T>& other) {
        if (this == &other) { 
            return *this; 
        }
        m_size = other.m_size;
        m_vec = other.m_vec;
        m_count = other.m_count;
        return *this;
    }

    Queue& operator=(Queue<T>&& other) {
        if (this == &other) { 
            return *this; 
        }
        m_size = other.m_size;
        m_count = other.m_count;
        m_vec = other.m_vec;
        other->m_size = 0;
        other.m_count = 0;
        other.m_vec.clear();
        return *this;
    }

    ~Queue() = default;

    // Element access
    T& front() {
        return m_vec[m_count];
    }
    // Capacity

    bool empty() const {
        return this->size() == 0;
    }

    unsigned int size() const {
        return m_size - m_count;
    }

    //Modifiers
    void push(const T& value) {
        m_vec.push_back(value);
        ++m_size;
    }

    void push(T&& value) {
        m_vec.push_back(std::move(value));
        ++m_size;
    }

    void pop() {
        if (!this->empty()) {
            m_vec[m_count] = 0;
            ++m_count;
        }    
    }
    
private:
    unsigned int m_count{0};
    unsigned int m_size;
    Vector<T> m_vec;
};

} // DSqueue

#endif