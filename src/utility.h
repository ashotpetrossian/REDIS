#ifndef UTILITY
#define UTILITY

#include "tokenizer.h"

using namespace DBTokenizer;

namespace Utility {

bool is_int(String& buf) {
    for (unsigned int i = 0; i < buf.size(); ++i) {
        if (i == 0 && buf[i] == '-') {
            continue;
        }
        if (buf[i] < '0' || buf[i] > '9') {
                return false;
        }
    }
    String max = max.itos(INT32_MAX);
    if (buf[0] != '-' && buf > max) {
        buf = max;
    }
    String min = min.itos(INT32_MIN);
    if (buf[0] == '-' && buf > min) {
        buf = min;
    }
    return true;
}

bool check(Tokenizer& tokenizer, String& buf) {
    if (tokenizer.check_name()) {
        buf = "Cannot have an empty name for ";
        return true;
    } else if (tokenizer.get_name() == "Wrong Name") {
        buf = "Wrong input for a name for";
        return true;
    } else if (tokenizer.check_args()) {
        buf = "Wrong number of arguments for ";
        return true;
    } else if (tokenizer.get_m_vec_args()[tokenizer.get_m_vec_args().size() - 1] == "Wrong Argument") {
        buf = "Wrong argument for ";
        return true;
    }
    return false;
}

bool check_args (Tokenizer& tokenizer, String& buf, const String& command_name, unsigned int args) {
    String s_args = s_args.itos(args + 1);
    if (check(tokenizer, buf)) {
        if (tokenizer.check_name()) {
            buf = "Wrong number of arguments for ";
            buf += command_name;
            buf += " (0, instead of ";
            buf += s_args;
            buf += ")";
            return true;
        } else if (args == 0) {
            buf.clear();
        } else if (args > 0) {
            buf = "Wrong number of arguments for ";
            buf += command_name;
            buf += " (1, instead of ";
            buf += s_args;
            buf += ")";
            return true;
        } else {
            buf += command_name;
            return true;
        }
    } else if (tokenizer.get_m_vec_args().size() != args) {
        String v_size = v_size.itos((tokenizer.get_m_vec_args().size() + 1));
        v_size += " instead of ";            
        buf = "Wrong number of arguments for ";
        buf += command_name; 
        buf += " (";
        buf += v_size;
        buf += s_args;
        buf += ")";
        return true;
    }
    return false;
}

void all_keys(Tokenizer& tokenizer, String& buf, int& buf_lines, const Vector<String>& keys) {
    for (unsigned int i = 0; i < keys.size(); ++i) {
        ++buf_lines;
        buf += buf.itos(buf_lines);
        buf += ") \"";
        buf += keys[i];
        buf += "\"";
        buf += '\n';
    }
}

// General Commands

// ping
String Pong(Tokenizer& tokenizer) { 
    String buf;
    String command_name = "ping";
    unsigned int args = 0;
    if (!tokenizer.check_name() || !tokenizer.check_args()) {
        buf = "Try pinging me again, with just 'ping' command!";
        return buf;
    }
    buf = "pong! (I know, I am fun at parties -_-)";
    return buf;
}

// keys*
String Keys(Tokenizer& tokenizer) { 
    String buf;
    String command_name = "keys*";
    unsigned int args = 0;
    if (!tokenizer.check_name() || !tokenizer.check_args()) {
            buf = "keys* command needs no name or arguments";
            return buf;
    }

    Vector<String> m_list_keys = tokenizer.m_list_map.get_keys();
    Vector<String> m_hash_keys = tokenizer.m_hash_map.get_keys();
    Vector<String> m_set_keys = tokenizer.m_set_map.get_keys();
    Vector<String> m_string_keys = tokenizer.m_string_map.get_keys();
    Vector<String> m_heap_keys = tokenizer.m_heap_map.get_keys();
    
    int buf_lines = 0;
    if (m_list_keys.size() != 0) {
        all_keys(tokenizer, buf, buf_lines, m_list_keys);
    }
    if (m_hash_keys.size() != 0) {
        all_keys(tokenizer, buf, buf_lines, m_hash_keys);
    }
    if (m_set_keys.size() != 0) {
        all_keys(tokenizer, buf, buf_lines, m_set_keys);
    }
    if (m_string_keys.size() != 0) {
        all_keys(tokenizer, buf, buf_lines, m_string_keys);
    }
    if (m_heap_keys.size() != 0) {
        all_keys(tokenizer, buf, buf_lines, m_heap_keys);
    }

    if (buf.size() == 0) {
        buf = "(integer) 0";
    } else {
        buf.pop_back();
    }
    return buf;
}

// flushall
String Flushall(Tokenizer& tokenizer) { 
    String buf;
    String command_name = "flushall";
    unsigned int args = 0;
    if (!tokenizer.check_name() || !tokenizer.check_args()) {
            buf = "flushall command needs no name or arguments";
            return buf;
    }

    tokenizer.m_list_map.clean();
    tokenizer.m_hash_map.clean();
    tokenizer.m_set_map.clean();
    tokenizer.m_string_map.clean();
    tokenizer.m_heap_map.clean();

    buf = "OK";
    return buf;
}

// List Functions

// lpush
String Lpush(Tokenizer& tokenizer) {
    String buf;
    if (Utility::check(tokenizer, buf)) {
        buf += "lpush";
        return buf;
    } else {
        if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
            List<String>* new_list_str = new List<String>;
            tokenizer.m_list_map[tokenizer.get_name()] = new_list_str;
        }

        for (int i = 0; i < tokenizer.get_m_vec_args().size(); ++i) {
            tokenizer.m_list_map[tokenizer.get_name()]->push_front(tokenizer.get_m_vec_args()[i]);        
        } 

        buf = "(integer) ";
        String v_size = v_size.itos(tokenizer.m_list_map[tokenizer.get_name()]->size());
        buf += v_size;  
    }
    return buf;
}

