//
// Created by 张智超 on 2018/2/10.
//

#ifndef MINISTL_MINISTL_SET_H
#define MINISTL_MINISTL_SET_H
#include "miniSTL_algorithm.h"
#include "miniSTL_RB_tree.h"
#include "miniSTL_alloc.h"
#include "miniSTL_pair.h"
namespace zzc {//
    template <typename Key, typename Compare = less<Key>, typename Alloc = alloc>
    class set {
    public:
        using key_type        = Key;
        using value_type      = Key;
        using key_compare     = Compare;
        using value_compare   = Compare;
    private:
        using rep_type        = rb_tree<key_type, value_type,
                identity<value_type>, key_compare, Alloc>;

        rep_type t;
    public:
        using pointer         = typename rep_type::pointer;
        using const_pointer   = const pointer;
        using reference       = typename rep_type::reference;
        using const_reference = const reference;
        using iterator        = typename rep_type::iterator;
        using const_iterator  = iterator;

        set() : t(Compare{}){};
        explicit set(Compare const& comp) : t(comp) {}

        iterator begin() { return t.begin(); }
        iterator end() { return t.end(); }
        bool empty() { return t.empty(); }
        size_t size() { return t.size(); }

        pair<iterator, bool> insert(value_type const& v) {
            pair<typename rep_type::iterator, bool> p = t.insert_unique(v);
            return pair<iterator, bool>{p.first, p.second};
        };

        void erase(iterator pos) {
            t.erase(const_cast<typename rep_type::iterator>(pos));
        }

        void clear() {
            t.clear();
        }

        iterator find(key_type const& key) {
            return t.find(key);
        }

    };
}



#endif //MINISTL_MINISTL_SET_H
