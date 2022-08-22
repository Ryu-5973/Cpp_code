/*
 * @Author: Ryu-59073
 * @Date: 2022-07-25 08:00:29
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-25 08:54:33
 * @FilePath: /Cpp_code/data_struct/mylist.cc
 * @Description: 普通列表和侵入式列表
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

#define container_of(ptr, type, member) \
    ({const typeof( ((type *)0)->member ) *__mptr = (ptr);\
    (type *)( (char *)__mptr - offsetof(type,member) );})

#define offsetof(type, member) ((size_t) &((type *)0)->member)

template<typename T>
struct list{
    list(T data): m_data(data), 
        m_prev(this), m_next(this) {
    }
    T m_data;
    list* m_prev;
    list* m_next;
};

template<typename T>
struct intrusive_list{
    intrusive_list(T data = T()): node(new Node){
    }
    struct Node{
        Node(): m_prev(this), m_next(this){
        }
        Node* m_prev;
        Node* m_next;
    };
    T data;
    Node* node;
};

using namespace std;
int main(int argc, char const *argv[]) {
    intrusive_list<int> *head = new intrusive_list<int>;
    intrusive_list<int> *second = new intrusive_list<int>(1);
    second->node->m_next = head->node->m_next;
    second->node->m_prev = head->node;
    head->node->m_next->m_prev = second->node;
    head->node->m_next = second->node; 
    // std::cout << list_entry(head, intrusive_list<int>, data)) << std::endl;
    typeof( ((intrusive_list<int>*)0)->node) *__mptr = &(head->node);
    cout << (long)&head->node << endl;
    cout << (long)__mptr << endl;
    cout << (long)&(((intrusive_list<int> *)0)->node) << endl;
    cout << (long)((size_t) &((intrusive_list<int> *)0)->node) << endl;
    cout << (long)&head << endl;
    // (intrusive_list<int>*)( (char *)__mptr - ((size_t) &((intrusive_list<int> *)0)->node) );
    return 0;
}

