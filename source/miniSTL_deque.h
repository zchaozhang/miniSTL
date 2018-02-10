//
// Created by 张智超 on 2018/1/26.
//

#ifndef MINISTL_MINISTL_DEQUE_H
#define MINISTL_MINISTL_DEQUE_H

#include <mach/vm_types.h>
#include "miniSTL_alloc.h"
#include "miniSTL_iterator.h"
#include "miniSTL_construct.h"
#include <algorithm>



namespace zzc {//
    inline size_t __deque_buf_size(size_t n, size_t sz) {
        return n != 0 ? n : (sz < 512 ? static_cast<size_t>(512) / sz : static_cast<size_t>(1));
    }


    //deque迭代器
    template<typename T, typename Ref, typename Ptr, size_t BufSiz>
    struct __deque_iterator {
        using iterator          = __deque_iterator<T, T &, T *, BufSiz>;
        using const_iterator    = __deque_iterator<T, T const &, T const *, BufSiz>;
        using iterator_category = random_access_iterator_tag;
        using value_type           = T;
        using pointer         = T *;
        using reference       = T &;
        using difference      = ptrdiff_t;
        using map_pointer       = pointer *;
        using self              = __deque_iterator;

        static size_t buffer_size() { return __deque_buf_size(BufSiz, sizeof(std::decay_t<T>)); }

        //     现行元素  头     尾
        pointer cur, first, last;
        //         指向管控中心
        map_pointer node;


        void set_node(map_pointer new_node) {
            node = new_node;
            first = *new_node;
            last = first + static_cast<difference >(buffer_size());
        }

        reference operator*() const { return *cur; }

        pointer operator->() const { return &(*cur); }

        difference operator-(self const &x) const {
            return static_cast<difference>(buffer_size() * (node - x.node - 1) + cur - first + x.last -
                                             x.cur);
        }