// rpush
String Rpush(Tokenizer& tokenizer) {
    String buf;
    if (Utility::check(tokenizer, buf)) {
        buf += "rpush";
        return buf;
    } else {
        if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
            List<String>* new_list_str = new List<String>;
            tokenizer.m_list_map[tokenizer.get_name()] = new_list_str;
        }

        for (int i = 0; i < tokenizer.get_m_vec_args().size(); ++i) {
            tokenizer.m_list_map[tokenizer.get_name()]->push_back(tokenizer.get_m_vec_args()[i]);        
        } 

        buf = "(integer) ";
        String v_size = v_size.itos(tokenizer.m_list_map[tokenizer.get_name()]->size());
        buf += v_size;  
    }
    return buf;
}

// lrange
String Lrange(Tokenizer& tokenizer) {
    String buf;
    String command_name = "lrange";
    unsigned int args = 2;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    
    if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
        buf = "empty list or set";
        return buf;
    }

    String arg1 = tokenizer.get_m_vec_args()[0];
    String arg2 = tokenizer.get_m_vec_args()[1];
    unsigned int list_size = tokenizer.m_list_map[tokenizer.get_name()]->size();
    if (!(is_int(arg1) && is_int(arg2))) {
        buf = "ERR value is not an integer, or out of range";
        return buf; 
    }

    int range1 = arg1.stoi();
    int range2 = arg2.stoi();

    if (range1 < 0 ) {
        range1 += list_size;
    }
    if (range2 < 0) {
        range2 += list_size;
    }

    if (range1 > range2) {
        buf = "empty list or set";
        return buf;
    }

    int i = 0;
    int buf_lines = 0;
    for (auto it = tokenizer.m_list_map[tokenizer.get_name()]->begin(); 
        it != tokenizer.m_list_map[tokenizer.get_name()]->end(); ++it) {
        if (i >= range1 && i <= range2) {
            ++buf_lines;
            buf += buf.itos(buf_lines);
            buf += ") \"";
            buf += it->data;
            buf += "\"";
            buf += '\n';
        }
        ++i;
    }

    if (buf.size() == 0) {
        buf = "ERR vvalue is not an integer, or out of range";
    } else {
        buf.pop_back();
    }
    return buf;
}

// lpop
String Lpop(Tokenizer& tokenizer) {
    String buf;
    String command_name = "lpop";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    
    if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
        buf = "(nil)";
        return buf;
    }
    
    buf += "\"";
    buf += tokenizer.m_list_map[tokenizer.get_name()]->pop_front();
    buf += "\"";
    return buf;
}

// rpop
String Rpop(Tokenizer& tokenizer) {
    String buf;
    String command_name = "rpop";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    
    if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
        buf = "(nil)";
        return buf;
    }
    
    buf += "\"";
    buf += tokenizer.m_list_map[tokenizer.get_name()]->pop_back();
    buf += "\"";
    return buf;
}

// llen
String Llen(Tokenizer& tokenizer) {
    String buf;
    String command_name = "llen";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    
    if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
        buf = "(integer) 0";
        return buf;
    }
    
    buf += "(integer) ";
    String list_size = list_size.itos(tokenizer.m_list_map[tokenizer.get_name()]->size());
    buf += list_size;
    return buf;
}

// lset
String Lset(Tokenizer& tokenizer) {
    String buf;
    String command_name = "lset";
    unsigned int args = 2;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    
    if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
        buf = "ERR,v no such key";
        return buf;
    }
    
    if (tokenizer.get_m_vec_args()[0].stoi() > tokenizer.m_list_map[tokenizer.get_name()]->size()) {
        buf = "ERR vindex out of range";
        return buf;
    }
    auto iter = tokenizer.m_list_map[tokenizer.get_name()]->begin();
    for (int i = 0; i < tokenizer.get_m_vec_args()[0].stoi(); ++i) {
        ++iter;
    }
    iter->data = tokenizer.get_m_vec_args()[1];

    buf = "OK";
    return buf;
}

// linsert
String Linsert(Tokenizer& tokenizer) {
    String buf;
    String command_name = "linsert";
    unsigned int args = 3;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    
    if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
        buf = "(integer) 0";
        return buf;
    }

    String bf_af = tokenizer.get_m_vec_args()[0];
    if (bf_af != "Before" || bf_af != "After") {
        buf = "ERR vsyntax errovr";
        return buf;
    }

    String value = tokenizer.get_m_vec_args()[1];
    String linsert_val = tokenizer.get_m_vec_args()[2]; 
    if (!tokenizer.m_list_map[tokenizer.get_name()]->exists(value)) {
        buf = "(integer) -1";
        return buf;
    }
    
    auto iter = tokenizer.m_list_map[tokenizer.get_name()]->begin();
    unsigned int position = 0;
    for (; iter != tokenizer.m_list_map[tokenizer.get_name()]->end(); ++iter) {
        if (iter->data == value) {
                if (bf_af == "Before") {
                    tokenizer.m_list_map[tokenizer.get_name()]->insert(position, linsert_val);
                } else if (bf_af == "After") {
                    tokenizer.m_list_map[tokenizer.get_name()]->insert(position + 1, linsert_val);
                }
        }
        ++position;
    }
    buf = buf.itos(tokenizer.m_list_map[tokenizer.get_name()]->size());
    return buf;
}

