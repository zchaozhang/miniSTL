//
// Created by 张智超 on 2018/1/31.
//

#ifndef MINISTL_MINISTL_RB_TREE_H
#define MINISTL_MINISTL_RB_TREE_H

#include <iostream>
#include "miniSTL_iterator.h"
#include "miniSTL_alloc.h"
#include "miniSTL_pair.h"

namespace zzc {
    enum class __rb_tree_color_type {
        red, black
    };

    struct __rb_tree_node_base {
        using color_type = __rb_tree_color_type;
        using base_ptr   = __rb_tree_node_base*;


        color_type color;
        base_ptr parent;
        base_ptr left;
        base_ptr right;

        static base_ptr minimum(base_ptr x) {
            while (x->left != nullptr)
                x = x->left;
            return x;
        }

        static base_ptr maximum(base_ptr x) {
            while (x->right != nullptr)
                x = x->right;
            return x;
        }
    };

    template <typename T>
    struct __rb_tree_node : public __rb_tree_node_base {
        T value;
        using link_type = __rb_tree_node<T>*;
    };






    struct __rb_tree_base_iterator {
        using base_ptr          = __rb_tree_node_base::base_ptr;
        using iterator_category = bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;

        base_ptr node;

        void increment() {
            //如果有右子节点，就向右走
            if (node->right != nullptr) {
                node = node->right;
                //然后左子树走到底就是解
                while (node->left != nullptr)
                    node = node->left;
            }
            //没有右子节点，找其父节点
            else {
                base_ptr y = node->parent;
                //如果现行节点本身是右子节点，就一只上溯，直到不是右子节点
                while (node == y->right) {
                    node = y;
                    y = y->parent;
                }
                //如果到了根节点,且根节点无右子节点
                if (node->right != y) {
                    node = y;
                }
            }
        }

        void decrement() {
            if (node->color == __rb_tree_color_type::red && node->parent->parent == node) {
                //比较特殊的情况，end()指向的header，node == header, header的右子节点即mostright
                node = node->right;
            }
            else if (node->left != nullptr) {
                base_ptr y = node->left;
                while (y->right != nullptr) {
                    y = y->right;
                }
                node = y;
            }
            else {
                base_ptr y = node->parent;
                while (node == y->left) {
                    node = y;
                    y = y->parent;
                }
                node = y;
            }
        }
    };



    template <typename T>
    struct __rb_tree_iterator :public __rb_tree_base_iterator {
        using value_type   = T;
        using reference    = T&;
        using pointer      = T*;
        using iterator     = __rb_tree_iterator<T>;
        using link_type    = __rb_tree_node<T>*;


        __rb_tree_iterator() = default;

        explicit __rb_tree_iterator(link_type x) {node = x;}

        __rb_tree_iterator(iterator const& x) {node = x.node;}

        reference operator*() {
            return static_cast<link_type>(node)->value;
        }

        pointer operator->() {
            return &(operator*());
        }

        iterator& operator++() {
            increment();
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            increment();
            return temp;
        }

        iterator& operator--() {
            decrement();
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            decrement();
            return temp;
        }


        friend bool operator == (__rb_tree_iterator const& i1, __rb_tree_iterator const i2) {
            return i1.node == i2.node;
        }

        friend bool operator != (__rb_tree_iterator const& i1, __rb_tree_iterator const i2) {
            return !(i1 == i2);
        }
    };

