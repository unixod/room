#ifndef ROOM_STD_C_PLUS_PLUS_14_H
#define ROOM_STD_C_PLUS_PLUS_14_H

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

/// borrowed from: http://isocpp.org/files/papers/N3656.txt
namespace std {

/// make_unique
template<class T> struct _Unique_if {
    typedef unique_ptr<T> _Single_object;
};

template<class T> struct _Unique_if<T[]> {
    typedef unique_ptr<T[]> _Unknown_bound;
};

template<class T, size_t N> struct _Unique_if<T[N]> {
    typedef void _Known_bound;
};

template<class T, class... Args>
    typename _Unique_if<T>::_Single_object
    make_unique(Args&&... args) {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

template<class T>
    typename _Unique_if<T>::_Unknown_bound
    make_unique(size_t n) {
        typedef typename remove_extent<T>::type U;
        return unique_ptr<T>(new U[n]());
    }

template<class T, class... Args>
    typename _Unique_if<T>::_Known_bound
    make_unique(Args&&...) = delete;

/// index_sequence
template<std::size_t...>
struct index_sequence {};

template<std::size_t n, std::size_t... seq>
struct make_index_sequence : make_index_sequence<n-1, n-1, seq...> {};

template<std::size_t... seq>
struct make_index_sequence<0, seq...> : index_sequence<seq...> {};
}

#endif // ROOM_STD_C_PLUS_PLUS_14_H