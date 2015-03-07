#ifndef ROOM_AST_SYMBOL_H
#define ROOM_AST_SYMBOL_H

#include <functional>
#include "room/lexer/token.h"
#include "room/ast/symbol.h"

namespace room {
namespace ast {

class Symbol {
public:
    Symbol(bool quote)
        : quoted(quote)
    {}

    virtual ~Symbol()
    {}

    const bool quoted;    // lexicaly, any symbol can be quoted
};

} // namespace ast
} // namespace room

#endif // ROOM_AST_SYMBOL_H
