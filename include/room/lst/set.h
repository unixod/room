#ifndef ROOM_LST_SET_H
#define ROOM_LST_SET_H

#include <vector>
#include <memory>
#include "room/lst/symbol.h"

namespace room {
namespace lst {

class Set : public Symbol {
public:
    Set(bool quoted)
        : Symbol(quoted)
    {}

    std::vector<std::unique_ptr<Symbol>> elements;
};

} // namespace lst
} // namespace room

#endif // ROOM_LST_SET_H