    //左旋操作
    inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base *&root) {
        //x为旋转点
        __rb_tree_node_base* y = x->right;
        x->right = y->left;
        if (y->left != nullptr)
            y->left->parent = x;
        y->parent = x->parent;

        if (x == root) {
            root = y;
        }
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    //右旋操作
    inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base *&root) {
        //x为旋转点
        __rb_tree_node_base* y = x->left;
        x->left = y->right;
        if (y->right != nullptr)
            y->right->parent = x;
        y->parent = x->parent;

        if (x == root)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }




    //红黑树的真正操作                       新增节点               root节点
    inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base *& root) {
        using color_t = __rb_tree_color_type;
        x->color = color_t::red;       //新节点必为红
        while (x != root && x->parent->color == color_t::red) {//父节点为红
            if (x->parent == x->parent->parent->left) { //父节点为祖父节点的左子节点
                __rb_tree_node_base* y = x->parent->parent->right;//y为伯父节点
                if (y && y->color == color_t::red) { //伯父结点为红
                    x->parent->color = color_t::black;//将更改父节点，伯父节点为红，祖父节点为黑
                    y->color = color_t::black;
                    x->parent->parent->color = color_t::red;
                    x = x->parent->parent;
                }
                else {//伯父节点为黑
                    //新节点为父节点的右子节点, 就先左旋归正
                    if (x == x->parent->right) {
                        x = x->parent;
                        __rb_tree_rotate_left(x, root);
                    }
                    //新节点如果为父节点的左子节点，直接右旋
                    //变颜色之后右旋
                    x->parent->color = color_t::black;
                    x->parent->parent->color = color_t::red;
                    __rb_tree_rotate_right(x->parent->parent, root);
                }

            }//跟上面的过程相似，只不过是相反的，就不写注释了
            else {
                __rb_tree_node_base* y = x->parent->parent->left; //令y为伯父节点
                if (y && y->color == color_t::red) {
                    x->parent->color = color_t::black;
                    y->color = color_t::black;
                    x->parent->parent->color = color_t::red;
                    x = x->parent->parent;
                }
                else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        __rb_tree_rotate_right(x, root);
                    }
                    x->parent->color = color_t::black;
                    x->parent->parent->color = color_t::red;
                    __rb_tree_rotate_left(x->parent->parent, root);
                }
            }
        }
        //根节点永远为黑
        root->color = color_t::black;
    }


    inline __rb_tree_node_base* __rb_tree_erase(__rb_tree_node_base* z, __rb_tree_node_base *& root,
                                __rb_tree_node_base *& leftmost, __rb_tree_node_base *& rightmost) {
        using color_t = __rb_tree_color_type;

        __rb_tree_node_base* y = z;
        __rb_tree_node_base* x = nullptr;
        __rb_tree_node_base* x_parent = nullptr;

        if (y->left == nullptr) {
            //z最多有一个非空子节点
            x = y->right;// x可能为空
        }
        else if (y->right == nullptr) {
            x = y->left; //x一定不为空
        }
        else {
            y = y->right; // z有两个非空子节点， x此时可能为空
            while (y->left != nullptr)
                y = y->left;

            x = y->right;
        }

        if (y != z) {//z存在两个子节点的时候   y代替z, y是z的后继节点
                     //x为后继节点的右子节点
            z->left->parent = y;
            y->left = z->left;

            if (y != z->right) {//z的右子节点存在左子节点
                x_parent = y->parent;

                if (x) //y的右子节点不为空
                    x->parent = y->parent;

                y->parent->left = x;
                y->right = z->right;
                z->right->parent = y;
            }
            else {//z的右子节点的左子节点为空时，x == nullptr
                x_parent = y;
            }

            if (z == root) {//删除的节点为根节点
                root = y;
            }
            else if (z->parent->left == z) {//z为其父节点的左子节点
                z->parent->left = y;
            }
            else {//z为其父节点的右子节点
                z->parent->right = y;
            }
            y->parent = z->parent;
            std::swap(y->color, z->color);
            y = z;
            //待删节点此时的颜色为后继节点的颜色
            //调整y = 后继节点
        }
        else {// y == z， z至多一个非空节点
              // 此时x为z的子节点
            x_parent = y->parent;

            if (x)
                x->parent = y->parent;

            if (z == root)
                root = x;
            else {
                if (z->parent->left == z) {//z是其父节点的左子节点
                    z->parent->left = x;
                }
                else {
                    z->parent->right = x;
                }

                if (leftmost == z) {//z为最小的值，及z为树最左的叶子节点
                    if (z->right == nullptr) //z此时一定没有左子节点
                        leftmost = z->parent;
                    else
                        leftmost = __rb_tree_node_base::minimum(x);

                }

                if (rightmost == z) {//z为最大值，及z为树的最右的叶子结点
                    if (z->left == nullptr)
                        leftmost = z->parent;
                    else
                        rightmost = __rb_tree_node_base::maximum(x);
                }
            }

        }
        //现在y是待删除点
        //x是后继节点的子节点
        if (y->color != color_t::red) {
            //如果y为黑，就一定会发生失衡（非根节点），失衡从后继节点子节点开始
            //删除某个节点后，会用它的后继节点来填上，
            // 并且后继节点会设置为和删除节点同样的颜色，所以删除节点的那个位置是不会破坏平衡的。
            while (x != root && ( x == nullptr || x->color == color_t::black)) {
                if (x == x_parent->left) {
                    __rb_tree_node_base* w = x_parent->right;
                    if (w->color == color_t::red) {
                        w->color = color_t::black;
                        x_parent->color = color_t::red;
                        __rb_tree_rotate_left(x_parent, root);
                        w = x_parent->right;
                    }

                    //w的两个儿子节点都为黑色
                    if ((w->left == nullptr || w->left->color == color_t::black) &&
                            (w->right == nullptr || w->right->color == color_t::black)) {
                        w->color = color_t::red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else {
                        //右为黑
                        if (w->right == nullptr || w->right->color == color_t::black) {
                            if (w->left)
                                w->left->color = color_t::black;
                            w->color = color_t::red;
                            __rb_tree_rotate_right(w, root);
                            w = x_parent->right;
                        }

                        w->color = x_parent->color;
                        x_parent->color = color_t::black;

                        if (w->right)
                            w->right->color = color_t::black;
                        __rb_tree_rotate_left(x_parent, root);
                        break;
                    }
                }//跟上面相似，一个左边一个右边
                else {
                    __rb_tree_node_base* w = x_parent->left;

                    if (w->color == color_t::red) {
                        w->color = color_t::black;
                        x_parent->color = color_t::red;
                        __rb_tree_rotate_right(x_parent, root);
                        w = x_parent->left;
                    }

                    //w的两个儿子节点都为黑色
                    if ((w->left == nullptr || w->left->color == color_t::black) &&
                        (w->right == nullptr || w->right->color == color_t::black)) {
                        w->color = color_t::red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else {
                        //右为黑
                        if (w->right == nullptr || w->right->color == color_t::black) {
                            if (w->left)
                                w->left->color = color_t::black;
                            w->color = color_t::red;
                            __rb_tree_rotate_left(w, root);
                            w = x_parent->right;
                        }

                        w->color = x_parent->color;
                        x_parent->color = color_t::black;

                        if (w->right)
                            w->right->color = color_t::black;
                        __rb_tree_rotate_right(x_parent, root);
                        break;
                    }
                }

            }

        }
        if (x)
            x->color = color_t::black;
        return y;
    }













    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = alloc>
    class rb_tree {
    protected:
        using void_pointer           = void*;
        using base_ptr               = __rb_tree_node_base*;
        using rb_tree_node           = __rb_tree_node<Value>;
        using rb_tree_node_allocator = simple_alloc<rb_tree_node, Alloc>;
        using color_type             = __rb_tree_color_type;

    public:

        using key_type               = Key;
        using value_type             = Value;
        using pointer                = value_type *;
        using const_pointer          = const pointer;
        using reference              = value_type &;
        using const_reference        = const reference;
        using link_type              = rb_tree_node *;
        using difference_type        = ptrdiff_t;

    protected:
        link_type get_node() { return rb_tree_node_allocator::allocate(); }
        void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

        link_type create_node(value_type const& x) {
            link_type temp = get_node();
            construct(&temp->value, x);
            return temp;
        }

        link_type clone_node(link_type x) {
            link_type temp = create_node(x->value);
            temp->color    = x->color;
            temp->left     = nullptr;
            temp->right    = nullptr;
            temp->parent   = nullptr;
            return temp;
        }

        void destroy_node(link_type p) {
            destroy(&p->value);    //析构value
            put_node(p);           //释放内存
        }

        size_t    node_count;         //树的大小 （节点数量）
        link_type header;
        Compare   key_compare;


        link_type& root() const { return reinterpret_cast<link_type&>(header->parent); }
        link_type& leftmost() const { return reinterpret_cast<link_type&>(header->left); }
        link_type& rightmost() const { return reinterpret_cast<link_type&>(header->right); }

        static link_type&  left(link_type x) { return reinterpret_cast<link_type&>(x->left);}
        static link_type&  right(link_type x) { return reinterpret_cast<link_type&>(x->right);}
        static link_type&  parent(link_type x) { return reinterpret_cast<link_type&>(x->parent);}
        static reference   value(link_type x) { return x->value;}
        static color_type& color(link_type x) { return x->color;}

        static link_type&  left(base_ptr x) { return reinterpret_cast<link_type&>(x->left);}
        static link_type&  right(base_ptr x) { return reinterpret_cast<link_type&>(x->right);}
        static link_type&  parent(base_ptr x) { return reinterpret_cast<link_type&>(x->parent);}
        static reference   value(base_ptr x) { return reinterpret_cast<link_type&>(x)->value;}
        static color_type& color(base_ptr x) { return x->color;}
        static const Key& key(base_ptr x) { return KeyOfValue{}(value(x));}

        static link_type& minimum(link_type x) {
            return static_cast<link_type&>(__rb_tree_node_base::minimum(x));
        }
        static link_type& maximum(link_type x) {
            return static_cast<link_type&>(__rb_tree_node_base::maximum(x));
        }

    public:
        using iterator = __rb_tree_iterator<value_type>;


    private:

        iterator __insert(base_ptr x, base_ptr y, value_type const& v);

        void __erase(link_type x);
        void init() {
            header = get_node();
            color(header) = color_type::red;

            root() = nullptr;
            leftmost() = header;
            rightmost() = header;//自己指向自己
        }

        void M_erase(link_type x);

    public:

        rb_tree() : node_count(0), key_compare(){ init(); }
        explicit rb_tree(Compare const& compe) : node_count(0), key_compare{compe} {init();}
        iterator begin() { return iterator{leftmost()}; }
        iterator end() { return iterator{rightmost()}; }
        bool empty() { return node_count == 0; }
        size_t size() { return node_count;}
        pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
                insert_unique(Value const& v);

        typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
        insert_equal(Value const& v);

        iterator find(Key const& key);

        void erase(iterator pos);

        void clear() {
            if (node_count != 0) {
                M_erase(root());
                leftmost() = header;
                rightmost() = header;
                root() = nullptr;
                node_count = 0;
            }
        }
        ~rb_tree(){ clear(); put_node(header);}

    };

    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator             //                        x为新插入点， y为插入点的父节点
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr _x, base_ptr _y,
                                                              const value_type &v) {
        link_type x = static_cast<link_type>(_x);
        link_type y = static_cast<link_type>(_y);

//        if (x == nullptr) {
//            std::cout << "x = null" << std::endl;
//        }

        link_type z;
        if (y == header || x != nullptr || key_compare(KeyOfValue{}(v), key(y))) {
            z = create_node(v);
            left(y) = z;
            if (y == header) {
                root() = z;
                rightmost() = z;
            }
            else if (y == leftmost()) {
                leftmost() = z;
            }
        }
        else {
            z = create_node(v);
            right(y) = z;
            if (y == rightmost())
                rightmost() = z;
        }
        parent(z) = y;
        left(z) = nullptr;
        right(z) = nullptr;
        __rb_tree_rebalance(z, header->parent);
        ++node_count;
        return iterator{z};
    }

    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value &v) {
        link_type y = header;
        link_type x = root();
        bool comp = true;
        while (x != nullptr) {
            y = x;
            comp = key_compare(KeyOfValue{}(v), key(x));

            x = comp ? left(x) : right(x);
        }
        //离开while后，y所指即插入点之父节点

        iterator j = iterator{y};
        if (comp) {
            if (j == begin()) {
                return pair<iterator, bool>{__insert(x, y, v), true};
            }
            else
                --j;
        }
        //std::cout << "y key  " << key(j.node) << "   " << KeyOfValue{}(v) << "  " << key_compare(key(j.node), KeyOfValue{}(v))<< std::endl;
        if (key_compare(key(j.node), KeyOfValue{}(v))) {
            //键值不存在重复，插入
            return pair<iterator, bool>{__insert(x, y, v), true};
        }

        return pair<iterator, bool>{j, false};
    }

    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const Value &v) {
        link_type y = header;
        link_type x = root();

        while (x) {
            y = x;
            x = key_compare(KeyOfValue{}(v), key(x)) ? left(x) : right(x);
        }
        return __insert(x, y, v);
    }

    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(rb_tree::iterator pos) {
        link_type y = static_cast<link_type>(__rb_tree_erase(pos.node, header->parent, header->left, header->right));
        destroy_node(y);
        --node_count;
    }

    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::M_erase(rb_tree::link_type x) {
        //递归删除
        //删除且不用平衡
        while (x) {
            M_erase(right(x));
            link_type y = left(x);
            destroy_node(x);
            x = y;
        }
    }

    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key &k) {
        link_type y = header;
        link_type x = root();

        while (x) {
            if (!key_compare(key(x), k))
                y = x, x = left(x);
            else
                x = right(x);
        }

        iterator j = iterator{y};
        return (j == end() || key_compare(k, key(y))) ? end() : j;
    }


}


#endif //MINISTL_MINISTL_RB_TREE_H
