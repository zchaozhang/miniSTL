//
// Created by 张智超 on 2018/1/20.
//

#ifndef MINISTL_MINITSL_TYPE_TRAITS_H
#define MINISTL_MINITSL_TYPE_TRAITS_H
namespace zzc {//
    struct true_type{};
    struct false_type{};

    template <typename type>
    struct type_traits{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = false_type;
        using has_trivial_copy_constrictor     = false_type;
        using has_trivial_assignment_operator  = false_type;
        using has_trivial_destructor           = false_type;
        using is_POD_type                      = false_type;
    };

    template <>
    struct type_traits<char>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<signed char>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<unsigned char>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<short>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<unsigned short>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<int>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<unsigned int>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<long>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<unsigned long>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<float >{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<double>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<long long>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<unsigned long long>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <>
    struct type_traits<long double>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };

    template <typename T>
    struct type_traits<T*>{
        using this_dummy_memeber_must_be_first = true_type;
        using has_trivial_default_constrictor  = true_type;
        using has_trivial_copy_constrictor     = true_type;
        using has_trivial_assignment_operator  = true_type;
        using has_trivial_destructor           = true_type;
        using is_POD_type                      = true_type;
    };
}
#endif //MINISTL_MINITSL_TYPE_TRAITS_H
