#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

namespace DSvector {

template <typename T>
class Vector {
public:
    // Member Functions
    Vector<T>() noexcept: m_size(0), m_capacity(0), buffer(nullptr) { };

    Vector<T>(unsigned int count) : m_capacity(count), m_size(0) {
        buffer = new T[m_capacity];
    }

    Vector<T>(unsigned int count, T value) {
        m_capacity = count;
        m_size = count;
        buffer = new T[m_capacity];
        for (int  i = 0; i < m_size; ++i) {
            buffer[i] = value;
        }
    }

     Vector<T>(const Vector<T>& other) {
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        buffer = new T[m_capacity];
        for (int  i = 0; i < m_size; ++i) {
            buffer[i] = other.buffer[i];
        }
    }

    Vector<T>& operator=(const Vector<T>& other) {
        if (this == &other) { 
            return *this; 
        }        
        if (m_capacity == other.m_capacity) {
            m_size = other.m_size;
            for (int i = 0; i < m_size; ++i) {
                buffer[i] = other.buffer[i];
            }
            return *this;
        }
        delete [] buffer; 
        buffer = nullptr;
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        buffer = new T[m_capacity];
        for (int i = 0; i < m_size; ++i) {
            buffer[i] = other.buffer[i];
        }
        return *this;
    }

    Vector<T>(Vector<T>&& other) noexcept {
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        buffer = other.buffer;
        other.m_capacity = 0;
        other.m_size = 0;
        other.buffer = nullptr;
    }

    Vector<T>& operator=(Vector<T>&& other) noexcept {
        delete [] buffer;
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        buffer = other.buffer;
        other.m_capacity = 0;
        other.m_size = 0;
        other.buffer = nullptr;
        return *this;
    }

    ~Vector<T>() noexcept {
        if(buffer != nullptr) {
            delete[] buffer;
            buffer = nullptr;
        }
        m_capacity = 0;
        m_size = 0;
    }

   // Element Access
    T& operator[]( unsigned int pos ) {
        return buffer[pos];
    }

    const T& operator[](unsigned int pos ) const {
        return buffer[pos];
    }

    // Capacity
    bool empty() const noexcept {
        return m_size > 0;
    }

    unsigned int size() const noexcept {
        return m_size;
    }

    unsigned int capacity() const noexcept {
        return m_capacity;
    }

    bool find(const T& value) const {
        for (int i = 0; i < m_size; ++i) {
            if (buffer[i] == value) {
                return true;
            }
        }
        return false;
    }

    // Modifiers
    void insert(unsigned int pos, const T& value) {
        if (m_size == m_capacity) {
            m_capacity = m_capacity ? m_capacity * 2 : 1;
        }
        T* tmp = new T[m_capacity];
        int i = 0;
        for (; i < pos; ++i) {
            tmp[i] = buffer[i];
        }
        tmp[i] = value;
        ++i;
        for (int j = pos; j < m_size; ++j) {
            tmp[i] = buffer[j];
        }
        swap(tmp);
    }

    void insert(unsigned int pos, const T&& value) {
         if (m_size == m_capacity) {
            m_capacity = m_capacity ? m_capacity * 2 : 1;
        }
        T* tmp = new T[m_capacity];
        int i = 0;
        for (; i < pos; ++i) {
            tmp[i] = buffer[i];
        }
        tmp[i] = value;
        ++i;
        for (int j = pos; j < m_size; ++j) {
            tmp[i] = buffer[j];
        }
        swap(tmp);
        value = 0;
    }

    void swap(Vector<T>& other) noexcept {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        buffer = other.buffer;
        other.m_size = 0;
        other.m_capacity = 0;
        other.buffer = nullptr;
    }

    void push_back(const T& value) {
        if (m_size == m_capacity) {
            m_capacity = m_capacity ? m_capacity * 2 : 1;
            T* tmp = new T[m_capacity];
            for (int i = 0; i < m_size; ++i) {
                tmp[i] = buffer[i];
            }
            delete [] buffer;
            buffer = nullptr;
            buffer = tmp;
            tmp = nullptr;
        }
        buffer[m_size] = value;
        ++m_size;
    }

    void push_back(T&& value) {
        if (m_size == m_capacity) {
            m_capacity = m_capacity ? m_capacity * 2 : 1;
            T* tmp = new T[m_capacity];
            for (int i = 0; i < m_size; ++i) {
                tmp[i] = buffer[i];
            }
            delete [] buffer;
            buffer = nullptr;
            buffer = tmp;
            tmp = nullptr;
        }
        buffer[m_size] = value;
        ++m_size;
    }

