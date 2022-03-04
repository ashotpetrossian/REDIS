#ifndef STRING_H_
#define STRING_H_

#include <iostream>

namespace DSstring {

class String {
public:
    // Member functions
    String() noexcept : m_size(0) {
        buffer = new char[1];
        buffer[0] = '\0';
    }

    String(unsigned int count, char ch) {
        m_size = count;
        buffer = new char[m_size + 1];
        for (int i = 0; i < m_size; ++i) {
            buffer[i] = ch;
        }
        buffer[m_size] = '\0';
    }

    String(const char* arr) {
        int len = 0;
        while (arr[len] != '\0') {
            ++len;
        }
        buffer = new char[len + 1];
        for (int i = 0; i < len; ++i) {
            buffer[i] = arr[i];
        }
        m_size = len;
        buffer[m_size] = '\0';
    }

    String(const String& other) {
        m_size = other.m_size;
        buffer = new char[m_size];
        for (unsigned int i = 0; i < m_size; ++i) {
            buffer[i] = other.buffer[i];
        }       
    }

    String(String&& other) noexcept {
        m_size = other.m_size;
        buffer = other.buffer;
        other.m_size = 0;
        other.buffer = nullptr;
    }

    ~String() noexcept {
        m_size = 0;
        if (buffer != nullptr) {
            delete [] buffer;
            buffer = nullptr;
        }
    }

    String& operator=(const String& other) {
        if (this == &other) { 
            return *this; 
        }
        m_size = other.m_size;
        delete [] buffer;
        buffer = new char[m_size + 1];
        for (int i = 0; i < m_size; ++i) {
            buffer[i] = other.buffer[i];
        }
        buffer[m_size] = '\0';
        return *this;
    }

    String& operator=(String&& other) noexcept {
        if (this == &other) { 
            return *this; 
        }
        m_size = other.m_size;
        delete [] buffer;
        buffer = other.buffer;
        other.m_size = 0;
        other.buffer = nullptr;
        return *this;
    }

    char& operator[](unsigned int pos) {
        return buffer[pos];
    }

    const char& operator[](unsigned int pos) const {
        return buffer[pos];
    }

    const char* c_str() const noexcept {
        return static_cast<const char*>(buffer);
    }

    // Capacity

    bool empty() const noexcept {
        return m_size == 0;
    }

    unsigned int size() const noexcept {
        return m_size;
    }

    // Operations 

    void clear() noexcept {
        m_size = 0;
        delete [] buffer;
        buffer = nullptr;
    }

    String& insert(unsigned int pos, char symbol) {
        String tmp(this->m_size + 1, '\0');
        for (int i = 0; i < pos; ++i) {
            tmp.buffer[i] = buffer[i];
        }
        tmp.buffer[pos] = symbol;
        int j = pos;
        for (int i = 0; i < this->m_size + 1; ++i, ++j) {
            tmp.buffer[i] = buffer[j];
        }
        this->swap(tmp);
        return *this;            
    }

    String& insert(unsigned int pos, String str) {
        String tmp(this->m_size + str.m_size, '\0');
        int i = 0; int j = 0;
        for (; i < pos; ++i) {
            tmp.buffer[i] = buffer[i];
        }
        for (; j < str.m_size; ++i, ++j) {
            tmp.buffer[i] = str.buffer[j];
        }
        int k = pos;
        for (; i < m_size + str.m_size; ++i, ++k) {
            tmp.buffer[i] = buffer[k];
        }
        swap(tmp);
        return *this;
    }

    String& erase(unsigned int pos) {
        String tmp(this->m_size - 1, '\0');
        char ch = buffer[pos];
        int i = 0;
        for (; i < pos; ++i) {
            tmp.buffer[i] = buffer[i];
        }
        for (int j = pos + 1; j < m_size; ++j, ++i) {
            tmp.buffer[i] = buffer[j];
        }
        swap(tmp);
        return *this;
    }

    String substr(unsigned int pos, unsigned int count) {
        String tmp(count, '\0');
        for (int i = pos, j = 0; j < count && i < m_size; ++i, ++j) {
            tmp.buffer[j] = buffer[i];
        }
        return tmp;
    }

    bool find(const char ch) const {
        for (int i = 0; i < m_size; ++i)
        {
            if (buffer[i] == ch) {
                return true;
            }
        }
        return false;
    }

    bool find(const String str) const {
        for (int i = 0; i < m_size; ++i)
        {
            int count = 0; int j = 0;
            while (j < str.m_size && buffer[i] == str.buffer[i]) {
                ++count; ++j; ++i;
            }
            if (count != 0) { 
                --i; 
            }
            if (count == str.m_size) {
                return true;
            }
        }
        return false;
    }

    void swap(String& other) noexcept {
        m_size = other.m_size;
        buffer = other.buffer;
        other.m_size = 0;
        other.buffer = nullptr;
    }

    void push_back(const char ch) {
        String tmp(m_size + 1, '\0');
        for (int i = 0; i < m_size; ++i) {
            tmp.buffer[i] = buffer[i];
        }
        tmp.buffer[m_size] = ch;
        swap(tmp);
    }

