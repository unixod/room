#ifndef ROOM_AST_SET_H
#define ROOM_AST_SET_H

#include <vector>
#include <memory>
#include "room/ast/symbol.h"
#include "room/lst/symbol.h"

namespace room {
namespace ast {

class Set : public Symbol, public lst::Symbol {
public:
    Set(bool quoted)
        : lst::Symbol(quoted)
    {}

    std::vector<std::unique_ptr<lst::Symbol>> elements;
};

} // namespace ast
} // namespace room

#endif // ROOM_AST_SET_H


