#pragma once
#include <atomic>

template <typename T>
class lockFreeStack
{
private:
    struct node
    {
        T data;
        node* next;
        node(T const& data_):
        data(data_)
        {}
    };
    std::atomic<node*> head;
public:
    lockFreeStack() {}
    void push(T const& data)
    {
        node* const new_node = new node(data);
        new_node->next = head.load();
        while(!head.compare_exchange_weak(new_node->next, new_node));
    }
    void pop(T& result)
    {
        node* old_head = head.load();
        while(!head.compare_exchange_weak(old_head, old_head->next));
        result=old_head->data;
    }
    
};