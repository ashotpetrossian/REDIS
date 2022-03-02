#ifndef MIN_HEAP_H_
#define MIN_HEAP_H_

#include <iostream>
#include <math.h>
#include "vector.h"

using namespace DSvector;

namespace DSheap {

template <typename T>
class MinHeap {
public: 
    // Member functions
    MinHeap() : array(nullptr), m_capacity(0), m_size(0) { }

    MinHeap(unsigned int cap) : m_capacity(cap), m_size(0) {
        array = new pair[m_capacity];
    }

    MinHeap(const MinHeap& other) {
        for (unsigned int i = 0; i < m_size; ++i) {
            array[i] = other.array[i];
        }
        m_size = other.size;
        m_capacity = other.m_capacity;
    }

    MinHeap(MinHeap&& other) {
        for (unsigned int i = 0; i < m_size; ++i) {
            array[i] = other.array[i];
        }
        m_size = other.size;
        m_capacity = other.m_capacity;
        other.array = nullptr;
        other.m_capacity = 0;
        other.m_size = 0;
    }

    MinHeap& operator=(const MinHeap& other) {
        if (this == &other) {
            return *this;
        }
        for (unsigned int i = 0; i < m_size; ++i) {
            array[i] = other.array[i];
        }
        m_size = other.size;
        m_capacity = other.m_capacity;
        return *this;
    }

    MinHeap& operator=(MinHeap&& other) {
        if (this == &other) {
            return *this;
        }
        for (unsigned int i = 0; i < m_size; ++i) {
            array[i] = other.array[i];
        }
        m_size = other.size;
        m_capacity = other.m_capacity;
        other.array = nullptr;
        other.m_capacity = 0;
        other.m_size = 0;
        return *this;
    }

    ~MinHeap() {
        m_size = 0;
        m_capacity = 0;
        delete[] array;
    }

    // Modifiers
    int extract_min() { 
        if (m_size <= 0) {
            return INT32_MAX; // ???? the string should be converted to integer
        } else if (m_size == 1) {
            --m_size;
            return array[0].m_key;
        }
        int root = array[0].m_key;
        array[0] = array[m_size - 1]; // last element comes to the root
        --m_size;
        min_heapify(0);
        return root; // returning the extracted root 
    }

    // [zadd]
    // string must be converted into int to be inserted
    // compare is done using keys 
    void insert_key(int key, T value) { 
        if (m_size == m_capacity) {
            m_capacity = m_capacity ? m_capacity * 2 : 1;
        }
        pair* tmp_arr = new pair[m_capacity];
        for (unsigned int i = 0; i < m_size; ++i)
        {
            tmp_arr[i] = array[i];
        }
        ++m_size;
        unsigned int index = m_size - 1;
        tmp_arr[index].m_key = key;
        tmp_arr[index].m_value = value;
        while (index != 0 && tmp_arr[parent(index)].m_key > tmp_arr[index].m_key) {
            pair temp = tmp_arr[index];
            tmp_arr[index] = tmp_arr[parent(index)];
            tmp_arr[parent(index)] = temp;
            index = parent(index);
        }  
        array = tmp_arr;
        tmp_arr = nullptr;     
    }

    void decrease_key(unsigned int i, int new_value) { // i is index, new_value is the smallest integer in 32 bytes
        array[i].m_key = new_value;
        while (i != 0 && array[parent(i)].m_key > array[i].m_key) {
            pair tmp = array[i];
            array[i] = array[parent(i)];
            array[parent(i)] = tmp;
            i = parent(i);
        }
    }

    // [zrem]
    // for client code delete operation is done using only value
    // returns the number of successful removals
    int delete_value(T value) { 
        unsigned int index = get_index_by_value(value);
        if (index >= m_size) { 
            return 0; 
        }
        decrease_key(index, INT32_MIN);
        extract_min();
        return 1;
    }

    unsigned int get_index_by_value(T value) const {
        for (unsigned int i = 0; i < m_size; ++i) {
            if (array[i].m_value == value) {
                return i;
            }
        }
        return -1; // can be checked for out of bounds
    }

    void min_heapify(unsigned int i) {
        unsigned int l = left(i);
        unsigned int r = right(i);
        unsigned int index = i;
        if (l < m_size && array[l].m_key < array[i].m_key) {
            index = l;
        }
        if (r < m_size && array[r].m_key < array[index].m_key) {
            index = r;
        }
        if (index != i) {
            pair tmp = array[i];
            array[i] = array[index];
            array[index] = tmp;
            min_heapify(index);
        }
    }

    // Non-modifiers
    bool key_exists(int key) const {
        for (unsigned int i = 0; i < m_size; ++i) {
            if (array[i].m_key == key) {
                return true;
            }
        }
        return false;
    }

    bool value_exists(T value) const {
        for (unsigned int i = 0; i < m_size; ++i) {
            if (array[i].m_value == value) {
                return true;
            }
        }
        return false;
    }

    T get_value_by_key(int key) const {
        for (unsigned int i = 0; i < m_size; ++i) {
            if (array[i].m_key == key) {
                return array[i].m_value;
            }
        }
        return T(); // better to check if exists before calling this function
    }

    // for [zscore] , returns the priority key
    int get_key_by_value (T value) const {
        for (unsigned int i = 0; i < m_size; ++i) {
            if (array[i].m_value == value) {
                return array[i].m_key;
            }
        }
        return INT32_MAX; // as well as for extracting.. if fails checking the greater element in int
    }

    void print_array() const {
        for (unsigned int i = 0; i < m_size; ++i) {
            std::cout << array[i].m_key << " --- " << array[i].m_value << std::endl;
        }
        std::cout << std::endl;
    } // Added for int to string conversion

    unsigned int height() const {
        return ceil(log2(m_size + 1)) - 1;
    }

    // [zcard]
    unsigned int size() const {
        return m_size;
    }

    // for [zrange], returning vector for easy handling
    Vector<T> get_all_values_by_priority() {
        Vector<T> vec(m_size);
        for (unsigned int i = 0; i < m_size; ++i) {
            vec.push_back(array[i].m_value);
        }
        return vec;
    }

    unsigned int parent(unsigned int index) const {
        return (index - 1) / 2;
    }

    unsigned int left(unsigned int index) const {
        return (2 * index + 1);
    }

    unsigned int right(unsigned int index) const {
        return (2 * index + 2);
    }

private:
    struct pair {
        int m_key;
        T m_value;
    };
    pair* array{nullptr};
    unsigned int m_capacity;
    unsigned int m_size;
};

}// DSheap

#endif