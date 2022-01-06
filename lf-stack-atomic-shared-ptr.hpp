#pragma once
#include <atomic>
#include <memory>


template<typename T>
class lockFreeStack
{
    struct node;
    using TAtomicSharedPtr = std::atomic<std::shared_ptr<node>>;
private:
    struct node
    {
        std::shared_ptr<T> data;
        TAtomicSharedPtr next;
        node(T const& data_):
            data(std::make_shared<T>(data_))
        {}
    };
    TAtomicSharedPtr head;
public:
    void push(T const& data)
    {
        std::shared_ptr<node> const new_node=std::make_shared<node>(data);
        new_node->next=head.load();
        while(!head.compare_exchange_weak(new_node->next,new_node));
    }
    std::shared_ptr<T> pop()
    {
        std::shared_ptr<node> old_head=head.load();
        while(old_head && !head.compare_exchange_weak(
                  old_head,old_head->next.load()));
        if(old_head) {
            old_head->next=std::shared_ptr<node>();
            return old_head->data;
        }
        
        return std::shared_ptr<T>();
    }
    ~lockFreeStack(){
        while(pop());
    }
};