        self &operator++() {
            ++cur;
            if (cur == last) {
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }


        self operator++(int) {
            self temp = *this;
            ++*this;
            return temp;
        }

        self &operator--() {
            if (cur == first) {
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }

        self operator--(int) {
            self temp = *this;
            --*this;
            return temp;
        }

        self& operator+=(difference n) {
            difference offset = n + (cur - first);
            //目标位置在同一缓冲区内
            if (offset >= 0 && offset < buffer_size()) {
                cur += n;
            } else {
                difference node_offset = offset > 0 ? offset / buffer_size() :
                        -static_cast<difference >(
                                (-offset - 1) / buffer_size()) - 1;
                set_node(node + node_offset);
                cur = first + (offset - node_offset * buffer_size());
            }
            return *this;
        }


        self operator+(difference n) const {
            self temp = *this;
            return temp += n;
        }

        self& operator-=(difference n)  {
            return *this += -n;
        }

        self operator-(difference n) const {
            self temp = *this;
            return temp -= n;
        }

        reference operator[](difference n) const {
            return *(*this + n);
        }

        bool operator== (self const& x) const { return cur == x.cur;}
        bool operator!= (self const& x) const { return cur != x.cur;}
        bool operator< (self const& x) const {
            return (node == x.node) ? (cur < x.cur) : node < x.node;
        }
    };











    template <typename T, typename Alloc = alloc, size_t BufSiz = 0>
    class deque {
    public:
        using value_type     = T;
        using pointer     = value_type *;
        using reference   = value_type &;
        using iterator    = __deque_iterator<T, T&, T*, BufSiz>;

    protected:
        using map_pointer   = pointer *;
        using data_allocator = simple_alloc<value_type, Alloc>;
        using map_allocator  = simple_alloc<pointer, Alloc>;

        pointer allocate_node() {
            return data_allocator::allocate(iterator::buffer_size());
        }

        void deallocate_node(pointer p) {
            data_allocator ::deallocate(p, iterator::buffer_size());
        }

        map_pointer map;    //指向map， map是连续空间，每个元素都指向了一个区域
        size_t map_size;    //map可容纳多少指针
        iterator start, finish;

        void create_map_and_nodes(size_t num);
        void fill_initialize(size_t n, value_type const& values);
        void reserve_map_at_back(size_t nodes_to_add = 1) {
            if (nodes_to_add + 1 > map_size - (finish.node - map))
                reallocate_map(nodes_to_add, false);
        }

        void reserve_map_at_front(size_t nodes_to_add = 1) {
            if (nodes_to_add > start.node - map)
                reallocate_map(nodes_to_add, true);
        }

        void reallocate_map(size_t add, bool add_at_front);

        void push_back_aux(value_type const& x) noexcept ;
        void push_front_aux(value_type const& x) noexcept ;
        void pop_front_aux() noexcept ;
        void pop_back_aux() noexcept ;

    public:
        deque(size_t n, value_type const& x) : start(), finish(), map(nullptr), map_size(0) {
            fill_initialize(n, x);
        }


        iterator begin() { return start; }
        iterator end() { return finish; }
        reference operator[](size_t n) {
            return start[static_cast<typename iterator::difference>(n)];
        }

        reference front() {
            return *start;
        }

        reference back() {
            iterator temp = finish;
            --temp;
            return *temp;
        }

        size_t size() { return finish - start;}
        bool empty() { return finish == start;}


        void push_back(value_type const& t) {
            if (finish.cur != finish.last) {
                construct(finish.cur, t);
                ++finish.cur;
            }
            else
                push_back_aux(t);
        }

        void push_front(value_type const& t) {
            if (start.cur != start.first) {
                construct(start.cur - 1, t);
                --start.cur;
            }
            else
                push_front_aux(t);
        }

        void pop_back() {
            if (finish.cur != finish.first) {
                --finish.cur;
                destroy(finish.cur);
            }
            else
                pop_back_aux();
        }

        void pop_front() {
            if (start.cur != start.last - 1) {
                destroy(start.cur);
                --start.cur;
            }
            else
                //第一缓冲区只有一个元素
                pop_front_aux();
        }
    };

    template <typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::create_map_and_nodes(size_t num) {
        //需要的节点数
        size_t num_nodes = num / iterator::buffer_size() + 1;
        //一个map管理几个节点，最少是8个，最多是节点数+2
        map_size = std::max(num_nodes + 2, static_cast<size_t>(8));
        map = map_allocator::allocate(map_size);

        //nstart, nfinish指向map最中央的的区域，有利于向两边扩散
        map_pointer nstart = map + (map_size - num_nodes) / 2;
        map_pointer nfinish = nstart + num_nodes - 1;

        map_pointer cur;

        try {
            for (cur = nstart; cur <= nfinish; ++cur) {
                *cur = allocate_node();
            }
        }
        catch (...) {
            //"commit or rollback"
        }

        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;
        finish.cur = finish.first + num % iterator::buffer_size();
    }

    template<typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::fill_initialize(size_t n, const T &values) {
        create_map_and_nodes(n);
        map_pointer cur;
        //为每个节点赋初值
        for (cur = start.node; cur < finish.node; ++cur)
            uninitialized_fill(*cur, *cur + iterator::buffer_size(), values);
        uninitialized_fill(finish.first, finish.cur, values);
    }

    template<typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::reallocate_map(size_t add, bool add_at_front) {
        size_t old_num_nodes = finish.node - start.node + 1;
        size_t new_num_nodes = old_num_nodes + add;

        map_pointer new_nstart;

        if (map_size > 2 * new_num_nodes) {
            new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? add : 0);
            if (new_nstart < start.node)
                std::copy(start.node, finish.node + 1, new_nstart);
            else
                std::copy_backward(start.node, finish.node, new_nstart + old_num_nodes);
        }
        else {
            size_t new_map_size = map_size + std::max(map_size, add) + 2;

            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? add : 0);

            std::copy(start.node, finish.node, new_nstart);
            map_allocator::deallocate(map, map_size);

            map = new_map;
            map_size = new_map_size;
        }

        start.set_node(new_nstart);
        finish.set_node(new_nstart + old_num_nodes - 1);
    }

    template<typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::push_back_aux(const T &t) noexcept {
        value_type t_copy = t;
        reserve_map_at_back();
        *(finish.node + 1) = allocate_node();
        construct(finish.cur, t_copy);
        finish.set_node(finish.node + 1);
        finish.cur = finish.first;
    }

    template<typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::push_front_aux(const T &t) noexcept {
        value_type t_copy = t;
        reserve_map_at_front();

        *(start.node - 1) = allocate_node();
        start.set_node(start.node - 1);
        start.cur = start.last - 1;
        construct(start.cur, t_copy);

    }

    template<typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::pop_front_aux() noexcept {
        destroy(start.first);//析构
        deallocate_node(start.first);//释放空间
        start.set_node(start.node + 1);
        start.cur = start.first;
    }

    template<typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::pop_back_aux() noexcept {
        deallocate_node(finish.first);//释放最后一个缓冲区
        finish.set_node(finish.node - 1);
        finish.cur = finish.last - 1;
        destroy(finish.cur);//析构
    }


}




#endif //MINISTL_MINISTL_DEQUE_H