// lindex
String Lindex(Tokenizer& tokenizer) {
    String buf;
    String command_name = "lindex";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
        buf = "(nil)";
        return buf;
    }
    
    unsigned int index = tokenizer.get_m_vec_args()[0].stoi();
    if (index > tokenizer.m_list_map[tokenizer.get_name()]->size()) {
        buf = "(nil)";
        return buf;
    }

    buf += "\"";
    buf += tokenizer.m_list_map[tokenizer.get_name()]->find(index);
    buf += "\"";
    return buf;        
}

// lpushx
String Lpushx(Tokenizer& tokenizer) {
    String buf;
    String command_name = "lpushx";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
        buf = "(integer) 0";
        return buf;
    }

    for (int i = 0; i < tokenizer.get_m_vec_args().size(); ++i) {
        tokenizer.m_list_map[tokenizer.get_name()]->push_front(tokenizer.get_m_vec_args()[i]);        
    } 

    buf = "(integer) ";
    String v_size = v_size.itos(tokenizer.m_list_map[tokenizer.get_name()]->size());
    buf += v_size;  
    return buf;
}


// rpushx
String Rpushx(Tokenizer& tokenizer) {
    String buf;
    String command_name = "rpushx";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
        buf = "(integer) 0";
        return buf;
    }

    for (int i = 0; i < tokenizer.get_m_vec_args().size(); ++i) {
        tokenizer.m_list_map[tokenizer.get_name()]->push_back(tokenizer.get_m_vec_args()[i]);        
    } 

    buf = "(integer) ";
    String v_size = v_size.itos(tokenizer.m_list_map[tokenizer.get_name()]->size());
    buf += v_size;  
    return buf;
}

// lsort
String Lsort(Tokenizer& tokenizer) {
    String buf;
    String command_name = "lsort";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
        buf = "(integer) 0";
        return buf;
    }

    tokenizer.m_list_map[tokenizer.get_name()]->sort();
    int buf_lines = 0;
    for (auto it = tokenizer.m_list_map[tokenizer.get_name()]->begin(); 
        it != tokenizer.m_list_map[tokenizer.get_name()]->end(); ++it) {
        ++buf_lines;
        buf += buf.itos(buf_lines);
        buf += ") \"";
        buf += it->data;
        buf += "\"";
        buf += '\n';
    }
    if (buf.size() == 0) {
        buf = "empty list or set";
    } else {
        buf.pop_back();
    }
    return buf;
    }

// lrem
String Lrem(Tokenizer& tokenizer) {
    String buf;
    String command_name = "lrem";
    unsigned int args = 2;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    
    if (!tokenizer.m_list_map.exists_key(tokenizer.get_name())) {
        buf = "(integer) 0";
        return buf;
    }

    String arg1 = tokenizer.get_m_vec_args()[0];  
    String arg2 = tokenizer.get_m_vec_args()[1];
    unsigned int list_size = tokenizer.m_list_map[tokenizer.get_name()]->size();
    if (!is_int(arg1)) {
        buf = "ERR vvalue is not an integer, or out of range";
        return buf; 
    }  

    int count = arg1.stoi();
    if (!tokenizer.m_list_map[tokenizer.get_name()]->exists(arg2)) {
        buf = "(integer) 0";
        return buf;
    }

    int counter = 0;
    for (unsigned int i = 0; i < count; ++i) {
        if (!tokenizer.m_list_map[tokenizer.get_name()]->exists(arg2)) {
            break;
        }
        tokenizer.m_list_map[tokenizer.get_name()]->erase(arg2);
        ++counter; 
    }

    if (counter == 0) {
        buf = "(integer) 0";
    } else {
        String counter_str = counter_str.itos(counter);
        buf = "(integer) ";
        buf += counter_str; 
    }
    return buf;
}


// Hash Functions

// hset
String Hset(Tokenizer& tokenizer) {
    String buf;
    if (Utility::check(tokenizer, buf)) {
        if (tokenizer.check_name()) {
            buf = "Wrong number of arguments for hset (0, instead of 1+)";
            return buf;
        } else if (tokenizer.check_args()) {
            buf = "(errovr) ERR vwrong number of arguments for 'hset' command";
            return buf;
        } else {
            buf += "hset";
            return buf;
        }
    } else if (tokenizer.get_m_vec_args().size() % 2 != 0) {
            buf = "(errovr) ERR vwrong number of arguments for 'hset' command";
            return buf;
    }
    
    if (!tokenizer.m_hash_map.exists_key(tokenizer.get_name())) {
        Hash<String, String>* new_hash_str = new Hash<String, String>;
        tokenizer.m_hash_map[tokenizer.get_name()] = new_hash_str;
    }
        
    unsigned int set_pairs = 0;
    for (int i = 0; i < tokenizer.get_m_vec_args().size() - 1; i += 2) {  
        if (!tokenizer.m_hash_map[tokenizer.get_name()]->exists_key(tokenizer.get_m_vec_args()[i])) {
            tokenizer.m_hash_map[tokenizer.get_name()]->insert(tokenizer.get_m_vec_args()[i],tokenizer.get_m_vec_args()[i + 1]); 
            ++set_pairs;
        }  
    } 
    buf = "(integer) ";
    String v_size = v_size.itos(set_pairs);
    buf += v_size;  

    return buf;
}

