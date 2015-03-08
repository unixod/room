#ifndef ROOM_LST_SYMBOL_H
#define ROOM_LST_SYMBOL_H

#include <functional>
#include "room/lexer/token.h"
#include "room/lst/symbol.h"

namespace room {
namespace lst {

class Symbol {
public:
    Symbol(bool quote)
        : quoted(quote)
    {}

    virtual ~Symbol()
    {}

    const bool quoted;    // lexicaly, any symbol can be quoted
};

} // namespace lst
} // namespace room

#endif // ROOM_LST_SYMBOL_H
