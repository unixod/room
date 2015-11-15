#ifndef ROOM_PARSER_DETAIL_ATOM_TREE_H
#define ROOM_PARSER_DETAIL_ATOM_TREE_H

namespace room {
namespace parser {
namespace detail {

class AtomTree {
public:
    struct Node;

};

struct AtomDescription;

struct AtomTree::Node {
    enum class Type {
        Atom, Set
    };

    Type type;
    Node* sibling = nullptr;
    bool quoted = false;

    union {
        Node *child;
        AtomDescription *descr;
    };
};

} // namespace detail
} // namespace parser
} // namespace room

#endif // ROOM_PARSER_DETAIL_ATOM_TREE_H
