#ifndef ROOM_PARSER_DETAIL_ATOM_SET_H
#define ROOM_PARSER_DETAIL_ATOM_SET_H

#include <string>

namespace room {
namespace parser {
namespace detail {

template<class T>
using StripType = std::remove_cv<
                    typename std::remove_reference<T>::type>;

/**
 * @brief The AtomSet struct
 *
 * This struct represents either atom or set (which can hold
 * other atoms and sets). The aim of the structure is
 * represent tree-like general structure of Room-program.
 *
 * For example, this Room-program:
 * @code
 *      x = {
 *          a >> (123)
 *          => b
 *      }
 *
 *      y = {}
 * @endcode
 *
 * can be represented by following AtomSet structure:
 *
 * @verbatim
 *      (head)
 *       |
 *       v
 *      [x] -> {} -> [y] -> [=] -> {}
 *            /
 *          [a] -> { } -> [=>] -> [b]
 *                 /|\
 *              [1][2][3]
 *
 *      where:
 *      ->           : denotes siblings relation
 *      /,| and \    : parent-children (from top to down) relation
 *      [some...]    : atom
 *      {}           : set (as you can see, set can contain other sets and atoms)
 *      (head)       : points to first entity (shown on the diagram just for clarity)
 * @endverbatim
 */
struct AtomSet {
    enum class Type {
        Set, Atom
    };

    struct SetDescription {
        bool quoted = false;
        AtomSet *child = nullptr;
    };

    struct AtomDescription {
        bool quoted = false;
        std::string name;
    };



    template<class Description>
    AtomSet(Description&& descr, typename std::enable_if<std::is_same<typename StripType<Description>::type, SetDescription>::value>::type* = nullptr)
        : type{Type::Set}
    {
        new (&set) Description{std::forward<Description>(descr)};
    }

    template<class Description>
    AtomSet(Description&& descr, typename std::enable_if<std::is_same<typename StripType<Description>::type, AtomDescription>::value>::type* = nullptr)
        : type{Type::Atom}
    {
        new (&atom) Description{std::forward<Description>(descr)};
    }

    AtomSet(AtomSet &&)
    noexcept (std::is_nothrow_move_constructible<SetDescription>::value &&
              std::is_nothrow_move_constructible<AtomDescription>::value);

    AtomSet(const AtomSet &) = delete;
    ~AtomSet();

    AtomSet & operator = (AtomSet &&)
    noexcept (std::is_nothrow_move_assignable<SetDescription>::value &&
              std::is_nothrow_move_assignable<AtomDescription>::value);

    AtomSet & operator = (const AtomSet &);


    Type type;
    AtomSet* sibling = nullptr;

    union {
        SetDescription set;
        AtomDescription atom;
    };
};

} // namespace detail
} // namespace parser
} // namespace room

#endif // ROOM_PARSER_DETAIL_ATOM_SET_H
