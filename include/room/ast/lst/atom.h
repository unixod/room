#ifndef ROOM_AST_LST_ATOM_H
#define ROOM_AST_LST_ATOM_H

#include <string>
#include "room/ast/symbol.h"

namespace room {
namespace ast {

class Atom : public Symbol {
public:
    Atom(std::string _name, bool quoted)
        : Symbol(quoted), name{std::move(_name)}
    {}

    const std::string name;
};

} // namespace ast
} // namespace room

#endif // ROOM_AST_LST_ATOM_H


