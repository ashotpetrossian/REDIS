This project resembles the infamous REDIS and its in-memory data structure store,
which is used as a database and cache.
Our implementation of REDIS is written entirely in C++.
Program supports multithreading, and includes our implementation of the underlying
data structures and the parser.
Almost all commands related to databases and caching are supported.
The commands are stored in a Hashtable, where the second template parameters are
the corresponding function pointers. Function pointers are implemented in
Utility.h, and included in Tokenizer.h.
There are five main data structures, where the user information can be cached.
The Parser, which includes the Tokenizer object, controls the storing and
response actions.
Server class uses sockets and threads to support multiple requests. In every
request, the server creates a new parser, so for every client the parser is
unique.
The Client code is separate from the server class, and must be run on every
client's CPU.
The inspiration for such a project came from V. Grigoryan, Director of
Picsart Academy.
In the documentation can be found a brief description and guideline for the commands.
