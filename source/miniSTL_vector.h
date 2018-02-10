//
// Created by 张智超 on 2018/1/23.
//

#ifndef MINISTL_MINISTL_VECTOR_H
#define MINISTL_MINISTL_VECTOR_H

#include "miniSTL_alloc.h"
#include "miniSTL_construct.h"
#include <cassert>
#include <algorithm>
namespace zzc {//


    template <typename T, typename Alloc = alloc>
    class vector {
    public:
        using value_type      = T;
        using pointer         = value_type*;
        using iterator        = value_type*;
        using reference       = value_type&;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
    protected:
        using data_allocator = simple_alloc<value_type, Alloc>;
        iterator start;              //表示目前使用空间的头
        iterator finish;             //表示目前使用空间的尾
        iterator end_of_storage;     //表示目前可用空间的尾

        void insert_aux(iterator position, T const& x);

        void deallocate() {
            if (start)
                data_allocator ::deallocate(start, end_of_storage - start);
        }

        iterator allocate_and_fill(size_t n, T const& x) {
            iterator res = data_allocator::allocate(n);
            uninitialized_fill_n(res, n, x);
            return res;
        }

        void fill_initialize(size_t n, T const& value) {
            start = allocate_and_fill(n, value);
            finish = start + n;
            end_of_storage = finish;
        }

    private:


        void fill_initiakuze(size_t n, T const& value) {
            start = allocate_and_fill(n, value);
            finish = start + n;
            end_of_storage = finish;
        }

    public:
        vector(size_t n, T const& value) {
            fill_initialize(n, value);
        }
        vector(): start(nullptr), finish(nullptr), end_of_storage(nullptr){};
        explicit vector(size_t n) {fill_initialize(n, T());}
        ~vector(){destroy(start, finish); deallocate();}

        const iterator begin() const {return start;}
        const iterator end() const { return finish;}
        size_t size() const { return end() - begin();}
        size_t capacity() {return end_of_storage - begin();}
        bool empty() const { return (begin() == end());}
        reference operator[] (size_t n) { return *(begin() + n);}

        reference front() { return *begin();}
        reference back() { return *(end()- 1);}

        void push_back(T const& x) {
            if (finish != end_of_storage) {
                construct(finish, x);
                ++finish;
            }
            else {
                insert_aux(end(), x);
            }

        }

        void pop_back() {
            assert(finish != start);
            --finish;
            destroy(finish);
        }

        //删除单个元素
        iterator erase(iterator position) {
            if (position + 1 != end())
                std::copy(position + 1, finish, position);
            --finish;
            destroy(finish);
            return position;
        }

        //删除[first, last)区间的所有元素
        iterator erase(iterator first, iterator last) {
            iterator i = std::copy(last, finish, first);
            destroy(i, finish);
            finish = finish - (last - first);
            return first;
        }


        void insert(iterator postion, size_t n, T const& x);

        void insert(iterator postion, T const& x);

        void resize(size_t new_size, T const& x) {
            if (new_size < size())
                erase(begin() + new_size, end());
            else
                insert(end(), new_size - size(), x);
        }

        void clear() {erase(begin(), end());}
    };

    template <typename T, typename Alloc>
    void vector<T, Alloc>::insert_aux(iterator position, T const &x) {
        if (finish != end_of_storage) {
            construct(finish, x);
            ++finish;
            T x_copy{x};
            std::copy_backward(position, finish - 2, finish - 1);
            *position = x_copy;
        }
        else {
            const size_t old_size = size();
            const size_t len = old_size != 0 ? 2 * old_size : 1;
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try {
                new_finish = uninitialized_copy(start, position, new_start);
                construct(new_finish, x);
                ++new_finish;
                //insert调用时，将后面的元素也一并copy过来
                new_finish = uninitialized_copy(position, finish, new_finish);
            }
            catch (...) {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
            //释放原vector
            destroy(begin(), end());
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }

    }

    template <typename T, class Alloc>
    void vector<T, Alloc>::insert(iterator position, size_t n, T const &x) {
        if (n != 0) {
            if (end_of_storage - finish >= n) {
                //备用空间足够
                T x_copy = x;
                const size_t elems_after = finish - position;
                iterator old_finish = finish;
                if (elems_after > n) {
                    //插入点之后的现有元素个数小于等于新增元素个数
                    //finish之后的空间都没有construct
                    uninitialized_copy(finish - n, finish, finish);

                    finish += n;
                    std::copy_backward(position, old_finish - n, old_finish);
                    std::fill(position, position + n, x_copy);
                }
                else {
                    uninitialized_fill_n(finish, n - elems_after, x_copy);
                    finish += n - elems_after;
                    uninitialized_copy(position, old_finish, finish);
                    finish += elems_after;
                    std::fill(position, old_finish, x_copy);
                }
            }
            else {
                //备用空间不够
                const size_t old_size = size();
                const size_t len = old_size + std::max(old_size, n);
                iterator  new_start = data_allocator::allocate(len);
                iterator new_finish = new_start;

                try {
                    //将旧vector的插入点之前的元素复制到新空间
                    new_finish = uninitialized_copy(start, position, new_start);
                    //将新元素填入新空间
                    new_finish = uninitialized_fill_n(new_finish, n, x);
                    //将就vector插入点之后的元素复制到新空间
                    new_finish = uninitialized_copy(position, finish, new_finish);
                }
                catch (...) {
                    destroy(new_start, new_finish);
                    data_allocator::deallocate(new_start, len);
                    throw;
                }
                //释放旧的vector
                destroy(start, finish);
                deallocate();

                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
            }
        }

    }

    template <typename T, class Alloc>
    void vector<T, Alloc>::insert(iterator position, T const &x) {
        insert(position, 1, x);
    };
}
#endif //MINISTL_MINISTL_VECTOR_H
