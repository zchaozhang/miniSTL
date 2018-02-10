//
// Created by 张智超 on 2018/1/19.
//

#ifndef MINISTL_MINISTL_CONSTRUCT_H
#define MINISTL_MINISTL_CONSTRUCT_H
#include <new>
#include <utility>
#include <type_traits>
namespace zzc {
    template <typename T, typename Ts>
    inline void construct(T* p, Ts const& values) {
        new (p) T(values);
    };

    template <typename T>
    inline void destroy(T* p) {
        p->~T();
    }

    template <typename Iterator>
    inline void __destroy(Iterator& , Iterator&);



    template <bool, typename Iterator>
    struct __destroy_aux {
        void operator()(Iterator, Iterator){}
    };

    template <typename Iterator>
    inline void destroy(Iterator first, Iterator end) {
        __destroy(first, end);
    }



    template <typename Iterator>
    inline void __destroy(Iterator& first, Iterator& end) {
        __destroy_aux<std::is_trivially_destructible_v<std::decay_t<Iterator>>, Iterator>{}(first, end);

    };

    template <typename Iterator>
    struct __destroy_aux<true, Iterator>{
        void operator()(Iterator first, Iterator end){
            for (; first != end; ++first)
                destroy(&*first);
        }
    };
}


#endif //MINISTL_MINISTL_CONSTRUCT_H
