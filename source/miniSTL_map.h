//
// Created by 张智超 on 2018/2/12.
//

#ifndef MINISTL_MINISTL_MAP_H
#define MINISTL_MINISTL_MAP_H
#include "miniSTL_algorithm.h"
#include "miniSTL_pair.h"
#include "miniSTL_alloc.h"
#include "miniSTL_RB_tree.h"
namespace zzc {
    template <typename Key, typename T, typename Compare = less<Key>, typename Alloc = alloc>
    class map {
        using key_type    = Key;
        using date_type   = T;
        using mapped_type = T;
        using value_type  = pair<Key, T>;
        using key_compare = Compare;

    private:
        using rep_type = rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc>;
        rep_type t;

    public:
        using pointer         = typename rep_type::pointer;
        using const_pointer   = const pointer;
        using reference       = typename rep_type::reference;
        using const_reference = const reference;
        using iterator        = typename rep_type::iterator;
        using const_iterator  = const iterator;

        map() : t(Compare{}) {}

        iterator begin() { return t.begin(); }
        iterator end() { return t.end(); }
        bool empty() { return t.empty(); }
        size_t size() { return t.size(); }

        pair<iterator, bool> insert(value_type const& x) {
            t.insert_unique(x);
        };

        T& operator[] (key_type const& k) {
            return t.insert_unique(value_type{k, T{}}).first->second;
        }

        void erase(iterator pos) {
            t.erase(pos);
        }

        void clear() {
            t.clear();
        }

        iterator find(key_type const& k) {
            return t.find(k);
        }

    };
}



#endif //MINISTL_MINISTL_MAP_H