// hkeys
String Hkeys(Tokenizer& tokenizer) {
    String buf;
    String command_name = "hkeys";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_hash_map.exists_key(tokenizer.get_name())) {
            buf = "empty list or set";
            return buf;
        }

        Vector<String> key = tokenizer.m_hash_map[tokenizer.get_name()]->get_keys();
        int buf_lines = 0;
        for (unsigned int i = 0; i < key.size(); ++i) {
            ++buf_lines;
            buf += buf.itos(buf_lines);
            buf += ") \"";
            buf += key[i];
            buf += "\"";
            buf += '\n';
        }
        if (buf.size() == 0) {
            buf = "empty list or set";
        } else {
            buf.pop_back();
        }
        return buf;
}

// hvals
String Hvals(Tokenizer& tokenizer) {
    String buf;
    String command_name = "hvals";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_hash_map.exists_key(tokenizer.get_name())) {
        buf = "empty list or set";
        return buf;
    }

    Vector<String> values = tokenizer.m_hash_map[tokenizer.get_name()]->get_values();
    int buf_lines = 0;
    for (unsigned int i = 0; i < values.size(); ++i) {
        ++buf_lines;
        buf += buf.itos(buf_lines);
        buf += ") \"";
        buf += values[i];
        buf += "\"";
        buf += '\n';
    }
    if (buf.size() == 0) {
        buf = "empty list or set";
    } else {
        buf.pop_back();
    }
    return buf;
}


// hgetall
String Hgetall(Tokenizer& tokenizer) {
    String buf;
    String command_name = "hgetall";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_hash_map.exists_key(tokenizer.get_name())) {
        buf = "empty list or set";
        return buf;
    }

    Vector<String> key = tokenizer.m_hash_map[tokenizer.get_name()]->get_keys();
    Vector<String> value = tokenizer.m_hash_map[tokenizer.get_name()]->get_values();
    int buf_lines = 0;
    for (unsigned int i = 0; i < key.size(); ++i) {
        ++buf_lines;
        buf += buf.itos(buf_lines);
        buf += ") \"";
        buf += key[i];
        buf += "\"";
        buf += " : \"";
        buf += value[i];
        buf += "\"";
        buf += '\n';
    }
    if (buf.size() == 0) {
        buf = "empty list or set";
    } else {
        buf.pop_back();
    }
    return buf;
}

// hexists
String Hexists(Tokenizer& tokenizer) {
    String buf;
    String command_name = "hexists";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_hash_map.exists_key(tokenizer.get_name())) {
        buf = "empty list or set";
        return buf;
    }

    String key = tokenizer.get_m_vec_args()[0];
    buf = "(integer) 0";
    if (tokenizer.m_hash_map[tokenizer.get_name()]->exists_key(key)) {
        buf = "(integer) 1";
    }
    return buf;
}

// hget
String Hget(Tokenizer& tokenizer) {
    String buf;
    String command_name = "hget";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_hash_map.exists_key(tokenizer.get_name())) {
        buf = "(nil)";
        return buf;
    }

    String key = tokenizer.get_m_vec_args()[0];
    String value;
    if (tokenizer.m_hash_map[tokenizer.get_name()]->exists_key(key)) {
        value = tokenizer.m_hash_map[tokenizer.get_name()]->get_value_by_key(key);
    } else {
        buf = "(nil)";
        return buf;
    }
    buf = "\"";
    buf += value;
    buf += "\"";
    return buf;
}

// hlen
String Hlen(Tokenizer& tokenizer) {
    String buf;
    String command_name = "hlen";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_hash_map.exists_key(tokenizer.get_name())) {
        buf = "0";
        return buf;
    }

    if (tokenizer.m_hash_map[tokenizer.get_name()]->get_keys().size() == 0) {
        buf = "0";
    } else {
        String key_size = key_size.itos(tokenizer.m_hash_map[tokenizer.get_name()]->get_keys().size());
        buf += key_size;
    }
    return buf;
}

// hstrlen
String Hstrlen(Tokenizer& tokenizer) {
    String buf;
    String command_name = "hstrlen";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_hash_map.exists_key(tokenizer.get_name())) {
        buf = "(nil)";
        return buf;
    }

    String key = tokenizer.get_m_vec_args()[0];
    String value;
    if (tokenizer.m_hash_map[tokenizer.get_name()]->exists_key(key)) {
        value = tokenizer.m_hash_map[tokenizer.get_name()]->get_value_by_key(key);
    } else {
        buf = "(nil)";
        return buf;
    }
    String val_size = val_size.itos(value.size());
    buf = "\"";
    buf += val_size;
    buf += "\"";
    return buf;
}

// hdel
String Hdel(Tokenizer& tokenizer) {
    String buf; 
    if (Utility::check(tokenizer, buf)) {
        if (tokenizer.check_name()) {
            buf = "Wrong number of arguments for hdel (0, instead of 1+)";
            return buf;
        } else if (tokenizer.check_args()) {
            buf = "ERR vwrong number of arguments for hdel command";
            return buf;
        } else {
            buf += "hdel";
            return buf;
        }
    } else if (tokenizer.get_m_vec_args().size() != 0) {
        buf.clear();
    }
    
    if (!tokenizer.m_hash_map.exists_key(tokenizer.get_name())) {
        buf = "(integer) 0";
        return buf;
    }

    unsigned int removed = 0;
    for (int i = 0; i < tokenizer.get_m_vec_args().size(); ++i) {
            if (tokenizer.m_hash_map[tokenizer.get_name()]->exists_key(tokenizer.get_m_vec_args()[i])) {
                tokenizer.m_hash_map[tokenizer.get_name()]->remove(tokenizer.get_m_vec_args()[i]); 
                ++removed;
            }  
    } 

    if (removed == 0) {
        buf = "(integer) 0";
    } else {
        String count_rem = count_rem.itos(removed);
        buf = "(integer) ";
        buf += count_rem;
    }
    return buf;
}


// Sets

