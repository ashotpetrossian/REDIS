#ifndef PARSER_H
#define PARSER_H

#include "utility.h"
#include "tokenizer.h"

using namespace Utility;
using namespace DSstring;
using namespace DSunordered_map;
using namespace DBTokenizer;

namespace DBParser {

class Parser
{
    using f_ptr = String (*)(Tokenizer& tokenizer);
    
public:
    // Member functions
    Parser() {
        init(); 
    }

    Parser(const Parser&) = default;
    Parser(Parser&&) = default;
    Parser& operator=(const Parser&) = default;
    Parser& operator=(Parser&&) = default;

    ~Parser() {
        tokenizer.m_list_map.clean();
        tokenizer.m_hash_map.clean();
        tokenizer.m_set_map.clean();
        tokenizer.m_string_map.clean();
        tokenizer.m_heap_map.clean();
    };

    void init() {
        // General Commands
        fptr_map["ping"] = Pong;
        fptr_map["keys*"] = Keys;
        fptr_map["flushall"] = Flushall;

        // Lists
        fptr_map["lpush"] = Lpush;
        fptr_map["rpush"] = Rpush;
        fptr_map["lrange"] = Lrange;
        fptr_map["lpop"] = Lpop;
        fptr_map["rpop"] = Rpop;
        fptr_map["llen"] = Llen; 
        fptr_map["lset"] = Lset;
        fptr_map["linsert"] = Linsert;
        fptr_map["lindex"] = Lindex;
        fptr_map["lpushx"] = Lpushx;
        fptr_map["rpushx"] = Rpushx;
        fptr_map["lsort"] = Lsort;
        fptr_map["lrem"] = Lrem;

        // Hashtables
        fptr_map["hset"] = Hset;
        fptr_map["hkeys"] = Hkeys;
        fptr_map["hvals"] = Hvals;
        fptr_map["hgetall"] = Hgetall;
        fptr_map["hexists"] = Hexists;
        fptr_map["hget"] = Hget;
        fptr_map["hlen"] = Hlen;
        fptr_map["hstrlen"] = Hstrlen;
        fptr_map["hdel"] = Hdel;

        // Sets
        fptr_map["sadd"] = Sadd;
        fptr_map["smembers"] = Smembers;
        fptr_map["scard"] = Scard;  
        fptr_map["sismember"] = Sismember;
        fptr_map["sdiff"] = Sdiff;
        fptr_map["sinter"] = Sinter;
        fptr_map["sunion"] = Sunion;
        fptr_map["spop"] = Spop;

        // String
        fptr_map["set"] = S_Set;
        fptr_map["get"] = Get;
        fptr_map["getrange"] = Getrange;
        fptr_map["exists"] = Exists;
        fptr_map["incr"] = Incr;
        fptr_map["incrby"] = Incrby;
        fptr_map["decr"] = Decr;
        fptr_map["decrby"] = Decrby;
        fptr_map["append"] = Append;
        fptr_map["del"] = Del;

        // MinHeap
        fptr_map["zadd"] = Zadd;
        fptr_map["zrange"] = Zrange;
        fptr_map["zcard"] = Zcard;
        fptr_map["zrem"] = Zrem;
        fptr_map["zscore"] = Zscore;
    }

    // Modifiers
    String get_response() {
        return response; 
    }
    void send_buffer(char* buffer) {
        tokenizer.set_buffer(buffer); 
    }

    void manager() {
        tokenizer.trim_buf_str();
        tokenizer.trim_args();

        if (fptr_map.exists_key(tokenizer.get_command()) == false) {
            response = "(error) Sorry, I don't recognize that command";
        } else {
            response = fptr_map[tokenizer.get_command()](tokenizer);            
        }
        tokenizer.clear_tokenizer();
    }

    void modify_reponse() { 
        String tmp;
        for(unsigned int i = 0; i < response.size(); ++i) {
            if(response[i] != '\0') {
                tmp.push_back(response[i]);
            }
        }
        response = tmp;
    }

    void modify_buffer(char* buffer) {
        modify_reponse(); 
        unsigned int i = 0;
        for(; i < response.size(); ++i) {
            buffer[i] = response[i];
        }
        response.clear();
    }

private:
    Tokenizer tokenizer;
    String response;
    Hash<String, f_ptr> fptr_map;
};

} // DBParser

#endif