    T erase(unsigned int pos) {
        T* tmp = new T[m_size - 1];
        int i = 0;
        for (; i < pos; ++i) {
            tmp[i] = buffer[i];
        }
        for (int j = pos + 1; j < m_size; ++j, ++i) {
            tmp[i] = buffer[j];
        }
        delete[] buffer;
        buffer = nullptr;
        buffer = tmp;
        tmp = nullptr;
        --m_size;
    }

    void pop_back() {
        T* tmp = new T[m_size - 1];
        for (int i = 0; i < (m_size - 1); ++i) {
            tmp[i] = buffer[i];
        }
        delete[] buffer;
        buffer = nullptr;
        buffer = tmp;
        tmp = nullptr;
        --m_size;
    }

    void resize(unsigned int count, T value = T()) { // ???
        if (count == m_size) { 
            return; 
        } else if (count < m_size) {
            T* tmp = new T[count];
            for (unsigned int i = 0; i < count; ++i)
            {
                tmp[i] = buffer[i];
            }
            this->m_size = count;
            buffer = tmp;
            tmp = nullptr;
            return;
        } else {
            T* tmp = new T[count];
            unsigned int  i = 0;
            for (; i < m_size; i++)
            {
                tmp[i] = buffer[i];
            }
            for (; i < count; ++i)
            {
                tmp[i] = value;
            }            
            m_size = count;
            m_capacity = count;
            buffer = tmp;
            tmp = nullptr;
            return;
        }
    }

    void clear() {
        if (buffer != nullptr) {
            delete [] buffer;
            buffer = nullptr;
        }
        m_capacity = 0;
        m_size = 0;
    }



    // Non-member functions
    friend Vector<T> merge(const Vector<T>& lhs, const Vector<T>& rhs) {
        Vector<T> result(lhs);
        for (int i = 0; i < rhs.size(); ++i) {
            if (!lhs.find(rhs.buffer[i])) {
                result.push_back(rhs.buffer[i]);
            }
        }
        return result;
    }

    friend Vector<T> difference(const Vector<T>& lhs, const Vector<T>& rhs) {
        Vector<T> result;
        for (int i = 0; i < lhs.size(); ++i) {
            if (!rhs.find(lhs.buffer[i])) {
                result.push_back(lhs.buffer[i]);
            }
        }
        return result;
    }

    friend Vector<T> intersection(const Vector<T>& lhs, const Vector<T>& rhs) {
        Vector<T> result;
        for (int i = 0; i < rhs.size(); ++i) {
            if (lhs.find(rhs.buffer[i])) {
                result.push_back(rhs.buffer[i]);
            }
        }
        return result;
    }

    friend bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
        if (lhs.m_size != rhs.m_size) {
            return false;
        }
        for (int i = 0; i < lhs.m_size; ++i) {
            if (lhs.buffer[i] != rhs.buffer[i]) {
                return false;
            }
        }
        return true;   
    }

    friend bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
        if (lhs.m_size == rhs.m_size) {
            return false;
        }
        for (int i = 0; i < lhs.m_size; ++i) {
            if (lhs.buffer[i] == rhs.buffer[i]) {
                return false;
            }
        }
        return true;   
    }

    friend bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
        if (lhs.m_size >= rhs.m_size) {
            return false;
        }
        for (int i = 0; i < lhs.m_size; ++i) {
            if (lhs.buffer[i] >= rhs.buffer[i]) {
                return false;
            }
        }
        return true;   
    }

    friend bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
        if (lhs.m_size > rhs.m_size) {
            return false;
        }
        for (int i = 0; i < lhs.m_size; ++i) {
            if (lhs.buffer[i] > rhs.buffer[i]) {
                return false;
            }
        }
        return true;   
    }
    friend bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
        if (lhs.m_size <= rhs.m_size) {
            return false;
        }
        for (int i = 0; i < lhs.m_size; ++i) {
            if (lhs.buffer[i] <= rhs.buffer[i]) {
                return false;
            }
        }
        return true;   
    }

    friend bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
        if (lhs.m_size < rhs.m_size) {
            return false;
        }
        for (int i = 0; i < lhs.m_size; ++i) {
            if (lhs.buffer[i] < rhs.buffer[i]) {
                return false;
            }
        }
        return true;   
    }

    friend Vector<T> operator+(const Vector<T>& lhs, const Vector<T>& rhs) {
        Vector<T> tmp = lhs;
        for (int i = 0; i < rhs.m_size; ++i) {
                tmp.push_back(rhs[i]);
        }
        return tmp;
    }

    Vector<T>& operator+=(const Vector<T>& other) {
        for (int i = 0; i < other.m_size; ++i) {
            push_back(other[i]);
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& strm, const Vector& vec) {
        for (unsigned int i = 0; i < vec.m_size; ++i)
        {
            strm << vec.buffer[i];
        }
        return strm;
    }

private:
    unsigned int m_size{};
    unsigned int m_capacity{};
    T* buffer{nullptr}; 
};

} //DS vector

#endif