// sadd
String Sadd(Tokenizer& tokenizer) {
    String buf; 
    if (Utility::check(tokenizer, buf)) {
        if (tokenizer.check_name()) {
            buf = "ERR vwrong number of arguments for sadd command";
            return buf;
        } else if (tokenizer.check_args()) {
            buf = "ERR vwrong number of arguments for sadd command";
            return buf;
        } else {
            buf += "sadd";
            return buf;
        }
    } else if (tokenizer.get_m_vec_args().size() != 0) {
        buf.clear();
    }
    
    int i = 0;
    unsigned int added = 0;
    if (!tokenizer.m_set_map.exists_key(tokenizer.get_name())) {
            Set<String>* new_set_str = new Set<String>(tokenizer.get_m_vec_args()[0]);
            tokenizer.m_set_map[tokenizer.get_name()] = new_set_str;
            ++i;
            ++added;
    }

    for (; i < tokenizer.get_m_vec_args().size(); ++i) {
        if (!tokenizer.m_set_map[tokenizer.get_name()]->exists(tokenizer.get_m_vec_args()[i])) {
            tokenizer.m_set_map[tokenizer.get_name()]->insert(tokenizer.get_m_vec_args()[i]);
            ++added;
        } 
    } 
    
    if (added == 0) {
        buf = "(integer) 0";
    } else {
        String count_rem = count_rem.itos(added);
        buf = "(integer) ";
        buf += count_rem;
    }
    return buf;
}

// smembers
String Smembers(Tokenizer& tokenizer) {
    String buf;
    String command_name = "smembers";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }    

    if (!tokenizer.m_set_map.exists_key(tokenizer.get_name())) {
        buf = "empty list or set";
        return buf;
    }

    Vector<String> values = tokenizer.m_set_map[tokenizer.get_name()]->vector_init();
    int buf_lines = 0;
    for (unsigned int i = 0; i < values.size(); ++i) {
        ++buf_lines;
        buf += buf.itos(buf_lines);
        buf += ") \"";
        buf += values[i];
        buf += "\"";
        buf += '\n';
    }
    if (buf.size() == 0) {
        buf = "empty list or set";
    } else {
        buf.pop_back();
    }
    return buf;
}

// scard
String Scard(Tokenizer& tokenizer) {
    String buf;
    String command_name = "scard";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_set_map.exists_key(tokenizer.get_name())) {
        buf = "0";
        return buf;
    }
    unsigned int Set_size = tokenizer.m_set_map[tokenizer.get_name()]->size();
    if (Set_size == 0) {
        buf = "0";
    } else {
        String key_size = key_size.itos(Set_size);          
        buf += key_size;
    }
    return buf;
}

// sismember
String Sismember(Tokenizer& tokenizer) {
    String buf;
    String command_name = "sismember";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (tokenizer.m_set_map.exists_key(tokenizer.get_name()) == false) {
        buf = "(integer) 0";
        return buf;
    } 

    if (tokenizer.m_set_map[tokenizer.get_name()]->exists(tokenizer.get_m_vec_args()[0]) == true) {            
        buf = "(integer) 1";
        return buf;
    } 
    buf = "(integer) 0";
    return buf;
}

// sdiff
String Sdiff(Tokenizer& tokenizer) {
    String buf;
    String command_name = "sdiff";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (tokenizer.m_set_map.exists_key(tokenizer.get_name()) == false) {
        buf = "empty list or set";
        return buf;
    } 
        
    Vector<String> val_vec = tokenizer.m_set_map[tokenizer.get_name()]->vector_init();
    Vector<String> val_vec2;
    if (tokenizer.m_set_map.exists_key(tokenizer.get_m_vec_args()[0])) {
        val_vec2 = tokenizer.m_set_map[tokenizer.get_m_vec_args()[0]]->vector_init();
    }

    Vector<String> sdiff = difference(val_vec, val_vec2);
    
    int buf_lines = 0;
    for (unsigned int i = 0; i < sdiff.size(); ++i) {
        ++buf_lines;
        buf += buf.itos(buf_lines);
        buf += ") \"";
        buf += sdiff[i];
        buf += "\"";
        buf += '\n';
    }
    if (buf.size() == 0) {
        buf = "empty list or set";
    } else {
        buf.pop_back();
    }
    return buf;
}

    // sinter
String Sinter(Tokenizer& tokenizer) {
    String buf;
    String command_name = "sinter";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (tokenizer.m_set_map.exists_key(tokenizer.get_name()) == false) {
        buf = "empty list or set";
        return buf;
    } 
        
    Vector<String> val_vec = tokenizer.m_set_map[tokenizer.get_name()]->vector_init();
    Vector<String> val_vec2;
    if (tokenizer.m_set_map.exists_key(tokenizer.get_m_vec_args()[0])) {
        val_vec2 = tokenizer.m_set_map[tokenizer.get_m_vec_args()[0]]->vector_init();
    }

    Vector<String> sinter = intersection(val_vec, val_vec2);
    
    int buf_lines = 0;
    for (unsigned int i = 0; i < sinter.size(); ++i) {
        ++buf_lines;
        buf += buf.itos(buf_lines);
        buf += ") \"";
        buf += sinter[i];
        buf += "\"";
        buf += '\n';
    }
    if (buf.size() == 0) {
        buf = "empty list or set";
    } else {
        buf.pop_back();
    }
    return buf;
}

    // sunion
