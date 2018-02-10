//
// Created by 张智超 on 2018/2/3.
//

#ifndef MINISTL_MINISTL_PAIR_H
#define MINISTL_MINISTL_PAIR_H

#include <utility>

namespace zzc {//
    template <typename T1, typename T2>
    struct pair {
        using first_type = T1;
        using second_type = T2;
        first_type first;
        second_type second;
        pair() : first{}, second{} {}
        pair(first_type const& f, second_type const& s) : first{f}, second{s}{}
        pair(first_type && f, first_type && s) : first{std::move(f)}, second{std::move(s)}{}
    };
}


#endif //MINISTL_MINISTL_PAIR_H
