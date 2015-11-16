#ifndef ROOM_PARSER_DETAIL_ATOM_SET_H
#define ROOM_PARSER_DETAIL_ATOM_SET_H

#include <string>

namespace room {
namespace parser {
namespace detail {

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
        Atom, Set
    };

    Type type;
    AtomSet* sibling = nullptr;

    union {
        struct {
            bool quoted = false;
            AtomSet *child = nullptr;
        } set;

        struct {
            bool quoted = false;
            std::string name;
        } atom;
    } description;
};

} // namespace detail
} // namespace parser
} // namespace room

#endif // ROOM_PARSER_DETAIL_ATOM_SET_H
