//
// Created by 张智超 on 2018/1/29.
//

#ifndef MINISTL_PRIORITY_QUEUE_H
#define MINISTL_PRIORITY_QUEUE_H
#include <vector>
#include <algorithm>
#include "miniSTL_heap_algorithm.h"

namespace zzc {//
    template <typename T, typename Sequence = std::vector<T>>
    class miniSTL_priority_queue {
    public:
        using value_type      = typename Sequence::value_type;
        using reference       = typename Sequence::reference;
        using const_reference = const reference;
    protected:
        Sequence c;
    public:
        miniSTL_priority_queue() = default;
        template <typename Iterator>
        miniSTL_priority_queue(Iterator first, Iterator last) : c(first, last) {
            zzc::make_heap(c.begin(), c.end());
        }

        bool empty() {
            return c.empty();
        }

        size_t size() {
            return c.size();
        }

        reference top() {
            return c.front();
        }

        void push(value_type const& x) {
            c.push_back(x);
            zzc::push_heap(c.begin(), c.end());
        }

        void pop() {
            zzc::pop_heap(c.begin(), c.end());
            c.pop_back();
        }
    };
}

#endif //MINISTL_PRIORITY_QUEUE_H
