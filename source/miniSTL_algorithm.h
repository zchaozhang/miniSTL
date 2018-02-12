//
// Created by 张智超 on 2018/2/9.
//

#ifndef MINISTL_MINISTL_ALGORITHM_H
#define MINISTL_MINISTL_ALGORITHM_H
#include "miniSTL_pair.h"
#include <type_traits>
namespace zzc {
    template <typename T>
    struct identity {
        T operator()(T x) {
            return x;
        }
    };
    template <typename T>
    struct identity<const T> {
        T operator()(T x) {
            return x;
        }
    };
    //
    template <typename T>
    struct less {
        bool operator()(T t1, T t2) {
            return t1 < t2;
        }
    };

    template <typename T>
    struct select1st {
        typename T::first_type operator ()(T const& t) {
            return t.first;
        }
    };
}

#endif //MINISTL_MINISTL_ALGORITHM_H
