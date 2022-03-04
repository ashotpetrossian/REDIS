#ifndef UNORDERED_MAP_H_
#define UNORDERED_MAP_H_

#include <iostream>

#include "vector.h"
#include "list.h"


using namespace DSvector;
using namespace DSlist;


namespace DSunordered_map {

template <typename T1, typename T2>
class Hash {
public:
    // Member functions
    Hash(unsigned int size = 20) : s_size(size), table(s_size, nullptr) { }
    Hash(const Hash&) = default;
    Hash(Hash&&) = default;
    Hash& operator=(const Hash&) = default;
    Hash& operator=(Hash&&) = default;
    ~Hash() = default;

    // Non-modifiers
    double load_factor() const {
        double n{}; 
        double m = static_cast<double>(s_size);
        for (unsigned int i = 0; i < table.size(); ++i) {
            if (table[i] != nullptr) {
                ++n;
            }
        }
        return n / m;
    }

    bool is_empty() const {
        for (int i = 0; i < table.size(); ++i) {
            if (table[i] != nullptr) { 
                return false; 
            }
        }
        return true;
    }

    unsigned int hash_function(T1 key) {
        if (key.size() == 0) { 
            return 0; 
        } else if (key.size() == 1) {
            return (((key[0] << 3) ^ key[0] >> 3) ^ key[0]) % s_size; 
        } else if (key.size() == 2) {
            return (((((key[0] << 3) ^ (key[0] >> 3)) ^ key[0])
                + (((key[1] << 5) ^ (key[1] >> 5)) ^ key[1])) % s_size); 
        }
        return ((((((key[0] << 3) ^ (key[0] >> 3)) ^ key[0])
                + (((key[1] << 5) ^ (key[1] >> 5)) ^ key[1])
                + (((key[2] << 7) ^ (key[2])>> 7)) ^ key[2])) % s_size);
    }

    T2 get_value_by_key(T1 key) {
        unsigned int index = hash_function(key);
        if (table[index] == nullptr) {
            return T2(); 
        } // must be called in parser
        auto iter = table[index]->begin();
        for (; iter != table[index]->end(); ++iter) {
            if (iter->data.m_key == key) {
                return iter->data.m_value;
            }
        }
        return T2();
    }

    unsigned int table_length() const {
        unsigned int count{};
        for (unsigned int i = 0; i < table.size(); ++i) {
            if (table[i] != nullptr) {
                count += table[i]->size();
            }
        }
        return count;
    }

    unsigned int free_slots() const {
        unsigned int count{};
        for (unsigned int i = 0; i < table.size(); ++i) {
            if (table[i] == nullptr) {
                count++;
            }
        }
        return count;
    }

    Vector<T1> get_keys() const {
        Vector<T1> keys;
        for (unsigned int i = 0; i < table.size(); ++i) {
            if (table[i] == nullptr) { 
                continue; 
            }
            auto iter = table[i]->begin();
            for (; iter != table[i]->end(); ++iter) {
                keys.push_back(iter->data.m_key);
            }            
        }
        return keys;
    }

    Vector<T2> get_values() const {
        Vector<T2> values;
        for (unsigned int i = 0; i < table.size(); ++i) {
            if (table[i] == nullptr) { 
                continue; 
            }
            auto iter = table[i]->begin();
            for (; iter != table[i]->end(); ++iter) {
                values.push_back(iter->data.m_value);
            }            
        }
        return values;
    }

    bool exists_key(T1 key) {
        unsigned int index = hash_function(key);
        if (table[index] == nullptr) { 
            return false; 
        }
        auto iter = table[index]->begin();
        for (; iter != table[index]->end(); ++iter) {
            if (iter->data.m_key == key) {
                return true;
            }
        }
        return false;
    }

    void print() const {
        for (unsigned int i = 0; i < table.size(); ++i) {
            if (table[i] == nullptr) {
                std::cout << "table [" << i << "] :: \n";
                continue; 
            }
            std::cout << "table [" << i << "] :: ";
            auto iter = table[i]->begin();
            for(; iter != table[i]->end(); ++iter) {
                std::cout << iter->data.m_key  << " -> " << *(iter->data.m_value) << " ";
            }
            std::cout << std::endl;
        }        
    }

    // Modifiers
    void rehash() {
        s_size *= 2;
        Hash tmp(s_size);
        for (unsigned int i = 0; i < table.size(); ++i) {
            if (table[i] == nullptr) { 
                continue; 
            }
            auto iter = table[i]->begin();
            for (; iter != table[i]->end(); ++iter) {
                tmp.insert(iter->data.m_key, iter->data.m_value);
            }
        }  
        swap(tmp);      
    }

    void swap(Hash& tmp) {
        s_size = tmp.s_size;
        table = std::move(tmp.table);
    }
    
    bool insert(T1 key, T2 value) { // modified void to bool
        if (load_factor() == 1.0) { 
            rehash(); 
        }
        unsigned int index = hash_function(key);
        if (exists_key(key)) { 
            return false; 
        } 
        pair tmp; tmp.m_key = key; tmp.m_value = value;
        if (table[index] == nullptr) {
            table[index] = new List<pair>;
        }
        table[index]->push_front(tmp);
        return true;
    }

    void remove(T1 key) {
        unsigned int index = hash_function(key);
        if (table[index] == nullptr) { return;  }
        auto iter = table[index]->begin();
        unsigned int pos{};
        for (; iter != table[index]->end(); ++iter) {
            if (iter->data.m_key == key) {
                table[index]->erase(pos);
            }
            ++pos;
        }                       
    }

    T2& operator[](T1 key) {
        unsigned int index = hash_function(key); 
        if (table[index] != nullptr) {
            auto iter = table[index]->begin();
            for (; iter != table[index]->end(); ++iter) {
                if (iter->data.m_key == key) {
                    return iter->data.m_value;
                }
            }
        }
        if (load_factor() == 1.0) { 
            rehash(); 
        }
        index = hash_function(key);
        pair tmp; 
        tmp.m_key = key;
        if (table[index] == nullptr) {
            table[index] = new List<pair>;  
        }
        table[index]->push_front(tmp);
        auto iter = table[index]->begin();
        return iter->data.m_value;
    }

    void clean() { // call when needed
        for (unsigned int i = 0; i < table.size(); ++i) {
            if (table[i] == nullptr) { 
                continue; 
            }
            auto iter = table[i]->begin();
            for (; iter != table[i]->end(); ++iter) {
                delete iter->data.m_value;
                iter->data.m_value = nullptr;
                iter->data.m_key.clear();
            } 
        }
    }

private:
    struct pair {
        T1 m_key;
        T2 m_value;
    };
    unsigned int s_size = 20;
    Vector<List<pair>*> table;
};

} // DSunordered_map

#endif