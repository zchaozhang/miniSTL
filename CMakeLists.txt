cmake_minimum_required(VERSION 3.9)
project(miniSTL)

set(CMAKE_CXX_STANDARD 17)



add_executable(miniSTL main.cpp miniSTL_construct.h miniSTL_alloc.h miniSTL_type_traits.h miniSTL_iterator.h miniSTL_vector.h miniSTL_deque.h miniSTL_heap_algorithm.h miniSTL_priority_queue.h miniSTL_RB_tree.h miniSTL_pair.h miniSTL_algorithm.h miniSTL_set.h)