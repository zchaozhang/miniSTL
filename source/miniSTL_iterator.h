//
// Created by 张智超 on 2018/1/20.
//

#ifndef MINISTL_MINISTL_ITERATOR_H
#define MINISTL_MINISTL_ITERATOR_H

#include <cstddef>

namespace zzc{
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag : public input_iterator_tag{};
    struct bidirectional_iterator_tag : public forward_iterator_tag{};
    struct random_access_iterator_tag : public bidirectional_iterator_tag{};

    template  <typename Category, typename T, typename Distance = ptrdiff_t, typename Pointer = T*, class Reference = T& >
    struct iterator {
        using iterator_category = Category;
        using value_type        = T;
        using difference_type   = Distance;
        using pointer           = Pointer;
        using reference         = Reference;
    };

    template <typename Iterator>
    struct iterator_traits {
        using iterator_category = typename Iterator::iterator_category;
        using value_type        = typename Iterator::value_type ;
        using difference_type   = typename Iterator::difference_type;
        using pointer           = typename Iterator::pointer;
        using reference         = typename Iterator::reference;


    };

    //对原生指针(native pointer)

    template <typename T>
    struct iterator_traits<T*> {
        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    };

    template <typename T>
    struct iterator_traits<T const*> {
        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T const*;
        using reference         = T const&;
    };
}


#endif //MINISTL_MINISTL_ITERATOR_H