String Sunion(Tokenizer& tokenizer) {
    String buf;
    String command_name = "sunion";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (tokenizer.m_set_map.exists_key(tokenizer.get_name()) == false) {
        buf = "empty list or set";
        return buf;
    } 
        
    Vector<String> val_vec = tokenizer.m_set_map[tokenizer.get_name()]->vector_init();
    Vector<String> val_vec2;
    if (tokenizer.m_set_map.exists_key(tokenizer.get_m_vec_args()[0])) {
        val_vec2 = tokenizer.m_set_map[tokenizer.get_m_vec_args()[0]]->vector_init();
    }

    Vector<String> sunion = merge(val_vec, val_vec2);
    
    int buf_lines = 0;
    for (unsigned int i = 0; i < sunion.size(); ++i) {
        ++buf_lines;
        buf += buf.itos(buf_lines);
        buf += ") \"";
        buf += sunion[i];
        buf += "\"";
        buf += '\n';
    }
    if (buf.size() == 0) {
        buf = "empty list or set";
    } else {
        buf.pop_back();
    }
    return buf;
}

// spop
String Spop(Tokenizer& tokenizer) {
    String buf;
    String command_name = "spop";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_set_map.exists_key(tokenizer.get_name())) {
        buf = "(nil)";
        return buf;
    }

    unsigned int Set_size = tokenizer.m_set_map[tokenizer.get_name()]->size();
    if (Set_size == 0) {
        buf = "(nil)";
    } else {
        String popped = tokenizer.m_set_map[tokenizer.get_name()]->pop();
        buf += "\"";
        buf += popped;
        buf += "\"";
    }
    return buf;
}    

// Strings

// set
String S_Set(Tokenizer& tokenizer) {
    String buf;
    String command_name = "set";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    
    if (!tokenizer.m_string_map.exists_key(tokenizer.get_name())) {

        String* new_str = new String;
        tokenizer.m_string_map[tokenizer.get_name()] = new_str;
    }
    *(tokenizer.m_string_map[tokenizer.get_name()]) = tokenizer.get_m_vec_args()[0];
    buf = "OK";
    return buf;
}

// get
String Get(Tokenizer& tokenizer) {
    String buf;
    String command_name = "get";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_string_map.exists_key(tokenizer.get_name())) {

        buf = "(nil)";
        return buf;
    }

    String getter = *(tokenizer.m_string_map[tokenizer.get_name()]);  
    buf = "\"";
    buf += getter;
    buf += "\"";
    return buf;
}

// getrange
String Getrange(Tokenizer& tokenizer) {
    String buf;
    String command_name = "getrange";
    unsigned int args = 2;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    
    if (!tokenizer.m_string_map.exists_key(tokenizer.get_name())) {
        buf = "\"\"";
        return buf;
    }

    String range_str = *(tokenizer.m_string_map[tokenizer.get_name()]);
    unsigned int string_size = tokenizer.m_string_map[tokenizer.get_name()]->size();

    String arg1 = tokenizer.get_m_vec_args()[0];
    String arg2 = tokenizer.get_m_vec_args()[1];
    unsigned int list_size = tokenizer.m_string_map[tokenizer.get_name()]->size();
    if (!(is_int(arg1) && is_int(arg2))) {
        buf = "ERR vvalue is not an integer, or out of range";
        return buf; 
    }

    int range1 = arg1.stoi();
    int range2 = arg2.stoi();

    if (range1 < 0 ) {
        range1 += string_size;
    }
    if (range2 < 0) {
        range2 += string_size;
    }

    if (range1 > range2) {
        buf = "empty list or set";
        return buf;
    }

    int buf_lines = 0;
    for (unsigned int i = 0; i < range_str.size(); ++i) {
        if (i >= range1 && i <= range2) {
            ++buf_lines;
            buf += buf.itos(buf_lines);
            buf += ") \"";
            buf += range_str[i];
            buf += "\"";
            buf += '\n';
        }
    }
    if (buf.size() == 0) {
        buf = "ERR vvalue is not an integer, or out of range";
    } else {
        buf.pop_back();
    }
    return buf;
}

// exists
String Exists(Tokenizer& tokenizer) {
    String buf;
    String command_name = "exists";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    if (!tokenizer.m_string_map.exists_key(tokenizer.get_name())) {
        buf = "(integer) 0";
    } else {
        buf = "(integer) 1";
    }
    return buf;
}

// incr
String Incr(Tokenizer& tokenizer) {
    String buf;
    String command_name = "incr";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    if (!tokenizer.m_string_map.exists_key(tokenizer.get_name())) {
        String* new_str = new String("0");
        tokenizer.m_string_map[tokenizer.get_name()] = new_str;
    }

    String tmp = *(tokenizer.m_string_map[tokenizer.get_name()]);
    int incr{};
    if (is_int(tmp)) {
        incr = tmp.stoi();

        if (incr == INT32_MAX) {
            buf = "Incrementable value is near supported capacity";
            buf += "\nPositive max value supported is 2,147,483,647.\nIncrement accordingly.";
            *(tokenizer.m_string_map[tokenizer.get_name()]) = tmp.itos(incr); 
            return buf;
        } 
        ++incr;

        *(tokenizer.m_string_map[tokenizer.get_name()]) = tmp.itos(incr);
        buf = "(integer) ";
        buf += *(tokenizer.m_string_map[tokenizer.get_name()]);
    } else {
        buf = "ERR vvalue is not an integer or out of range\n";
    }
    if (incr == INT32_MIN) {
        buf += "\nIncrementable value is near supported capacity.";
        buf += "\nNegative min value supported is -2,147,483,646.\nIncrement accordingly.";
        return buf;
    }
    return buf;
}

