//
// Created by 张智超 on 2018/1/20.
//

#ifndef MINISTL_MINSTL_ALLOC_H
#define MINISTL_MINSTL_ALLOC_H
#include <new>
#include <iostream>
#include <functional>
#include "miniSTL_construct.h"
#include "miniSTL_type_traits.h"
#include <memory>
#if 0
    #define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
    #define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; exit(1)
#endif
namespace zzc {//
    using void_func_void = std::function<void ()>;

    //第一级配置器
    template <int inst>
    class __malloc_alloc_template {
    private:
        //oom : out of memory.
        static void *oom_malloc(size_t);
        static void *oom_realloc(void *, size_t);
        static void_func_void __malloc_alloc_oom_handler;


    public:
        static void * allocate(size_t n) {
            void *res = malloc(n);
            if (res == nullptr)
                res = oom_malloc(n);
            return res;
        }

        static void deallocate(void* p, size_t ) {
            free(p);
        }
        static void deallocate(void* p) {
            free(p);
        }

        static void * reallocate(void* p, size_t, size_t new_sz) {
            void *res = realloc(p, new_sz);
            if (res == nullptr)
                res = oom_realloc(p, new_sz);
            return res;
        }

        //仿c++的set_new_handler()
        static void_func_void set_malloc_handler(void_func_void f) {
            void_func_void old = __malloc_alloc_oom_handler;
            __malloc_alloc_oom_handler = f;
            return old;
        }


    };


    template <int inst>
    void_func_void __malloc_alloc_template<inst>::__malloc_alloc_oom_handler = nullptr;

    template <int inst>
    void * __malloc_alloc_template<inst>::oom_malloc(size_t n) {
        void_func_void my_malloc_handler;
        void *res;
        while (1) {
            my_malloc_handler = __malloc_alloc_oom_handler;
            if (my_malloc_handler == nullptr) {
                __THROW_BAD_ALLOC;
            }
            my_malloc_handler();
            res = malloc(n);
            if (res)
                return res;
        }

    }

    template <int inst>
    void * __malloc_alloc_template<inst>::oom_realloc(void *p, size_t n) {
        void_func_void my_malloc_handler;
        void *res;
        while(1) {
            my_malloc_handler = __malloc_alloc_oom_handler;
            if (my_malloc_handler == nullptr) {
                __THROW_BAD_ALLOC;
            }
            my_malloc_handler();
            res = realloc(p, n);
            if (res)
                return res;
        }
    }

    using alloc = __malloc_alloc_template<0>;
//
//
//
//
//
//    //第二级配置器(对小额内存的分配，防止内存碎片
//
//    //小额区块的内存为8的倍数
//    enum : size_t {__ALIGN = 8}; // 小型区块的上调边界
//    enum : size_t {__MAX_BYTES = 128}; // 小型区块的上界
//    enum : size_t {__NFREELISTS = __MAX_BYTES / __ALIGN}; // free-lists个数
//
//    template <bool threads, int inst>
//    class __default_alloc_template {
//    private:
//        //将byte上调至8的倍数
//        static size_t ROUND_UP(size_t bytes) {
//            return ((bytes) + __ALIGN - 1) & ~(__ALIGN - 1);
//        }
//        //free-lists的节点构造
//        union obj {
//            union obj* free_list_link;
//            char client_data[1];
//        };
//
//        static obj * volatile free_list[__NFREELISTS];
//        //获取free_list的索引号
//        static size_t FREELIST_INDEX(size_t bytes) {
//            return (bytes + __ALIGN - 1) / __ALIGN - 1;
//        }
//
//
//    };


    template <typename T, class Alloc>
    class simple_alloc {
    public:
        static T * allocate(size_t n) {
            return 0 == n ? nullptr : static_cast<T *>(Alloc::allocate(n * sizeof(T)));
        }

        static T * allocate() {
            return static_cast<T *>(Alloc::allocate(sizeof(T)));
        }

        static void deallocate(T *p, size_t n) {
            if (0 != n)
                Alloc::deallocate(p, n * sizeof(T));
        }

        static void deallocate(T *p) {
            Alloc::deallocate(p);
        }
    };

    //----------------fill------------------------
    //是pod类型则用std::fill
    template <typename Iterator, typename T>
    inline void __uninitialized_fill_aux(Iterator first, Iterator last, true_type, T const& x) {
        return std::fill(first, last, x);

    };

    template <typename Iterator, typename T>
    inline Iterator __uninitialized_fill_aux(Iterator first, Iterator last, false_type, T const& x) {
        Iterator cur;
        for (; cur != last; ++cur)
            construct(&*cur, x);
        return cur;
    };

    template <typename Iterator, typename T>
    inline void uninitialized_fill(Iterator first, Iterator last, T const& values) {
        using type = std::remove_pointer_t<Iterator>;
        __uninitialized_fill_aux(first, last, typename zzc::type_traits<type>::is_POD_type{}, values);
    };
    //------------fill_n-------------------------
    template <typename Iterator, typename T>
    inline void __uninitialized_fill_n_aux(Iterator first, size_t n,  true_type, T const& values) {
        return std::fill_n(first, n, values);
    };

    template <typename Iterator, typename T>
    inline Iterator __uninitialized_fill_n_aux(Iterator first, size_t n, false_type, T const& values) {
        Iterator cur = first;
        for (; n > 0; --n, ++cur) {
            construct(&*cur, values);
        }
        return cur;
    };


    template <typename Iterator, typename T>
    inline Iterator uninitialized_fill_n(Iterator first, size_t n, T const& values) {
        using type = std::remove_pointer_t<Iterator>;

        return __uninitialized_fill_n_aux(first, n, typename type_traits<type>::is_POD_type{}, values);
    };


    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, true_type) {
        ForwardIterator cur = result;
        return std::copy(first, last, result);
    };

    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, false_type) {
        ForwardIterator cur = result;
        for (; first != last; ++first, ++cur) {
            construct(&*cur, *first);
        }
        return cur;
    };

    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
        using type = std::remove_pointer_t<ForwardIterator>;
        return __uninitialized_copy_aux(first, last, result, typename type_traits<type>::is_POD_type{});
    };

}


#endif //MINISTL_MINSTL_ALLOC_H
