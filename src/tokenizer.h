#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "unordered_map.h"
#include "string.h"
#include "vector.h"
#include "list.h"
#include "set.h"
#include "min_heap.h"


using namespace DSunordered_map;
using namespace DSstring;
using namespace DSvector;
using namespace DSlist;
using namespace DSset;
using namespace DSheap;

namespace DBTokenizer {

class Tokenizer {
public:
    // Database
    Hash<String, List<String>*> m_list_map;
    Hash<String, Hash<String,String>*> m_hash_map;
    Hash<String, Set<String>*> m_set_map;
    Hash<String, String*> m_string_map;
    Hash<String, MinHeap<String>*> m_heap_map;

public:
    Tokenizer() {
        String m_buf_str{};
        String m_command{};
        String m_name{};
        String m_args{};
        Vector<String> m_vec_args{};
    }

    Tokenizer(const Tokenizer&) = default;
    Tokenizer(Tokenizer&&) = default;
    Tokenizer& operator=(const Tokenizer&) = default;
    Tokenizer& operator=(Tokenizer&&) = default;

    ~Tokenizer() = default;

    // Modifiers
    unsigned int char_len (char* buffer) {
        unsigned int i = 0;
        for (; buffer[i] != '\0'; ++i);
        return i;
    }

    void set_buffer(char* buffer) { 
        for (unsigned int i = 0; i < char_len(buffer); ++i) {
            m_buf_str.push_back(*(buffer + i));
        }
    } 
    String get_command() { return m_command;}
    String get_name() { return m_name; }
    Vector<String> get_m_vec_args() { return m_vec_args; }

    void trim_buf_str() {
        int count_space = 0;
        int count_quotes = 2;
        for(int i = 0; i < m_buf_str.size(); ++i)
        {
            if((m_buf_str[i] != ' ' && i != 0 && m_buf_str[i - 1] == ' ') && ((count_quotes % 2 == 0) || (count_quotes == 2))) {
                ++count_space; 
            }
            
            if (i >= 1 && m_buf_str[i-1] == '"' && m_buf_str[i] != ' ' && count_quotes % 2 == 0 && count_space < 2) {
                m_name = "Wrong Name";
                return;
            }

            if ((count_quotes % 2 == 1)) {
                if (count_space == 0 || (count_space == 1 && m_buf_str[0] == ' ')) {
                    m_command.push_back(m_buf_str[i]);
                } else if (count_space == 1 || (count_space == 2 && m_buf_str[0] == ' ')) {
                    m_name.push_back(m_buf_str[i]);
                } else if(m_buf_str[i] != '\n') {
                    m_args.push_back(m_buf_str[i]);
                }
            } else if (count_quotes % 2 == 0 ) {
                count_quotes = 2;
                if ((count_space == 0 || (count_space == 1 && m_buf_str[0] == ' ')) && m_buf_str[i] != ' ' && m_buf_str[i] != '\n') {
                    m_command.push_back(m_buf_str[i]);
                } else if ((count_space == 1 || count_space == 2 && m_buf_str[0] == ' ') && m_buf_str[i] != ' ' && m_buf_str[i] != '\n') {
                    m_name.push_back(m_buf_str[i]);
                } else if(m_buf_str[i] != '\n') {
                    m_args.push_back(m_buf_str[i]); 
                }
            }

            if(m_buf_str[i] == '"') {
                ++count_quotes;
            }
        }
    }

    void trim_args() {
        int count_quotes = 2;
        String trimmer = "";
        for (int i = 0; i < m_args.size(); ++i) {
            if (i >= 1 && m_args[i-1] == '"' && m_args[i] != ' ' && count_quotes % 2 == 0) {
                m_vec_args.push_back("Wrong Argument");
                return;
            }
            if (m_args[i] == '"') {
                ++count_quotes;
            }

            if (count_quotes % 2 == 1 && m_args[i] != '"') {
                trimmer += m_args[i];
            } else if (count_quotes % 2 == 0 && m_args[i] != ' ' && m_args[i] != '"') {
                trimmer += m_args[i];
            }

            if (((m_args[i] == ' ') || (m_args[i] == '\n')|| (m_args[i] == '\0') || (i == m_args.size() - 1)) && (trimmer != "") && (count_quotes % 2 == 0)) {
                m_vec_args.push_back(trimmer); 
                trimmer = "";
                count_quotes = 2;
            }
        }
    }

    bool check_name() { 
        if (m_name.size() == 0) { return true; } 
        return false;
    } 
    bool check_args() { 
        if (m_vec_args.size() == 0) { return true; } 
        return false;
    } 

    void clear_tokenizer() {
        m_buf_str.clear();
        m_command.clear();
        m_name.clear();
        m_args.clear();
        m_vec_args.clear();
    }

private:
    String m_buf_str;
    String m_command;
    String m_name;
    String m_args;
    Vector<String> m_vec_args;
};

} // DBTokenizer

#endif