// incrby
String Incrby(Tokenizer& tokenizer) {
    String buf;
    String command_name = "incrby";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    if (!tokenizer.m_string_map.exists_key(tokenizer.get_name())) {
        String* new_str = new String("0");
        tokenizer.m_string_map[tokenizer.get_name()] = new_str;
    }

    String tmp = *(tokenizer.m_string_map[tokenizer.get_name()]);
    String tmp2 = tokenizer.get_m_vec_args()[0];
    int incr{};
    int incrby{};
    if (is_int(tmp) && is_int(tmp2)) {
        incr = tmp.stoi();
        incrby = tmp2.stoi();

        if (incr == INT32_MAX) {
            buf = "Incrementable value is near supported capacity";
            buf += "\nPositive max value supported is 2,147,483,647.\nIncrement accordingly.";
            *(tokenizer.m_string_map[tokenizer.get_name()]) = tmp.itos(incr); 
            return buf;
        } 
        if (incrby == INT32_MAX) {
            buf = "Positive max value supported is 2,147,483,647.\nIncrement accordingly.";
            return buf;
        } 

        incr += incrby;
        *(tokenizer.m_string_map[tokenizer.get_name()]) = tmp.itos(incr);
        buf = "(integer) ";
        buf += *(tokenizer.m_string_map[tokenizer.get_name()]);
    } else {
        buf = "ERR vvalue is not an integer or out of range\n";
        return buf;
    }

    if(incrby == INT32_MIN) {
        buf += "\nNegative max value is to increment by is -2,147,483,646\nIncrement accordingly.";
    }
    return buf;
}


// decr
String Decr(Tokenizer& tokenizer) {
    String buf;
    String command_name = "decr";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    if (!tokenizer.m_string_map.exists_key(tokenizer.get_name())) {
        String* new_str = new String("0");
        tokenizer.m_string_map[tokenizer.get_name()] = new_str;
    }

    String tmp = *(tokenizer.m_string_map[tokenizer.get_name()]);
    int decr{};
    if (is_int(tmp)) {
        decr = tmp.stoi();

        if (decr == INT32_MAX) {
            buf = "\nDecrementable value is near supported capacity";
            buf += "\nPositive max value supported is 2,147,483,647.\nDecrement accordingly.";
            *(tokenizer.m_string_map[tokenizer.get_name()]) = tmp.itos(decr);
            return buf;
        } 
        ++decr;

        *(tokenizer.m_string_map[tokenizer.get_name()]) = tmp.itos(decr);
        buf = "(integer) ";
        buf += *(tokenizer.m_string_map[tokenizer.get_name()]);
    } else {
        buf = "ERR vvalue is not an integer or out of range\n";
    }
    if (decr == INT32_MIN) {
        buf += "\nDecrementable value is near supported capacity.";
        buf += "\nNegative min value supported is -2,147,483,646.\nDecrement accordingly.";
        return buf;
    }
    return buf;
}

// decrby
String Decrby(Tokenizer& tokenizer) {
    String buf;
    String command_name = "decrby";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    if (!tokenizer.m_string_map.exists_key(tokenizer.get_name())) {
        String* new_str = new String("0");
        tokenizer.m_string_map[tokenizer.get_name()] = new_str;
    }

    String tmp = *(tokenizer.m_string_map[tokenizer.get_name()]);
    String tmp2 = tokenizer.get_m_vec_args()[0];
    int decr{};
    int decrby{};
    if (is_int(tmp) && is_int(tmp2)) {
        decr = tmp.stoi();
        decrby = tmp2.stoi();

        if (decr == INT32_MAX) {
            buf = "\nDecrementable value is near supported capacity";
            buf += "\nPositive max value supported is 2,147,483,647.\nDecrement accordingly.";
            *(tokenizer.m_string_map[tokenizer.get_name()]) = tmp.itos(decr);
            return buf;
        } 

        if (decrby == INT32_MIN) {
            buf = "\nNegative max value is to decrement by is -2,147,483,646\nDecrement accordingly.";
            return buf;
        } 

        decr -= decrby;
        *(tokenizer.m_string_map[tokenizer.get_name()]) = tmp.itos(decr);
        buf = "(integer) ";
        buf += *(tokenizer.m_string_map[tokenizer.get_name()]);
    } else {
        buf = "ERR vvalue is not an integer or out of range\n";
        return buf;
    }

    if(decrby == INT32_MAX) {
        buf += "\nPositive max value supported is 2,147,483,647.\nDecrement accordingly.";
    }
    return buf;
}

// append
String Append(Tokenizer& tokenizer) {
    String buf;
    String command_name = "append";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    if (!tokenizer.m_string_map.exists_key(tokenizer.get_name())) {
        String* new_str = new String;
        tokenizer.m_string_map[tokenizer.get_name()] = new_str;
    }

    String tmp = *(tokenizer.m_string_map[tokenizer.get_name()]);
    String append_str = tokenizer.get_m_vec_args()[0];
    for (int i = 0; i < append_str.size(); ++i) {
        tmp.push_back(append_str[i]);
    }
    *(tokenizer.m_string_map[tokenizer.get_name()]) = tmp;
    buf = buf.itos(append_str.size());
    return buf;
}

