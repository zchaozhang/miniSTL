//
// Created by 张智超 on 2018/1/28.
//

#ifndef MINISTL_MINISTL_HEAP_ALGORITHM_H
#define MINISTL_MINISTL_HEAP_ALGORITHM_H

#include <cstddef>

namespace zzc {//
    template <typename T>
    void swap(T& a, T& b) noexcept {
        T temp = a;
        a = b;
        b = temp;
    }

    template <typename RandomAccessIterator, typename T>
    inline void __push_heap(RandomAccessIterator first, size_t holeIndex, size_t topIndex, T value) {
        size_t parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && *(first + parent) < value) {
            //令当前洞值为父值
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    };


    template <typename RandomAccessIterator>
    inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
        __push_heap(first, (last - first - 1), 0, *(last - 1));
    }

    template <typename RandomAccessIterator, typename T>
    inline void __pop_heap(RandomAccessIterator first, size_t hole, size_t len, T value) {
        size_t top = hole;
        size_t rightChild = 2 * hole + 2;
        while (rightChild < len) {
            if (*(first + rightChild) < *(first + (rightChild - 1)))
                --rightChild;
            *(first  + hole) = *(first + rightChild);
            hole = rightChild;
            rightChild = 2 * (rightChild + 1);
        }

        if (len == rightChild) {
            *(first + hole) = *(first + (rightChild - 1));
            hole = rightChild - 1;
        }

        __push_heap(first, hole, top, value);
    }

    template <typename RandomAccessIterator>
    inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
        swap(*first, *(last - 1));
        __pop_heap(first, 0, (last - first - 1), *first);
    }

    template <typename RandomAccessIterator>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
        while (last - first > 1) {
            zzc::pop_heap(first, last--);
        }
    }


    template <typename RandomAccessIterator>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
        if (last - first < 2)
            return;

        size_t len = last - first;
        size_t hole = (len - 2) / 2;

        while (1) {
            __pop_heap(first, hole, len, *(first + hole));
            if (!hole)
                return;
            --hole;
        }

    }
}




#endif //MINISTL_MINISTL_HEAP_ALGORITHM_H
