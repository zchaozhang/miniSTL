//
// Created by 张智超 on 2018/2/9.
//

#ifndef MINISTL_MINISTL_ALGORITHM_H
#define MINISTL_MINISTL_ALGORITHM_H
namespace zzc {
    template <typename T>
    struct identity {
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
}

#endif //MINISTL_MINISTL_ALGORITHM_H