// del
String Del(Tokenizer& tokenizer) { 
    String buf; 
    if (Utility::check(tokenizer, buf)) {
        if (tokenizer.check_name()) {
            buf = "Wrong number of arguments for del (0, instead of 1+)\n";
            return buf;
        } else if (tokenizer.check_args()) {
            buf.clear();
        } else {
            buf += "del\n";
            return buf;
        }
    } else if (tokenizer.get_m_vec_args().size() != 0) {
        buf.clear();
    }
    
    unsigned int removed = 0;
    if (tokenizer.m_string_map.exists_key(tokenizer.get_name())) {
        tokenizer.m_string_map.remove(tokenizer.get_name());
        ++removed;
    }

    for (int i = 0; i < tokenizer.get_m_vec_args().size(); ++i) {
            if (tokenizer.m_string_map.exists_key(tokenizer.get_m_vec_args()[i])) {
                tokenizer.m_string_map.remove(tokenizer.get_m_vec_args()[i]);
                ++removed;
            }  
    } 

    if (removed == 0) {
        buf = "(integer) 0";
    } else {
        String count_rem = count_rem.itos(removed);
        buf = "(integer) ";
        buf += count_rem;
    }
    return buf;
}

// MinHeap

// zadd
String Zadd(Tokenizer& tokenizer) {
    String buf;
    String command_name = "zadd";
    unsigned int args = 2;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }

    if (!tokenizer.m_heap_map.exists_key(tokenizer.get_name())) {
        MinHeap<String>* new_heap = new MinHeap<String>;
        tokenizer.m_heap_map[tokenizer.get_name()] = new_heap;
    }

    if (!is_int(tokenizer.get_m_vec_args()[0])) {
        buf = "ERR value is not an integer, or out of range";
        return buf;
    }
    
    int key = tokenizer.get_m_vec_args()[0].stoi();
    String value = tokenizer.get_m_vec_args()[1];
    tokenizer.m_heap_map[tokenizer.get_name()]->insert_key(key, value);

    buf = "(integer) 1";
    return buf;
}

// zrange
String Zrange(Tokenizer& tokenizer) {
    String buf;
    String command_name = "zrange";
    unsigned int args = 2;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    if (!tokenizer.m_heap_map.exists_key(tokenizer.get_name())) {
        buf = "empty list or set";
        return buf;
    }

    Vector<String> range_heap = tokenizer.m_heap_map[tokenizer.get_name()]->get_all_values_by_priority();
    unsigned int heap_size = tokenizer.m_heap_map[tokenizer.get_name()]->size();

    String arg1 = tokenizer.get_m_vec_args()[0];
    String arg2 = tokenizer.get_m_vec_args()[1];
    if (!(is_int(arg1) && is_int(arg2))) {
        buf = "ERR value is not an integer, or out of range";
        return buf; 
    }

    int range1 = arg1.stoi();
    int range2 = arg2.stoi();

    if (range1 < 0 ) {
        range1 += heap_size;
    }
    if (range2 < 0) {
        range2 += heap_size;
    }

    if (range1 > range2) {
        buf = "empty list or set";
        return buf;
    }

    int buf_lines = 0;
    for (unsigned int i = 0; i < range_heap.size(); ++i) {
        if (i >= range1 && i <= range2) {
            ++buf_lines;
            buf += buf.itos(buf_lines);
            buf += ") \"";
            buf += range_heap[i];
            buf += "\"";
            buf += '\n';
        }
    }
    if (buf.size() == 0) {
        buf = "ERR value is not an integer, or out of range";
    } else {
        buf.pop_back();
    }
    return buf;
}

// zcard
String Zcard(Tokenizer& tokenizer) {
    String buf;
    String command_name = "zcard";
    unsigned int args = 0;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    if (!tokenizer.m_heap_map.exists_key(tokenizer.get_name())) {
        buf = "(integer) 0";
        return buf;
    }

    if (tokenizer.m_heap_map[tokenizer.get_name()]->size() == 0) {
        buf = "(integer) 0";
    } else {
        buf = "(integer) ";
        buf += buf.itos(tokenizer.m_heap_map[tokenizer.get_name()]->size());
    }
    return buf;
}

// zrem
String Zrem(Tokenizer& tokenizer) {
    String buf; 
    if (Utility::check(tokenizer, buf)) {
        if (tokenizer.check_name()) {
            buf = "Wrong number of arguments for zrem (0, instead of 1+)";
            return buf;
        } else if (tokenizer.check_args()) {
            buf = "ERR wrong number of arguments for zrem command";
            return buf;
        } else {
            buf += "zrem";
            return buf;
        }
    } else if (tokenizer.get_m_vec_args().size() != 0) {
        buf.clear();
    }
    
    if (!tokenizer.m_heap_map.exists_key(tokenizer.get_name())) {
        buf = "(integer) 0";
        return buf;
    }

    unsigned int removed = 0;
    for (int i = 0; i < tokenizer.get_m_vec_args().size(); ++i) {
            if (tokenizer.m_heap_map[tokenizer.get_name()]->value_exists(tokenizer.get_m_vec_args()[i])) {
                tokenizer.m_heap_map[tokenizer.get_name()]->delete_value(tokenizer.get_m_vec_args()[i]); 
                ++removed;
            }  
    } 

    if (removed == 0) {
        buf = "(integer) 0";
    } else {
        String count_rem = count_rem.itos(removed);
        buf = "(integer) ";
        buf += count_rem;
    }
    return buf;
}

// zscore 
String Zscore(Tokenizer& tokenizer) {
    String buf;
    String command_name = "zscore";
    unsigned int args = 1;
    if (check_args(tokenizer, buf, command_name, args)) {
        return buf;
    }
    if (!tokenizer.m_heap_map.exists_key(tokenizer.get_name())) {
        buf = "(nil)";
        return buf;
    }
    
    if (tokenizer.m_heap_map[tokenizer.get_name()]->value_exists(tokenizer.get_m_vec_args()[0])) {
        buf = buf.itos(tokenizer.m_heap_map[tokenizer.get_name()]->get_key_by_value(tokenizer.get_m_vec_args()[0]));
    } else {
        buf = "(nil)";
    }
    return buf;
}

} // Utility

#endif