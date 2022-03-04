#ifndef SET_H_
#define SET_H_

#include <iostream>

#include "vector.h"
#include "queue.h"
#include "string.h"

using namespace DSvector;
using namespace DSqueue;
using namespace DSstring;

namespace DSset {
    
template <typename T>
class Set {
public: 
    // Member functions
    Set(T key) {
        root = new Node;
        root->m_key = key;
        root->left = root->right = nullptr;
        m_size = 1;
    }

    Set(const Set<T>&) = delete;
    Set(Set&&) = delete;
    Set& operator=(const Set&) = delete;
    Set& operator=(Set&&) = delete;

    ~Set() {
        cleaner(root);
    }

    // Modifiers
    void insert(T key) {
        root = inserter(this->root, key);
        ++m_size;
    }

    void remove(T key) {
        if (root != nullptr) {
            root = deleter(root, key);
            --m_size;
        }
    }
    
    // Non-modifiers
    void print() const {
        inorder_printer(root);
    }

    T pop() {
        Node* curr = min_value(root);
        T tmp = T();
        if (m_size != 0) {
            tmp = curr->m_key;
            remove(curr->m_key);
        }
        return tmp;
    }

    // void printos() {
    //     level_order_printer(root);
    // }

    int get_height() {
        return height(root);
    }

    bool exists(T key) const {
        return search(root, key);
    }

    unsigned int size() {
        return m_size; 
    }

    T operator[](T key) {
        return operator_helper(root, key);
    }

    T get_nodes() {
        string_creator(root);
        return all_nodes;
    }

    T top() const {
        if (root != nullptr) {
            return root->m_key;
        } else {
            return T(); // ????
        }
    }

    Vector<T> vector_init() { 
        nodes_vector.clear();
        vector_creator(root);
        return nodes_vector;
    }

private:
    struct Node {
        T m_key;
        Node* left;
        Node* right;
    };

    Node* root;
    unsigned int m_size{};
    T all_nodes{};
    Vector<T> nodes_vector;

    Node* right_rotate(Node* y) { // y is the parent
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;
        
        return x;
    }

    Node* left_rotate(Node* y) { // y is the parent
        Node* x = y->right;
        Node* T2 = x->left;

        x->left = y;
        y->right = T2;

        return x;
    }

    int get_balance_factor(Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        return (height(node->left) - height(node->right));
    }

    int height(Node* node) const {
        if (node == nullptr) {
            return -1;
        } else {
            int lheight = height(node->left);
            int rheight = height(node->right);

            if (lheight > rheight) {
                return lheight + 1;
            } else {
                return rheight + 1;
            }
        }
    }

    Node* inserter(Node* node, T key) {
        if (node == nullptr) {
            Node* tmp = new Node;
            tmp->m_key = key;
            tmp->right = tmp->left = nullptr;
            return tmp;
        }
        if (key < node->m_key) {
            node->left = inserter(node->left, key);
        } else if (key > node->m_key) {
            node->right = inserter(node->right, key);
        } else { 
            return node; 
        }

        int bf = get_balance_factor(node);

        if (bf > 1 && key < node->left->m_key) {
            return right_rotate(node);
        }
        if (bf < -1 && key > node->right->m_key) {
            return left_rotate(node);
        } 
        if (bf > 1 && key > node->left->m_key) {
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }
        if (bf < -1 && key < node->right->m_key) {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }
        return node;
    }

    Node* deleter(Node* node, T key) {
        if (node == nullptr) { 
            return node; 
        } else if (key < node->m_key) {
            node->left = deleter(node->left, key);
        } else if (key > node->m_key) {
            node->right = deleter(node->right, key);
        } else {
            if (node->left == nullptr) {
                Node* tmp = node->right;
                delete node;
                node = nullptr;
                return tmp;
            }
            else if (node->right == nullptr) {
                Node* tmp = node->left;
                delete node;
                node = nullptr;
                return tmp;
            } 
            else {
                Node* tmp = min_value(node->right);
                node->m_key = tmp->m_key;
                node->right = deleter(node->right, tmp->m_key);
            }
        }
        if (node == nullptr) { 
            return node; 
        }
        
        int bf = get_balance_factor(node);

        if (bf > 1 && get_balance_factor(node->left) >= 0) {
            return right_rotate(node);
        } 
        if (bf > 1 && get_balance_factor(node->left) < 0) {
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }
        if(bf < -1 && get_balance_factor(node->right) <= 0) {
            return left_rotate(node);
        }
        if (bf < -1 && get_balance_factor(node->right) > 0) {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }

        return node;
    }

    void inorder_printer(Node* node) const {
        if (node != nullptr) {
            inorder_printer(node->left);
            std::cout << node->m_key << " ";
            inorder_printer(node->right);
        }
    }

    // void level_order_printer(Node* node) {
    //     if (!node) { return; }
    //     std::queue<Node*> QUEUE;
    //     QUEUE.push(node);
    //     while (!QUEUE.empty())
    //     {
    //         Node* curr = QUEUE.front();
    //         std::cout << curr->m_key << " ";
    //         if (curr->left) { QUEUE.push(curr->left); }
    //         if (curr->right) { QUEUE.push(curr->right); }
    //         QUEUE.pop();
    //     }    
    // }

    Node* min_value(Node* node) const {
        if (node == nullptr) {
            return node;
        }
        Node* curr = node;
        while (curr->left != nullptr) {
            curr = curr->left;
        }
        return curr;
    }

    bool search(Node* node, T key) const {
        if (node == nullptr) { 
            return false; 
        }
        if (node->m_key == key) { 
            return true; 
        } else {
            if (key < node->m_key) {
                return search(node->left, key);
            } else {
                return search(node->right, key);
            }
        }
    }

    T operator_helper(Node* node, T key) {
        if (node == nullptr) { return T(); } // ????
        if (node->m_key == key) { return key; }
        else {
            if (key < node->m_key) {
                return operator_helper(node->left, key);
            } else {
                return operator_helper(node->right, key);
            }
        }
    }

    void string_creator(Node* node) { 
        if (node != nullptr) {
            all_nodes += node->m_key;
            string_creator(node->left);
            string_creator(node->right);
        }
    }

    void vector_creator(Node* node) {
        if (node != nullptr) {
            vector_creator(node->left);
            nodes_vector.push_back(node->m_key);
            vector_creator(node->right);
        }
    }

    void cleaner(Node* node) {
        if (!node) { 
            return; 
        }
        Queue<Node*> QUEUE;
        QUEUE.push(node);
        while (!QUEUE.empty())
        {
            Node* curr = QUEUE.front();
            if (curr->left) { 
                QUEUE.push(curr->left); 
            }
            if (curr->right) { 
                QUEUE.push(curr->right); 
            }
            delete curr;
            curr = nullptr;
            QUEUE.pop();
        } 
    }
};

} // DSset

#endif