    void pop_back() {
        String tmp(m_size - 1, '\0');
        for (int i = 0; i < m_size - 1; ++i) {
            tmp.buffer[i] = buffer[i];
        }
        swap(tmp);
    }

    String& operator+=(const char ch) {
        this->push_back(ch);
        return *this;
    }

    String& operator+=(const String& str) {
        String tmp(m_size + str.m_size + 1, '\0');
        int i = 0;
        for (; i < m_size; ++i) {
            tmp.buffer[i] = buffer[i];
        }
        for (int j = 0; j < str.m_size; ++i, ++j) {
            tmp.buffer[i] = str.buffer[j];
        }
        m_size = tmp.m_size;
        buffer = tmp.buffer;
        tmp.buffer = nullptr;
        return *this;
    }

    // Non-member functions
    friend String operator+(const String& str, const char ch) {
        String tmp(str.m_size + 1, '\0');
        for (int i = 0; i < str.m_size; ++i) {
            tmp.buffer[i] = str.buffer[i];
        }
        return tmp;
    }

    friend String operator+(const String& lhs, const String& rhs) {
        String tmp(lhs.m_size + rhs.m_size + 1, '\0');
        int i = 0;
        for (; i < lhs.m_size; ++i) {
            tmp.buffer[i] = lhs.buffer[i];
        }
        for (int j = 0; j < rhs.m_size; ++i, ++j) {
            tmp.buffer[i] = rhs.buffer[j];
        }
        return tmp;
    }

    friend bool operator==(const String& lhs, const String& rhs) {
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

    friend bool operator!=(const String& lhs, const String& rhs) {
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

    friend bool operator<(const String& lhs, const String& rhs) {
        if (lhs.m_size > rhs.m_size) {
            return false;
        } else if (lhs.m_size < rhs.m_size) {
            return true;
        }
        for (int i = 0; i < lhs.m_size; ++i) {
            if (int(lhs.buffer[i]) < int(rhs.buffer[i])) {
                return true;
            } else {
                return false;
            }
        }
        return false;
    }

    friend bool operator<=(const String& lhs, const String& rhs) {
        if (lhs.m_size > rhs.m_size) {
            return false;
        } else if (lhs.m_size < rhs.m_size) {
            return true;
        }
        for (int i = 0; i < lhs.m_size; ++i) {
            if (lhs.buffer[i] < rhs.buffer[i]) {
                return true;
            } else if (lhs.buffer[i] == rhs.buffer[i]) {
                continue;
            } else {
                return false;
            }
        }
        return true; 
    }

    friend bool operator>(const String& lhs, const String& rhs) {
        if (lhs.m_size < rhs.m_size) {
            return false;
        } else if (lhs.m_size > rhs.m_size) {
            return true;
        }
        for (int i = 0; i < lhs.m_size; ++i){
            if (lhs.buffer[i] > rhs.buffer[i]) {
                return true;
            } else {
                return false;
            }
        }
        return false;
    }

    friend bool operator>=(const String& lhs, const String& rhs) {
        if (lhs.m_size < rhs.m_size) {
            return false;
        } else if (lhs.m_size > rhs.m_size) {
            return true;
        }
        for (int i = 0; i < lhs.m_size; ++i) {
            if (lhs.buffer[i] > rhs.buffer[i]) {
                return true;
            } else if(lhs.buffer[i] == rhs.buffer[i]) {
                continue;
            } else {
                return false;
            }
        }
        return true; 
    }

    friend std::ostream& operator<<(std::ostream& strm, const String& str) {
        for (int i = 0; i < str.m_size; ++i) {
            strm << str.buffer[i];
        }
        return strm;
    }

    friend std::istream& operator>>(std::istream& strm, String& str) {
        char* tmp = new char[String::bufSIZE];
        char c; int i = 0;
        while(c != '\n'){
            c = getchar();
            tmp[i++] = c;
        }
        tmp[i] = '\0';
        str.m_size = i - 1;
        str.buffer = new char[str.m_size + 1];
        for(int i = 0 ; i < str.m_size ; i++){
            str.buffer[i] = tmp[i];
        }
        delete [] tmp;
        tmp = nullptr;
        str.buffer[str.m_size + 1] = '\0';
        return strm;
    }

    // Numeric conversions
    int stoi() {
        int tmp {0};
        for (int i = 0; i < m_size; ++i) {
            if (i == 0 && buffer[i] == '-') {
                continue;
            }
            tmp = tmp * 10 + (buffer[i] - '0');
        }
        if (buffer[0] == '-') {
            tmp *= -1;
        }
        return tmp;
    }

    String itos(int num) {
        String str1;
        String str2;
        if (num < 0) {
            str2.push_back('-');
            if (num == INT32_MIN) {
                ++num;
            }
            num *= -1;
        }
        int tmp = num;
        while (tmp != 0) {
            char n = '0' + (tmp % 10);
            str1.push_back(n);
            tmp /= 10;
        }
        for (unsigned int i = 0; i < str1.size(); ++i) {
            str2.push_back(str1[str1.size() -1 - i]);
        }
        return str2;
    }

private:
    unsigned int m_size{};
    char* buffer{nullptr};
    static const unsigned int bufSIZE = 255;
};

} // DSstring

#endif