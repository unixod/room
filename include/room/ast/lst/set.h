#ifndef ROOM_AST_LST_SET_H
#define ROOM_AST_LST_SET_H

#include <vector>
#include <memory>
#include "room/ast/symbol.h"

namespace room {
namespace ast {

class Set : public Symbol {
public:
    Set(bool quoted)
        : Symbol(quoted)
    {}

    std::vector<std::unique_ptr<Symbol>> elements;
};

} // namespace ast
} // namespace room

#endif // ROOM_AST_LST_SET_H


